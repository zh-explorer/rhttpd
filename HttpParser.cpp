//
// Created by explorer on 2020/12/24.
//

#include "HttpParser.h"
#include "globalContext.h"
#include "httpProcess.h"
#include "parseHelper.h"
#include "cstring"

HttpParser::HttpParser(AIO *io) {
    this->io = io;
    std::function<void(HttpParser *)> f = [](HttpParser *c) -> void {
        auto *resp = c->parse();
        c->return_resp(resp);
    };

    std::function<void(Coroutine *, HttpParser *)> end = [](Coroutine *coro, HttpParser *c) -> void {
        delete c;
    };

    this->coroutine = new Coroutine(f, this);
    this->coroutine->add_done_callback(end, this);
    current_loop->add_to_loop(this->coroutine);
}

#define MAX_HEADER = 0x10000


Response *HttpParser::parse() {
    auto *req = new Request();

    // parse method
    unsigned char *end_line;
    end_line = this->read_line();
    if (end_line == nullptr) {
        delete req;
        return new Response(414, "line too large");
    }


    int space_idx;
    space_idx = parseHelper::find_space(reinterpret_cast<const char *>(this->line_start), end_line - this->line_start);
    if (space_idx == -1) {
        delete req;
        return new Response(400);
    }
    enum Method m = parseHelper::parse_method(reinterpret_cast<const char *>(this->line_start), space_idx);
    if (m == other) {
        delete req;
        return new Response(405);
    }
    req->method = m;

    this->update_idx(space_idx + 1); // must update current index every time
    this->skip_space();

    // parse url
    space_idx = parseHelper::find_space(reinterpret_cast<const char *>(this->line_start), end_line - this->line_start);
    if (space_idx == -1) {
        delete req;
        return new Response(400);
    }
    unsigned char *url;
    unsigned int url_size;
    std::tie(url, url_size) = this->url_decode(this->line_start, space_idx);
    if (!this->url_check(url, url_size)) {
        delete req;
        return new Response(403);
    }

    req->set_url(url, url_size);
    this->update_idx(space_idx + 1);
    this->skip_space();

    // parse http version, support 1.0/1.1
//    space_idx = helper.find_space(reinterpret_cast<const char *>(this->line_start), end_line - this->line_start);
//    if (space_idx == -1) {
//        delete req;
//        return new Response(400);
//    }

    if (strncmp(reinterpret_cast<const char *>(this->line_start), "HTTP/1.0", 8) != 0) {
        if (strncmp(reinterpret_cast<const char *>(this->line_start), "HTTP/1.1", 8) != 0) {
            delete req;
            return new Response(505);
        }
    }
    // ignore version
    this->line_start = end_line + 2;

    //parse http header
    while (true) {
        end_line = this->read_line();
        if (end_line == nullptr) {
            return new Response(400);
        }
        if (end_line == this->line_start) {
            break;
        }
        int semicolon_idx = parseHelper::find_ch(reinterpret_cast<const char *>(this->line_start),
                                                 end_line - this->line_start, ':');
        if (semicolon_idx == -1 || semicolon_idx == 0) {
            continue;       // skip this error header;
        }

        unsigned int key_size = semicolon_idx;
        auto *key = static_cast<unsigned char *>(malloc(key_size + 1));
        memcpy(key, this->line_start, key_size);
        key[key_size] = 0;
        this->update_idx(semicolon_idx + 1);
        this->skip_space();

        unsigned int value_size = end_line - this->line_start;
        auto *value = static_cast<unsigned char *>(malloc(value_size + 1));
        memcpy(value, this->line_start, value_size);
        value[value_size] = 0;
        req->add_header(key, key_size, value, value_size);

        this->line_start = end_line + 2;
    }
    // the http request header parse finish
    auto resp = globalContext::p->process(req);
    delete req;
    return resp;
}

unsigned char *HttpParser::read_line() {
    // find /r/n in exist buffer
    unsigned char *i;
    for (i = this->line_start; i < this->line_end - 1; i++) {
        if (*i == '\r' && *(i + 1) == '\n') {
            return i;
        }
    }
    // check if remain buffer is enough
    if (LINE_BUFFER_SIZE - (this->line_start - this->line_buffer) < LINE_MAX) {
        memmove(this->line_buffer, this->line_start, this->line_end - this->line_start);
        this->line_end = this->line_buffer + (this->line_end - this->line_start);
        this->line_start = this->line_buffer;
    }

    while (true) {
        unsigned char *search_start = this->line_end;
        int re = this->io->read_any(this->line_end, LINE_BUFFER_SIZE - (this->line_end - this->line_buffer),
                                    -1);
        if (re == -1) {
            return nullptr;
        }
        this->line_end += re;
        for (i = search_start - 1; i < this->line_end - 1; i++) {
            if (*i == '\r' && *(i + 1) == '\n') {
                return i;
            }
        }
        // too long line
        if ((this->line_end - this->line_start) > LINE_MAX) {
            return nullptr;
        }
    }
}

void HttpParser::update_idx(unsigned int size) {
    this->line_start += size;
}

void HttpParser::skip_space() {
    while (*this->line_start == ' ') {
        this->line_start++;
    }
}

bool HttpParser::url_check(unsigned char *buffer, unsigned int size) {
    if (buffer == nullptr || size == 0) {
        return false;
    }
    for (unsigned int i = 0; i < size - 1; i++) {
        if (buffer[i] == '.' && buffer[i + 1] == '.') {
            return false;
        }
    }
    return true;
}

std::tuple<unsigned char *, unsigned int> HttpParser::url_decode(unsigned char *buffer, unsigned int size) {
    auto *url = static_cast<unsigned char *>(malloc(size));
    unsigned int i = 0;
    unsigned int j = 0;
    while (i < size) {
        if (buffer[i] != '%') {
            if (buffer[i] >= 'A' && buffer[i] <= 'Z') {
                url[j] = buffer[i] + 32;
            } else {
                url[j] = buffer[i];
            }
            j++;
            i++;
        } else {
            if (i + 2 >= size) {
                // error, no enough char
                free(url);
                return {nullptr, 0};
            }

            // TODO: wtf
            unsigned char c;
            if (buffer[i + 1] >= '0' && buffer[i + 1] <= '9') {
                c = (buffer[i + 1] - '0') * 16;
            } else if (buffer[i + 1] >= 'a' && buffer[i + 1] <= 'z') {
                c = (buffer[i + 1] - 'a' + 10) * 16;
            } else if (buffer[i + 1] >= 'A' && buffer[i + 1] <= 'Z') {
                c = (buffer[i + 1] - 'A' + 10) * 16;
            } else {
                free(url);
                return {nullptr, 0};
            }

            if (buffer[i + 2] >= '0' && buffer[i + 2] <= '9') {
                c += (buffer[i + 2] - '0');
            } else if (buffer[i + 2] >= 'a' && buffer[i + 2] <= 'z') {
                c += (buffer[i + 2] - 'a' + 10);
            } else if (buffer[i + 2] >= 'A' && buffer[i + 2] <= 'Z') {
                c += (buffer[i + 2] - 'A' + 10);
            } else {
                free(url);
                return {nullptr, 0};
            }

            url[j] = c;
            j++;
            i += 3;
        }
    }
    return {url, j};
}



void HttpParser::return_resp(Response *resp) {

    resp->return_resp(this->io);
}

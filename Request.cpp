//
// Created by explorer on 2020/12/29.
//

#include "Request.h"
#include "globalContext.h"
#include <cstring>

void Request::set_url(unsigned char *url, unsigned int size) {
    if (size == 1 && url[0] == '/') {
        this->url = static_cast<unsigned char *>(malloc(globalContext::index_file_len + 1));
        this->url_size = globalContext::index_file_len;
        strcpy(reinterpret_cast<char *>(this->url), globalContext::index_file);
    } else {
        this->url = static_cast<unsigned char *>(malloc(size + 1));
        memcpy(this->url, url, size);
        this->url_size = size;
    }
}

#include <Coroutine/log.h>

void Request::add_header(unsigned char *key, unsigned int key_size, unsigned char *value, unsigned int value_size) {
    try {
        str_value &v = this->headers.search(key, key_size);
        v.set_value(value, value_size);
    } catch (NotFindException &e) {
        this->headers.insert(key, key_size, {value, value_size});
    }
}

void Request::add_get_param(unsigned char *key, unsigned int key_size, unsigned char *value, unsigned int value_size) {
    char tmp1[0x100];
    char tmp2[0x100];
    strncpy(tmp1, reinterpret_cast<const char *>(key), key_size);
    tmp1[key_size] = 0;
    strncpy(tmp2, reinterpret_cast<const char *>(value), value_size);
    tmp2[value_size] = 0;
    logger(INFO, stderr, "get: %s->%s", tmp1, tmp2);

    try {
        str_value &v = this->get_param.search(key, key_size);
        v.set_value(value, value_size);
    } catch (NotFindException &e) {
        this->get_param.insert(key, key_size, {value, value_size});
    }
}


Request::~Request() {
    free(url);

//    auto f = [](str_value &v) -> void {
//        delete v.buf;
//        v.size = 0;
//        v.buf = nullptr;
//    };
//    this->headers.iter(f);
}

void Request::parse_cookie() {
    try {
        auto &cookie = this->headers.search((unsigned char *) "Cookie", 6);
        unsigned char *p = cookie.buf;
        unsigned int remain = cookie.size;
        while (true) {
            int split_idx = parseHelper::find_ch(p, remain, ';');
            if (split_idx == -1) {
                int equ_idx = parseHelper::find_ch(p, remain, '=');
                if (equ_idx != -1) {
                    this->cookie.insert(p, equ_idx, {p + equ_idx + 1, remain - equ_idx - 1});
                }
                break;
            }
            int equ_idx = parseHelper::find_ch(p, split_idx, '=');
            if (equ_idx != -1) {
                this->cookie.insert(p, equ_idx, {p + equ_idx + 1, static_cast<unsigned int>(split_idx - equ_idx - 1)});
            }
            split_idx += 1;
            while (p[split_idx] == ' ') {
                split_idx++;
            }
            p += split_idx;
            remain -= split_idx;
        }
    } catch (NotFindException &e) {
        return;
    }
}


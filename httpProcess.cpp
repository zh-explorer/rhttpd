//
// Created by explorer on 2020/12/31.
//

#include "httpProcess.h"
#include <cstdio>
#include <sys/stat.h>
#include "globalContext.h"
#include "Coroutine/log.h"
#include "httpHandler.h"
#include "KeyMatch.h"

Response *httpProcess::process(Request *req) {
    auto resp = new Response(200);

    // add some base data
    resp->set_header("Server", "nginx/1.14.0 (Ubuntu)");
    char *s = parseHelper::get_date();
    resp->set_header("Date", s);
    free((void *) s);
    req->parse_cookie();
    try {
        auto *handler = globalContext::url_matcher.search(reinterpret_cast<char *>(req->url), req->url_size);
        if (handler) {
            handler->process(resp, req);
        }
    } catch (NotFindException &e) {
        // check is this is a static router
        FILE *static_file = this->get_static_file(req->url, req->url_size);
        if (static_file) {
            this->process_static_file(resp, req, static_file);
        } else {
            resp->set_status_code(404);
            return resp;
        }
    }

    char const *content_type = parseHelper::parse_content_type(req->url, req->url_size);
    resp->set_header("Content-Type", content_type);

    resp->set_header("Accept-Ranges", "bytes");
    resp->update_header_buffer();

    return resp;
}

void httpProcess::process_static_file(Response *resp, Request *req, FILE *static_file) {
    resp->set_body(static_file);
    unsigned int file_length = parseHelper::get_file_length(static_file);
    char tmp_buf[100];
    snprintf(tmp_buf, 100, "%u", file_length);
    resp->set_header("Content-Length", tmp_buf);

    // set Last-Modified
    char *buf = parseHelper::file_mtime(fileno(static_file));
    resp->set_header("Last-Modified", buf);
    free(buf);
}

FILE *httpProcess::get_static_file(const unsigned char *url, unsigned int size) {
    if (size < globalContext::static_route_len) {
        return nullptr;
    }
    if (strncmp(reinterpret_cast<const char *>(url), globalContext::static_res_router,
                globalContext::static_route_len)) {
        return nullptr;
    }

    auto *buffer = static_cast<unsigned char *>(malloc(globalContext::document_root_len + size + 1));
    memset(buffer, 0, globalContext::document_root_len + size + 1);
    strncpy(reinterpret_cast<char *>(buffer), globalContext::document_root, globalContext::document_root_len);
    strncat(reinterpret_cast<char *>(buffer), reinterpret_cast<const char *>(url), size);

    logger(INFO, stderr, "%s", buffer);

    struct stat statbuf{};
    if (stat(reinterpret_cast<const char *>(buffer), &statbuf) || !S_ISREG(statbuf.st_mode)) {
        return nullptr;
    }

    FILE *fp = fopen(reinterpret_cast<const char *>(buffer), "r");
    free(buffer);
    return fp;
}



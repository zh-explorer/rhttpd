//
// Created by explorer on 2020/12/29.
//

#include "Request.h"

void Request::set_url(unsigned char *url, unsigned int size) {
    this->url = url;
    this->size = size;
}

void Request::add_header(unsigned char *key, unsigned int key_size, unsigned char *value, unsigned int value_size) {
    try {
        header_value &v = this->headers.search(key, key_size);
        free(v.buf);
        v.buf = value;
        v.size = value_size;
    } catch (NotFindException &e) {
        this->headers.insert(key, key_size, {value, value_size});
    }
}

Request::~Request() {
    free(url);

    auto f = [](header_value v) -> void {
        delete v.buf;
        v.size = 0;
        v.buf = nullptr;
    };
    this->headers.iter(f);
}

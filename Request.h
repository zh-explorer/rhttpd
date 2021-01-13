//
// Created by explorer on 2020/12/29.
//

#ifndef RHTTPD_REQUEST_H
#define RHTTPD_REQUEST_H

#include "KeyMatch.h"
#include "parseHelper.h"
#include <cstring>
#include <Coroutine/AIO.h>

class str_value {
public:
    unsigned char *buf = nullptr;
    unsigned int size = 0;

    str_value() = default;

    str_value &operator=(str_value &&value) noexcept {
        this->buf = value.buf;
        this->size = value.size;
        value.buf = nullptr;
        value.size = 0;
        return *this;
    }
//
//    str_value(str_value &v) {
//        this->buf = static_cast<unsigned char *>(malloc(v.size));
//        this->size = v.size;
//        memcpy(this->buf, v.buf, v.size);
//    }

    str_value(str_value &&v) noexcept {
        this->buf = v.buf;
        this->size = v.size;
        v.buf = nullptr;
        v.size = 0;
    }

    str_value(unsigned char *buf, unsigned int size) {
        this->buf = static_cast<unsigned char *>(malloc(size + 1));
        memcpy(this->buf, buf, size);
        this->buf[size] = 0;
        this->size = size;
    }

    void set_value(unsigned char *buf, unsigned int size) {
        free(this->buf);
        this->buf = static_cast<unsigned char *>(malloc(size + 1));
        memcpy(this->buf, buf, size);
        this->buf[size] = 0;
        this->size = size;
    }

    void set_value(std::tuple<unsigned char *, unsigned int> tuple) {
        unsigned char *buf;
        unsigned int size;
        std::tie(buf, size) = tuple;
        this->buf = static_cast<unsigned char *>(malloc(size + 1));
        memcpy(this->buf, buf, size);
        this->buf[size] = 0;
        this->size = size;
    }

    ~str_value() {
        free(this->buf);
        this->size = 0;
    }
};

class Request {
public:
    explicit Request(AIO *io) : io(io) {};

    void set_url(unsigned char *url, unsigned int size);

    void add_header(unsigned char *key, unsigned int key_size, unsigned char *value, unsigned int value_size);

    void add_get_param(unsigned char *key, unsigned int key_size, unsigned char *value, unsigned int value_size);

    void parse_cookie();

    void set_remain(str_value v) {
        this->remain = std::move(v);
    }

    unsigned char *url = nullptr;
    unsigned int url_size{};
    enum Method method;

    KeyMatch<unsigned char, str_value, unsigned char *> headers;
    KeyMatch<unsigned char, str_value, unsigned char *> get_param;
    KeyMatch<unsigned char, str_value, unsigned char *> cookie;
    str_value remain;
    AIO *io = nullptr;

    ~Request();
};


#endif //RHTTPD_REQUEST_H

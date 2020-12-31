//
// Created by explorer on 2020/12/29.
//

#ifndef RHTTPD_REQUEST_H
#define RHTTPD_REQUEST_H

#include "KeyMatch.h"
#include "parseHelper.h"

struct header_value {
    unsigned char *buf;
    unsigned int size;
};

class Request {
public:
    void set_url(unsigned char *url, unsigned int size);

    void add_header(unsigned char *key, unsigned int key_size, unsigned char *value, unsigned int value_size);

    unsigned char *url = nullptr;
    unsigned int size;
    enum Method method;

    KeyMatch<unsigned char, header_value, unsigned char *> headers;

    ~Request();
};


#endif //RHTTPD_REQUEST_H

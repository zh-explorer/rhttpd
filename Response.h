//
// Created by explorer on 2020/12/29.
//

#ifndef RHTTPD_RESPONSE_H
#define RHTTPD_RESPONSE_H

#include <tuple>
#include <vector>

#include <Coroutine/array_buf.h>
#include <cstring>
#include <Coroutine/AIO.h>

class Response {
public:
    explicit Response(int status_code) : status_code(status_code) {};

    Response(int status_code, const char *content) : status_code(status_code) {};

    void set_header(const char *key, const char *value) {
        header.write((unsigned char *) key, strlen(key));
        header.write((unsigned char *) ": ", 2);
        header.write((unsigned char *) value, strlen(value));
        header.write((unsigned char *) "\r\n", 2);
    }

    void set_body(unsigned char *data, unsigned int size) {
        this->body.write(data, size);
    }

    void set_body(FILE *file) {
        this->body_file = file;
        this->is_file = true;
    }

    int get_status_code() const {
        return status_code;
    };

    void set_status_code(int code) {
        status_code = code;
    };

    void return_resp(AIO *io);

    ~Response();

private:
    bool is_file = false;
    FILE *body_file = nullptr;
    array_buf header;
    array_buf body;
    int status_code;
};


#endif //RHTTPD_RESPONSE_H

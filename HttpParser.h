//
// Created by explorer on 2020/12/24.
//

#ifndef RHTTPD_HTTPPARSER_H
#define RHTTPD_HTTPPARSER_H


#include <Coroutine/AIO.h>
#include <Coroutine/Coroutine.h>
#include <functional>
#include <tuple>
#include "Response.h"
#include "Request.h"

#define LINE_MAX 0x1000
#define LINE_BUFFER_SIZE 0x2000

class HttpParser {
public:
    explicit HttpParser(AIO *io);

    Response *parse();


private:
    AIO *io;
    Coroutine *coroutine;

    unsigned char line_buffer[LINE_BUFFER_SIZE];
    unsigned char *line_start = line_buffer;
    unsigned char *line_end = line_buffer;

    unsigned char *read_line();

    void update_idx(unsigned int size);

    void skip_space();

    static std::tuple<unsigned char *, unsigned int> url_decode(unsigned char *buffer, unsigned int size);

    static bool url_check(unsigned char *buffer, unsigned int size);

    void return_resp(Response *resp);

    ~HttpParser() {
        delete io;
        coroutine->destroy();
    }
};


#endif //RHTTPD_HTTPPARSER_H

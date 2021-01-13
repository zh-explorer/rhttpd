//
// Created by explorer on 2020/12/31.
//

#ifndef RHTTPD_HTTPPROCESS_H
#define RHTTPD_HTTPPROCESS_H

#include "Response.h"
#include "Request.h"

class httpProcess {
public:
    Response *process(Request *);

    static void process_static_file(Response *resp, Request *req, FILE *static_file);

    static FILE *get_static_file(const unsigned char *url, unsigned int size);


};


#endif //RHTTPD_HTTPPROCESS_H

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
};


#endif //RHTTPD_HTTPPROCESS_H

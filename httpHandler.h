//
// Created by explorer on 2021/1/5.
//

#ifndef RHTTPD_HTTPHANDLER_H
#define RHTTPD_HTTPHANDLER_H


#include "Request.h"
#include "Response.h"

class user;

class httpHandler {
public:
    virtual void process(Response *resp, Request *req) = 0;
};

class login : public httpHandler {
public:
    void process(Response *resp, Request *req) override;
};

class reg : public httpHandler {
public:
    void process(Response *resp, Request *req) override;

private:
    static void resp_error(Response *resp, char *err1, char *err2, char *err3);
};

class check_user : public httpHandler {
public:
    void process(Response *resp, Request *req) override;

    user * check(Response *resp, Request *req);
};

class submit_page : public check_user {
    void process(Response *resp, Request *req) override;
};

class submit : public check_user {
public:
    void process(Response *resp, Request *req) override;
};

#endif //RHTTPD_HTTPHANDLER_H

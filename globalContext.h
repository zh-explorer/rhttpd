//
// Created by explorer on 2020/12/31.
//

#ifndef RHTTPD_GLOBALCONTEXT_H
#define RHTTPD_GLOBALCONTEXT_H


#include "KeyMatch.h"

class httpProcess;

class user;

class globalContext {
public:
    static httpProcess *p;
    static const char *static_res_router;
    static unsigned int static_route_len;
    static const char *document_root;
    static unsigned int document_root_len;
    static const char *index_file;
    static unsigned int index_file_len;

    static KeyMatch<char, class httpHandler *, char *> url_matcher;
    static KeyMatch<char, class user *, char *> users;

    static void init();
};


#endif //RHTTPD_GLOBALCONTEXT_H

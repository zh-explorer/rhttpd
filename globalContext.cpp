//
// Created by explorer on 2020/12/31.
//

#include "globalContext.h"
#include <cstring>
#include "httpHandler.h"

httpProcess *globalContext::p = nullptr;
//const char *globalContext::static_res_router = "/static/";
//const char *globalContext::document_root = "/var/www/html";


const char *globalContext::static_res_router = "/";
const char *globalContext::document_root = "/var/www/html/";
const char *globalContext::index_file = "/login.html";

unsigned int globalContext::static_route_len = 0;
unsigned int globalContext::document_root_len = 0;
unsigned int globalContext::index_file_len = 0;

KeyMatch<char, class httpHandler *, char *> globalContext::url_matcher = {};
KeyMatch<char, class user *, char *> globalContext::users = {};

void globalContext::init() {
    static_route_len = strlen(static_res_router);
    document_root_len = strlen(document_root);
    index_file_len = strlen(index_file);

    url_matcher.insert("/login", 6, new login());
    url_matcher.insert("/register", 9, new reg());
    url_matcher.insert("/submit.html", 12, new submit_page());
    url_matcher.insert("/2048/index.html", 16, new check_user());
    url_matcher.insert("/submit", 7, new submit());
}

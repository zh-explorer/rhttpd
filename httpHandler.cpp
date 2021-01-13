//
// Created by explorer on 2021/1/5.
//

#include "httpHandler.h"
#include <cstring>
#include "Request.h"
#include "globalContext.h"
#include "KeyMatch.h"
#include "user.h"
#include <cstring>
#include "httpProcess.h"

void login::process(Response *resp, Request *req) {
    try {
        auto &name = req->get_param.search((unsigned char *) "name", 4);
        auto &passwd = req->get_param.search((unsigned char *) "passwd", 6);

        auto *user = globalContext::users.search(reinterpret_cast<char *>(name.buf), name.size);
        if (user->password.length() != passwd.size && strncmp(user->password.c_str(),
                                                              reinterpret_cast<const char *>(passwd.buf),
                                                              passwd.size)) {
            resp->set_status_code(302);
            resp->set_header("Location", "/login.html");
            return;
        } else {

            char * tmp_buf = static_cast<char *>(malloc(passwd.size + 6));
            snprintf(tmp_buf, passwd.size + 6, "pass=%s", passwd.buf);
            tmp_buf[passwd.size + 5] = 0;
            resp->set_header("Set-Cookie", tmp_buf);
            free(tmp_buf);

            tmp_buf = static_cast<char *>(malloc(name.size + 6));
            snprintf(tmp_buf, name.size + 6, "name=%s", name.buf);
            tmp_buf[name.size + 5] = 0;
            resp->set_header("Set-Cookie", tmp_buf);
            free(tmp_buf);

            resp->set_status_code(302);
            resp->set_header("Location", "/2048/index.html");
            return;
        }
    } catch (NotFindException &e) {
        resp->set_status_code(302);
        resp->set_header("Location", "/login.html");
        return;
    }
}

void reg::process(Response *resp, Request *req) {

    try {
        req->get_param.search((unsigned char *) "passwd", 6);
    } catch (NotFindException &e) {
        this->resp_error(resp, "", "require", "");
        return;
    }

    try {
        req->get_param.search((unsigned char *) "name", 4);
    } catch (NotFindException &e) {
        this->resp_error(resp, "require", "", "");
        return;
    }

    try {
        req->get_param.search((unsigned char *) "re_passwd", 9);
    } catch (NotFindException &e) {
        this->resp_error(resp, "", "", "require");
        return;
    }


    auto &passwd = req->get_param.search((unsigned char *) "passwd", 6);
    auto &re_passwd = req->get_param.search((unsigned char *) "re_passwd", 9);
    if (re_passwd.size != passwd.size || memcmp(re_passwd.buf, passwd.buf, re_passwd.size)) {
        resp_error(resp, "", "Password and confirm password not consistent!", "");
        return;
    }
    auto &name = req->get_param.search((unsigned char *) "name", 4);
    try {
        globalContext::users.search(reinterpret_cast<char *>(name.buf), name.size);
        resp_error(resp, "name exist", "", "");
        return;
    } catch (NotFindException &e) {

    }

    auto u = new user(reinterpret_cast<char *>(name.buf), reinterpret_cast<char *>(passwd.buf));
    globalContext::users.insert(reinterpret_cast<char *>(name.buf), name.size, u);


    char *tmp_buf = static_cast<char *>(malloc(name.size + 6));
    snprintf(tmp_buf, name.size + 6, "name=%s", name.buf);
    tmp_buf[name.size + 5] = 0;
    resp->set_header("Set-Cookie", tmp_buf);

    tmp_buf = static_cast<char *>(realloc(tmp_buf, passwd.size + 6));
    snprintf(tmp_buf, passwd.size + 6, "pass=%s", passwd.buf);
    tmp_buf[passwd.size + 5] = 0;
    resp->set_header("Set-Cookie", tmp_buf);
    free(tmp_buf);

    resp->set_status_code(302);
    resp->set_header("Location", "/2048/index.html");
}

void reg::resp_error(Response *resp, char *err1, char *err2, char *err3) {
    unsigned char *file_data;
    unsigned int size;
    std::tie(file_data, size) = parseHelper::render("register_err.html", 0x100, err1,
                                                    err2, err3);
    resp->set_body(file_data, size);
    free(file_data);
}

void check_user::process(Response *resp, Request *req) {
    if (this->check(resp, req)) {
        FILE *static_file = httpProcess::get_static_file(req->url, req->url_size);
        if (static_file) {
            httpProcess::process_static_file(resp, req, static_file);
        } else {
            resp->set_status_code(404);
        }
    } else {
        resp->set_status_code(302);
        resp->set_header("Location", "/login.html");
    }
}

user *check_user::check(Response *resp, Request *req) {
    try {
        auto &name = req->cookie.search((unsigned char *) "name", 4);
        auto &passwd = req->cookie.search((unsigned char *) "pass", 4);

        auto user = globalContext::users.search(reinterpret_cast<char *>(name.buf), name.size);
        if (user->password.length() != passwd.size && strncmp(user->password.c_str(),
                                                              reinterpret_cast<const char *>(passwd.buf),
                                                              passwd.size)) {
            return nullptr;
        }
        return user;

    } catch (NotFindException &e) {
        return nullptr;
    }
}

char *def = "<h1><font color=\"#ff0000\">I can eat glass, it doesn't hurt me.</span></font><br></h1><h2><font color=\"#ff9c00\">I can eat glass, it doesn't hurt me.</font></h2><h3><font color=\"#ffff00\">I can eat glass, it doesn't hurt me.</font></h3><h4><font color=\"#00ff00\">I can eat glass, it doesn't hurt me.</font></h4><h5><font style=\"background-color: rgb(255, 255, 255);\" color=\"#00ffff\">I can eat glass, it doesn't hurt me.</font></h5><h6><font color=\"#0000ff\">I can eat glass, it doesn't hurt me.</font></h6><font color=\"#9c00ff\">I can eat glass, it doesn't hurt me.</font><br>";

void submit_page::process(Response *resp, Request *req) {
    user *u = this->check(resp, req);
    if (u) {
        unsigned char *file_data;
        unsigned int size;
        if (!u->comment) {
            std::tie(file_data, size) = parseHelper::render("submit.html", 0x1000, def);
        } else {
            std::tie(file_data, size) = parseHelper::render("submit.html", strlen(u->comment) + 1, u->comment);
        }
        resp->set_body(file_data, size);
        free(file_data);
    } else {
        resp->set_status_code(302);
        resp->set_header("Location", "/login.html");
    }
}

void submit::process(Response *resp, Request *req) {
    user *u = this->check(resp, req);
    if (u) {
        if (u->comment) {
            free(u->comment);
        }
        try {
            auto &size = req->headers.search((unsigned char *) "Content-Length", 14);
            int content_size = atoi(reinterpret_cast<const char *>(size.buf));
            if (content_size < 0 || content_size > 0x20000) {
                u->comment = nullptr;
                resp->set_status_code(302);
                resp->set_header("Location", "/login.html");
                return;
            }
            unsigned char *buffer;
            if (req->remain.size >= content_size) {
                auto v = std::move(req->remain);
                buffer = v.buf;
                v.buf = nullptr;
                v.size = 0;
            } else {
                buffer = static_cast<unsigned char *>(malloc(content_size));
                unsigned int remain_size = 0;
                if (req->remain.size) {
                    auto v = std::move(req->remain);
                    memcpy(buffer, v.buf, v.size);
                    remain_size = v.size;
                    v.buf = nullptr;
                    v.size = 0;
                }
                req->io->read(buffer + remain_size, content_size - remain_size, -1);
            }
            if (strncmp("word=", reinterpret_cast<const char *>(buffer), 5)) {
                free(buffer);
                resp->set_status_code(302);
                resp->set_header("Location", "/login.html");
                u->comment = nullptr;
                return;
            }
            int split = parseHelper::find_ch(reinterpret_cast<const unsigned char *>(buffer), content_size, '&');
            if (split == -1) {
                split = content_size;
            }
            int s;
            unsigned char *b;
            std::tie(b, s) = parseHelper::url_decode(reinterpret_cast<unsigned char *>(buffer + 5), split - 5);
            u->comment = (char *) b;
            free(buffer);
            resp->set_status_code(302);
            resp->set_header("Location", "/login.html");
        } catch (NotFindException &) {
            resp->set_status_code(302);
            resp->set_header("Location", "/login.html");
            u->comment = nullptr;
        }
    } else {
        resp->set_status_code(302);
        resp->set_header("Location", "/login.html");
    }
}

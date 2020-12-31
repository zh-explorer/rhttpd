//
// Created by explorer on 2020/12/24.
//

#ifndef RHTTPD_PARSEHELPER_H
#define RHTTPD_PARSEHELPER_H

#include "KeyMatch.h"

enum Method {
    get,
    post,
    head,
    other,
};

class parseHelper {
public:


    static enum Method parse_method(const char *start, unsigned data_size);

    static int find_ch(const char *start, unsigned data_size, unsigned char ch);

    static int find_space(const char *start, unsigned data_size);

    static const char *get_result_phrase(int status_code);

private:
    static KeyMatch<char, enum Method, const char *> method_matcher;

    static std::map<int, const char *> status_phrase;

};


#endif //RHTTPD_PARSEHELPER_H

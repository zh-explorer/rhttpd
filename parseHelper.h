//
// Created by explorer on 2020/12/24.
//

#ifndef RHTTPD_PARSEHELPER_H
#define RHTTPD_PARSEHELPER_H

#include "KeyMatch.h"

class httpHandler;

enum Method {
    get,
    post,
    head,
    other,
};

class parseHelper {
public:

    static char *get_date();

    static char *file_mtime(char *filename);

    static char *file_mtime(int fd);

    static char *http_time(time_t sec);

    static char const *parse_content_type(unsigned char const *url, unsigned int size);

    static enum Method parse_method(const char *start, unsigned data_size);

    static int find_ch(const unsigned char *start, unsigned data_size, unsigned char ch);

    static int find_space(const unsigned char *start, unsigned data_size);

    static const char *get_result_phrase(int status_code);

    static unsigned int get_file_length(FILE *fp);

    static std::tuple<unsigned char *, unsigned int> render(char *filename, unsigned int size, ...);

    static std::tuple<unsigned char *, unsigned int> url_decode(unsigned char *buffer, unsigned int size);

    static bool url_check(unsigned char *buffer, unsigned int size);

private:
    static KeyMatch<char, enum Method, const char *> method_matcher;
    static KeyMatch<char, char const *, char *> content_matcher;

    static std::map<int, const char *> status_phrase;


};


#endif //RHTTPD_PARSEHELPER_H

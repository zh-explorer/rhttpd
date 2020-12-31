//
// Created by explorer on 2020/12/24.
//

#include "parseHelper.h"


KeyMatch<char, enum Method, const char *> parseHelper::method_matcher = {
        {"GET",  3, get},
        {"POST", 4, post},
        {"HEAD", 4, head},
};

std::map<int, const char *> parseHelper::status_phrase = {
        {100, "Continue"},
        {101, "Switching Protocols"},
        {200, "OK"},
        {201, "Created"},
        {202, "Accepted"},
        {203, "Non-Authoritative Information"},
        {204, "No Content"},
        {205, "Reset Content"},
        {206, "Partial Content"},
        {300, "Multiple Choices"},
        {301, "Moved Permanently"},
        {302, "Found"},
        {303, "See Other"},
        {304, "Not Modified"},
        {305, "Use Proxy"},
        {307, "Temporary Redirect"},
        {400, "Bad Request"},
        {401, "Unauthorized"},
        {402, "Payment Required"},
        {403, "Forbidden"},
        {404, "Not Found"},
        {405, "Method Not Allowed"},
        {406, "Not Acceptable"},
        {407, "Proxy Authentication Required"},
        {408, "Request Timeout"},
        {409, "Conflict"},
        {410, "Gone"},
        {411, "Length Required"},
        {412, "Precondition Failed"},
        {413, "Payload Too Large"},
        {414, "URI Too Long"},
        {415, "Unsupported Media Type"},
        {416, "Range Not Satisfiable"},
        {417, "Expectation Failed"},
        {426, "Upgrade Required"},
        {500, "Internal Server Error"},
        {501, "Not Implemented"},
        {502, "Bad Gateway"},
        {503, "Service Unavailable"},
        {504, "Gateway Timeout"},
        {505, "HTTP Version Not Supported"}
};


enum Method parseHelper::parse_method(const char *start, unsigned data_size) {
    try {
        return parseHelper::method_matcher.search(start, data_size);
    } catch (NotFindException &e) {
        return other;
    }
}

int parseHelper::find_space(const char *start, unsigned int data_size) {
    return find_ch(start, data_size, ' ');
}

int parseHelper::find_ch(const char *start, unsigned int data_size, unsigned char ch) {
    for (int i = 0; i < data_size; i++) {
        if (start[i] == ch) {
            return i;
        }
    }
    return -1;
}

const char *parseHelper::get_result_phrase(int status_code) {
    return parseHelper::status_phrase[status_code];
}

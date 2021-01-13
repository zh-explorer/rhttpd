//
// Created by explorer on 2021/1/6.
//

#ifndef RHTTPD_USER_H
#define RHTTPD_USER_H

#include <string>

class user {
public:
    std::string name;
    std::string password;
    char * comment = nullptr;

    user(char *username, char *passwd) {
        this->name = username;
        this->password = passwd;
    }
};


#endif //RHTTPD_USER_H

//
// Created by explorer on 2020/12/29.
//

#include "Response.h"

#include <cstring>
#include <cstdio>
#include "parseHelper.h"

#define BLOCK_SIZE 1024

Response::~Response() {
    if (this->body_file) {
        fclose(this->body_file);
    }
}

void Response::return_resp(AIO *io) {
    char tmp_buf[BLOCK_SIZE];
    int re = snprintf(tmp_buf, BLOCK_SIZE, "HTTP/1.1 %d %s\r\n", this->status_code,
                      parseHelper::get_result_phrase(this->status_code));

    io->write(reinterpret_cast<unsigned char *>(tmp_buf), re, -1);

    while (true) {
        re = this->header.read(reinterpret_cast<unsigned char *>(tmp_buf), BLOCK_SIZE);
        if (re == BLOCK_SIZE) {
            io->write(reinterpret_cast<unsigned char *>(tmp_buf), re, -1);
        } else if (re <= 1022) {    // TODO: overwrite 1
            tmp_buf[re] = '\r';
            tmp_buf[re + 1] = '\n';
            re += 2;
            io->write(reinterpret_cast<unsigned char *>(tmp_buf), re, -1);
            break;
        } else {
            io->write(reinterpret_cast<unsigned char *>(tmp_buf), re, -1);
            io->write((unsigned char *) "\r\n", 2, -1);
            break;
        }
    }

    if (body_file) {
        while (true) {
            re = fread(tmp_buf, 1, BLOCK_SIZE, this->body_file);
            if (re == BLOCK_SIZE) {
                io->write(reinterpret_cast<unsigned char *>(tmp_buf), re, -1);
            } else {
                io->write(reinterpret_cast<unsigned char *>(tmp_buf), re, -1);
                break;
            }
        }
    } else {
        while (true) {
            re = this->body.read(reinterpret_cast<unsigned char *>(tmp_buf), BLOCK_SIZE);
            if (re == 0) {
                break;
            } else if (re == BLOCK_SIZE) {
                io->write(reinterpret_cast<unsigned char *>(tmp_buf), re, -1);
            } else {
                io->write(reinterpret_cast<unsigned char *>(tmp_buf), re, -1);
                break;
            }
        }
    }
}


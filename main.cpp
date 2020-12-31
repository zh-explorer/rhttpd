#include <iostream>

#include "HttpParser.h"

#include "httpProcess.h"
#include "globalContext.h"

void start_server();

int main() {
    setbuf(stdin, nullptr);
    setbuf(stdout, nullptr);
    setbuf(stderr, nullptr);

    globalContext::p = new httpProcess();

    auto *server = new Coroutine(start_server);

    auto *loop = new EventLoop();

    loop->add_to_loop(server);
    loop->loop();

    return 0;
}

void start_server() {
    AIOServer io;
    if (auto re = io.bind(12345) != 0) {
        logger(ERR, stderr, "bind error: %s", strerror(errno));
        exit(-1);
    }
    io.listen(30);
    while (true) {
        auto new_io = io.accept();
        if (new_io == nullptr) {
            logger(ERR, stderr, "accept error");
            break;
        }
        new HttpParser(new_io);
    }
}
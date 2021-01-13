#include <iostream>

#include "HttpParser.h"
#include <sys/socket.h>

#include "httpProcess.h"
#include "globalContext.h"

void start_server();

void recv_stdin();

int main() {
    setbuf(stdin, nullptr);
    setbuf(stdout, nullptr);
    setbuf(stderr, nullptr);

    signal(SIGPIPE, SIG_IGN);

    globalContext::init();
    globalContext::p = new httpProcess();

    auto *server = new Coroutine(start_server);
    auto *exit = new Coroutine(recv_stdin);

    auto *loop = new EventLoop();

    loop->add_to_loop(server);
    loop->add_to_loop(exit);
    loop->loop();

    return 0;
}

void recv_stdin() {
    unsigned char buffer[0x100];
    AIO a(STDIN_FILENO);
    while (true) {
        auto re = a.read_any(buffer, 0x100, -1);
        if (re == -1) {
            exit(0);
        }
    }
}

void start_server() {
    AIOServer io;
    int opt = 1;
    if (setsockopt(io.get_fd(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0) {
        logger(ERR, stderr, "set socket opt error");
        exit(-1);
    }
//    if (auto re = io.bind(12345) != 0) {
//        logger(ERR, stderr, "bind error: %s", strerror(errno));
//        exit(-1);
//    }
    io.listen(30);

    struct sockaddr_in addr;
    unsigned int addr_len;
    addr_len = sizeof(addr);
    auto result = getsockname(io.get_fd(), (struct sockaddr *) &addr, (socklen_t *) &addr_len);
    if (result == -1) {
        printf("can not find port, maybe server is error");
    }

    printf("server start success: http://54.176.255.241:%d", ntohs(addr.sin_port));


    while (true) {
        auto new_io = io.accept();
        if (new_io == nullptr) {
            logger(ERR, stderr, "accept error");
            break;
        }
        new HttpParser(new_io);
    }
}
//
// Created by explorer on 2020/12/31.
//

#include "httpProcess.h"
#include <cstdio>

Response *httpProcess::process(Request *) {
    auto resp = new Response(200);
    resp->set_header("Server", "apbche");
    resp->set_header("Content-Type", "text/plain");
    FILE *f = fopen("/etc/passwd", "r");
    resp->set_body(f);
    return resp;
}

cmake_minimum_required(VERSION 3.10)
project(rhttpd)

set(CMAKE_CXX_STANDARD 14)

add_executable(rhttpd main.cpp HttpParser.cpp HttpParser.h parseHelper.cpp parseHelper.h KeyMatch.h test/test.cpp Response.cpp Response.h Request.cpp Request.h httpProcess.cpp httpProcess.h globalContext.cpp globalContext.h)

target_link_libraries(rhttpd Coroutine)
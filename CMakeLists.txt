cmake_minimum_required(VERSION 3.10)
project(rhttpd)

set(CMAKE_CXX_STANDARD 14)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
#set(CMAKE_CXX_FLAGS -fsanitize=address)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
#SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

add_executable(rhttpd main.cpp HttpParser.cpp HttpParser.h parseHelper.cpp parseHelper.h KeyMatch.h test/test.cpp Response.cpp Response.h Request.cpp Request.h httpProcess.cpp httpProcess.h globalContext.cpp globalContext.h httpHandler.cpp httpHandler.h user.cpp user.h)

target_link_libraries(rhttpd Coroutine)

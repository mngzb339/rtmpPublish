
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNO_CRYPTO")
cmake_minimum_required(VERSION 3.4.1)
add_library( # Sets the name of the library.
        rtmp

        STATIC

        amf.c
        hashswf.c
        log.c
        parseurl.c
        rtmp.c)

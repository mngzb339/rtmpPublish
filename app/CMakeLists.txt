

cmake_minimum_required(VERSION 3.4.1)
# 添加子 cmakeList
add_subdirectory(src/main/cpp/librtmp)
add_library(
        native-lib

        SHARED
        src/main/cpp/native-lib.cpp)


find_library( # Sets the name of the path variable.
        log-lib

        log)

target_link_libraries(
        native-lib
        rtmp
        log)
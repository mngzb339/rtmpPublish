

cmake_minimum_required(VERSION 3.4.1)
# 添加子 cmakeList
add_subdirectory(src/main/cpp/librtmp)
add_library(
        native-lib

        SHARED
        src/main/cpp/native-lib.cpp
        src/main/cpp/VideoChannel.cpp
        src/main/cpp/AudioChannl.cpp)

include_directories(src/main/cpp/include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/src/main/cpp/libs/armeabi-v7a")
find_library( # Sets the name of the path variable.
        log-lib

        log)

target_link_libraries(
        native-lib
        rtmp
        x264
        faac
        log)
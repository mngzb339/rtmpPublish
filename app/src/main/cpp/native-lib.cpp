#include <jni.h>
#include <string>
#include "librtmp/rtmp.h"

#include <x264.h>
extern "C" JNIEXPORT jstring JNICALL
Java_com_luban_publisher_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    RTMP_Alloc();
    x264_picture_t *p = new x264_picture_t;
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_luban_publisher_LivePusher_native_1init(JNIEnv *env, jobject thiz) {
    // TODO: implement native_init()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_luban_publisher_LivePusher_native_1start(JNIEnv *env, jobject thiz, jstring path) {
    // TODO: implement native_start()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_luban_publisher_LivePusher_native_1setVideoEncInfo(JNIEnv *env, jobject thiz, jint width,
                                                            jint height, jint fps, jint bitrate) {
    // TODO: implement native_setVideoEncInfo()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_luban_publisher_LivePusher_native_1pushVideo(JNIEnv *env, jobject thiz, jbyteArray data) {
    // TODO: implement native_pushVideo()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_luban_publisher_LivePusher_native_1stop(JNIEnv *env, jobject thiz) {
    // TODO: implement native_stop()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_luban_publisher_LivePusher_native_1release(JNIEnv *env, jobject thiz) {
    // TODO: implement native_release()
}
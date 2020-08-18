#include <jni.h>
#include <string>
#include "librtmp/rtmp.h"

#include <x264.h>
#include "safe_queue.h"
#include "macro.h"
#include "VideoChannnel.h"

SafeQueue<RTMPPacket *> packet;
VideoChannnel *channnel;

// 指针的引用
void releasePackets(RTMPPacket *&packet) {
    if (packet) {
        DELETE(packet)
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_luban_publisher_LivePusher_native_1init(JNIEnv *env, jobject thiz) {
// 准备一个工具类辅助编码
// 准备一个队列 将打包好的数据数据放入队列
    channnel = new VideoChannnel();
    packet.setReleaseCallback(releasePackets);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_luban_publisher_LivePusher_native_1start(JNIEnv *env, jobject thiz, jstring path) {
    // TODO: implement native_start()
}
/**
 * 初始化编码器
 */
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
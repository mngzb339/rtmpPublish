#include <jni.h>
#include <string>
#include "librtmp/rtmp.h"
#include <x264.h>
#include "safe_queue.h"
#include "macro.h"
#include "VideoChannel.h"

SafeQueue<RTMPPacket *> packet;
VideoChannel *videoChannel = 0;
int isStart = 0;
pthread_t pid;

// 指针的引用
void releasePackets(RTMPPacket *&packet) {
    if (packet) {
        DELETE(packet)
    }
}

void *start(void *args) {
    char *url = static_cast<char *>(args);
    do {
        RTMP *rtmp = RTMP_Alloc();
        if (rtmp) {
            LOGE("rtmp creat faild");
            break;
        }
        RTMP_Init(rtmp);
        // 设置超时时间
        rtmp->Link.timeout = 5;

        int ret = RTMP_SetupURL(rtmp, url);
        if (!ret) {
            LOGE("设置地址失败");
            break;
        }
        //开启输出模式
        RTMP_EnableWrite(rtmp);
        ret =  RTMP_Connect(rtmp, 0);
    } while (0);

    delete url;
    return 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_luban_publisher_LivePusher_native_1init(JNIEnv *env, jobject thiz) {
    // 准备一个工具类辅助编码
    // 准备一个队列 将打包好的数据数据放入队列
    videoChannel = new VideoChannel;

    packet.setReleaseCallback(releasePackets);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_luban_publisher_LivePusher_native_1start(JNIEnv *env, jobject thiz, jstring path_) {
    if (isStart) {
        return;
    }
    const char *path = env->GetStringUTFChars(path_, 0);
    char *url = new char[strlen(path) + 1];
    strcpy(url, path);
    isStart = 1;
    //启动线程
    pthread_create(&pid, 0, start, url);
    env->ReleaseStringUTFChars(path_, path);
}
/**
 * 初始化编码器 s
 * bitrate 马率
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_luban_publisher_LivePusher_native_1setVideoEncInfo(JNIEnv *env, jobject instance,
                                                            jint width, jint height, jint fps,
                                                            jint bitrate) {

    if (videoChannel) {
        videoChannel->setVideoEncInfo(width, height, fps, bitrate);
    }

}

extern "C"
JNIEXPORT void JNICALL
Java_com_luban_publisher_LivePusher_native_1pushVideo(JNIEnv *env, jobject thiz, jbyteArray data) {
}
extern "C"
JNIEXPORT void JNICALL
Java_com_luban_publisher_LivePusher_native_1stop(JNIEnv *env, jobject thiz) {
}
extern "C"
JNIEXPORT void JNICALL
Java_com_luban_publisher_LivePusher_native_1release(JNIEnv *env, jobject thiz) {
}
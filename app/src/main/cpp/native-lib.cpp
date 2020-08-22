#include <jni.h>
#include <string>
#include "librtmp/rtmp.h"
#include <x264.h>
#include "safe_queue.h"
#include "macro.h"
#include "VideoChannel.h"
#include "AudioChannl.h"

SafeQueue<RTMPPacket *> packets;
VideoChannel *videoChannel = 0;
AudioChannel *audioChannel = 0;

int isStart = 0;
pthread_t pid;
uint32_t start_time;
int readyPushing = 0;
// 指针的引用
void releasePackets(RTMPPacket *&packet) {
    if (packet) {
        RTMPPacket_Free(packet);
        delete packet;
        packet = 0;
    }

}
void callback(RTMPPacket *packet) {
    if (packet) {
        packet->m_nTimeStamp = RTMP_GetTime() - start_time;
        packets.push(packet);
    }
}
void *start(void *args) {

    char *url = static_cast<char *>(args);
    RTMP *rtmp = 0;
    do {
        rtmp = RTMP_Alloc();
        if (!rtmp) {
            LOGE("rtmp创建失败38");
            break;
        }
        RTMP_Init(rtmp);
        //设置超时时间 5s
        rtmp->Link.timeout = 5;
        int ret = RTMP_SetupURL(rtmp, url);
        if (!ret) {
            LOGE("rtmp设置地址失败:%s", url);
            break;
        }
        //开启输出模式
        RTMP_EnableWrite(rtmp);
        ret = RTMP_Connect(rtmp, 0);
        if (!ret) {
            LOGE("rtmp开启输出模式连接地址失败:%s", url);
            break;
        }
        ret = RTMP_ConnectStream(rtmp, 0);
        if (!ret) {
            LOGE("rtmp56连接流失败:%s", url);
            break;
        }
        LOGE("rtmp56连接流失败:%s", "我已经链接上啦");

        //准备好了 可以开始推流了
        readyPushing = 1;
        //记录一个开始推流的时间
        start_time = RTMP_GetTime();
        packets.setWork(1);
        RTMPPacket *packet = 0;
        //循环从队列取包 然后发送
        while (isStart) {
            packets.pop(packet);
            if (!isStart) {
                break;
            }
            if (!packet) {
                continue;
            }
            // 给rtmp的流id
            packet->m_nInfoField2 = rtmp->m_stream_id;
            //发送包 1:加入队列发送
            ret = RTMP_SendPacket(rtmp, packet, 1);
            releasePackets(packet);
            if (!ret) {
                LOGE("发送数据失败");
                break;
            }
        }
        releasePackets(packet);

    } while (0);
    isStart = 0;
    readyPushing = 0;
    packets.setWork(0);
    packets.clear();
    if (rtmp) {
        RTMP_Close(rtmp);
        RTMP_Free(rtmp);
    }
    delete url;
    return 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_luban_publisher_LivePusher_native_1init(JNIEnv *env, jobject thiz) {
    // 准备一个工具类辅助编码
    // 准备一个队列 将打包好的数据数据放入队列

    videoChannel = new VideoChannel;
    videoChannel->setVideoCallback(callback);
    packets.setReleaseCallback(releasePackets);
    // 音频率
    audioChannel = new AudioChannel;
    audioChannel->setAudioCallback(callback);


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
Java_com_luban_publisher_LivePusher_native_1pushVideo(JNIEnv *env, jobject thiz, jbyteArray data_) {

    if (!videoChannel || !readyPushing) {
        return;
    }
    jbyte *data = env->GetByteArrayElements(data_, NULL);
    videoChannel->encodeData(data);
    env->ReleaseByteArrayElements(data_, data, 0);

}
extern "C"
JNIEXPORT void JNICALL
Java_com_luban_publisher_LivePusher_native_1stop(JNIEnv *env, jobject thiz) {
    readyPushing = 0;
    pthread_join(pid, 0);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_luban_publisher_LivePusher_native_1release(JNIEnv *env, jobject thiz) {
    DELETE(videoChannel);
    DELETE(audioChannel);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_luban_publisher_LivePusher_native_1setAudioEncInfo(JNIEnv *env, jobject thiz,
                                                            jint sampleRateInHz,
                                                            jint channelConfig) {

    if (audioChannel) {
        audioChannel->setAudioEncInfo(sampleRateInHz,channelConfig);
    }

}
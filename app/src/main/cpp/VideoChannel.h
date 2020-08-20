//
// Created by liuyang on 2020/8/18.
//

#ifndef PUBLISHER_VIDEOCHANNEL_H
#define PUBLISHER_VIDEOCHANNEL_H

#include <inttypes.h>
#include <x264.h>
#include <pthread.h>
#include "librtmp/rtmp.h"

class VideoChannel {
    typedef void (*VideoCallback)(RTMPPacket* packet);

public:

    VideoChannel();

    ~VideoChannel();

    void setVideoEncInfo(int width, int height, int fps, int bitrate);
    void encodeData(int8_t *data);

    void setVideoCallback(VideoCallback callback);

private:
    //互斥锁
    pthread_mutex_t mutex;

    int mWidth;

    int mBitrate;

    int mFps;

    int mHeigth;
    x264_picture_t *pic_in = 0;
    int ySize;
    int uvSize;
    int index = 0;

    x264_t * videoCodec=0;
    VideoCallback callback;
    void sendSpsPps(uint8_t *sps, uint8_t *pps, int len, int pps_len);

    void sendFrame(int type, int payload, uint8_t *p_payload);
};


#endif //PUBLISHER_VIDEOCHANNEL_H

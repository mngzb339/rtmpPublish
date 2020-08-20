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

public:

    VideoChannel();

    ~VideoChannel();

    void setVideoEncInfo(int width, int height, int fps, int bitrate);


private:
    //互斥锁
    pthread_mutex_t mutex;

    int mWidth;

    int mBitrate;

    int mFps;

    int mHeigth;

    x264_t * videoCodec;
};


#endif //PUBLISHER_VIDEOCHANNEL_H

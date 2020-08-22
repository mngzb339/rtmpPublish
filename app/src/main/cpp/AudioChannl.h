//
// Created by liuyang on 2020/8/22.
//
#include "faac.h"
#include <sys/types.h>

#ifndef PUBLISHER_AUDIOCHANNL_H
#define PUBLISHER_AUDIOCHANNL_H


class AudioChannel {
    typedef void (*AudioCallback)(RTMPPacket *packet);


public:
    AudioChannel();

    ~AudioChannel();

    void setAudioEncInfo(int sampleRateInHz, int channelConfig);

    void setAudioCallback(AudioCallback callback);

    unsigned long  inputSamples;
    unsigned long  maxOutputBytes;

    faacEncHandle audioCodec=0;

    AudioCallback callback;

    int mChannel;



};


#endif //PUBLISHER_AUDIOCHANNL_H

//
// Created by liuyang on 2020/8/22.
//
#include "faac.h"
#include <sys/types.h>
#include "librtmp/rtmp.h"

#ifndef PUBLISHER_AUDIOCHANNL_H
#define PUBLISHER_AUDIOCHANNL_H


class AudioChannel {
    typedef void (*AudioCallback)(RTMPPacket *packet);
public:
    AudioChannel();

    ~AudioChannel();

    void setAudioEncInfo(int sampleRateInHz, int channelConfig);

    void setAudioCallback(AudioCallback callback);

    int getInputSample();

    void encodeData(int8_t *data);

    RTMPPacket *getAudioTag();

    unsigned long  inputSamples;
    unsigned long  maxOutputBytes;

    unsigned char*   buffer;
    faacEncHandle audioCodec=0;

    AudioCallback callback=0;

    int mChannel;

};


#endif //PUBLISHER_AUDIOCHANNL_H

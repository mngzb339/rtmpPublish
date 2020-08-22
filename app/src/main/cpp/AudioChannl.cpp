//
// Created by liuyang on 2020/8/22.
//

#include "AudioChannl.h"
#include "librtmp/rtmp.h"
#include "include/faac.h"
AudioChannel::AudioChannel() {

}

AudioChannel::~AudioChannel() {

}

void AudioChannel::setAudioCallback(AudioCallback callback) {
    this->callback = callback;
}

void AudioChannel::setAudioEncInfo(int sampleRateInHz, int channelConfig) {
    // 打开编码器
    mChannel = channelConfig;

    // imputSamples 输入给编码器的样本 maxOutputBytes:最多一次能接收多少个样本呢
    audioCodec =  faacEncOpen(sampleRateInHz,channelConfig,&inputSamples,&maxOutputBytes);
}


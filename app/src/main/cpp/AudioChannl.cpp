//
// Created by liuyang on 2020/8/22.
//

#include <cstring>
#include "AudioChannl.h"
#include "librtmp/rtmp.h"
#include "include/faac.h"
#include "macro.h"


AudioChannel::AudioChannel() {

}

AudioChannel::~AudioChannel() {
    DELETE(buffer);
    if (audioCodec) {
        faacEncClose(audioCodec);
        audioCodec = 0;
    }
}

void AudioChannel::setAudioCallback(AudioCallback callback) {
    this->callback = callback;
}

void AudioChannel::setAudioEncInfo(int sampleRateInHz, int channelConfig) {
    // 打开编码器
    mChannel = channelConfig;

    // imputSamples 输入给编码器的样本 maxOutputBytes:最多一次能接收多少个样本呢
    audioCodec = faacEncOpen(sampleRateInHz, channelConfig, &inputSamples, &maxOutputBytes);
    faacEncConfigurationPtr configurationPtr = faacEncGetCurrentConfiguration(audioCodec);
    // 指定mpegVersion版本
    configurationPtr->mpegVersion = MPEG4;
    // 指定lc 低复杂度规格
    configurationPtr->aacObjectType = LOW;
    // 指定16位
    configurationPtr->inputFormat = FAAC_INPUT_16BIT;
    // 编码初原始数据 既不是adts 也不是adif
    configurationPtr->outputFormat = 0;

    faacEncSetConfiguration(audioCodec, configurationPtr);

    buffer = new unsigned char[maxOutputBytes];
}

int AudioChannel::getInputSample() {
    return inputSamples;
}
RTMPPacket *AudioChannel::getAudioTag() {
    u_char *buf;
    u_long len;
    faacEncGetDecoderSpecificInfo(audioCodec, &buf, &len);
    int bodySize = 2 + len;
    RTMPPacket *packet = new RTMPPacket;
    RTMPPacket_Alloc(packet, bodySize);
    //双声道
    packet->m_body[0] = 0xAF;
    if (mChannel == 1) {
        packet->m_body[0] = 0xAE;
    }
    packet->m_body[1] = 0x00;
    //图片数据
    memcpy(&packet->m_body[2], buf, len);

    packet->m_hasAbsTimestamp = 0;
    packet->m_nBodySize = bodySize;
    packet->m_packetType = RTMP_PACKET_TYPE_AUDIO;
    packet->m_nChannel = 0x11;
    packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
    return packet;
}
void AudioChannel::encodeData(int8_t *data) {
    //返回编码后的字节长度
    int bytelen = faacEncEncode(audioCodec, reinterpret_cast<int32_t *>(data), inputSamples, buffer,
                                maxOutputBytes);
    if (bytelen > 0) {
        //看表
        int bodySize = 2 + bytelen;
        RTMPPacket *packet = new RTMPPacket;
        RTMPPacket_Alloc(packet, bodySize);
        //双声道
        packet->m_body[0] = 0xAF;
        if ( mChannel== 1) {
            packet->m_body[0] = 0xAE;
        }
        //编码出的声音 都是 0x01
        packet->m_body[1] = 0x01;
        //图片数据
        memcpy(&packet->m_body[2], buffer, bytelen);

        packet->m_hasAbsTimestamp = 0;
        packet->m_nBodySize = bodySize;
        packet->m_packetType = RTMP_PACKET_TYPE_AUDIO;
        packet->m_nChannel = 0x11;
        packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
        callback(packet);
    }
}



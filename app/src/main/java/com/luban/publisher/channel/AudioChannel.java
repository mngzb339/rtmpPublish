package com.luban.publisher.channel;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;

import com.luban.publisher.LivePusher;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import static java.util.concurrent.Executors.*;

/**
 * @author liuyang
 */
public class AudioChannel {
    private LivePusher mPusher;
    private AudioRecord audioRecord;
    private ExecutorService executorService;
    private int inputSamples;
    private int channel = 2;
    private boolean isLiving;

    public AudioChannel(LivePusher pusher) {
        mPusher = pusher;
        executorService = newSingleThreadExecutor();
        int channelConfig;
        /* 准备录音机 采集pcm 数据 */
        if (channel == 2) {
            /* 双声道 */
            channelConfig = AudioFormat.CHANNEL_IN_STEREO;
        } else {
            /* 单声道 */
            channelConfig = AudioFormat.CHANNEL_IN_MONO;
        }

        mPusher.native_setAudioEncInfo(44100, channel);
        //16 位 所以是2个字节
        inputSamples = mPusher.native_getInputSamples() * 2;
        /*给个大点的值 声音平滑度好点 */
        int minBufferSize = AudioRecord.getMinBufferSize(MediaRecorder.AudioSource.MIC, 44100, channelConfig) * 2;
        /* MediaRecorder.AudioSource.MIC 1 麦克风 2：44100 */
        audioRecord = new AudioRecord(MediaRecorder.AudioSource.MIC, 44100,
                channelConfig, AudioFormat.ENCODING_PCM_16BIT, minBufferSize > inputSamples ? minBufferSize : inputSamples);
    }

    public void stopLive() {
        isLiving = true;
    }

    public void startLive() {
        isLiving = true;
        executorService.submit(new AudioTask());
    }

    public void release() {
        audioRecord.release();
    }

    class AudioTask implements Runnable {

        @Override
        public void run() {
            // 启动录音机器
            byte[] bytes = new byte[inputSamples];
            audioRecord.startRecording();
            while (isLiving) {
                int len = audioRecord.read(bytes, 0, bytes.length);
                if (len > 0) {
                    mPusher.native_pushAudio(bytes);
                }
            }
            //停止录音机器
            audioRecord.stop();
        }
    }
}

package com.luban.publisher;

import androidx.appcompat.app.AppCompatActivity;

import android.hardware.Camera;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    private LivePusher livePusher;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        SurfaceView surfaceView = findViewById(R.id.surfaceView);

        livePusher = new LivePusher(this, 800, 480, 800_000, 10, Camera.CameraInfo.CAMERA_FACING_BACK);
        //  设置摄像头预览的界面

        livePusher.setPreviewDisplay(surfaceView.getHolder());
        findViewById(R.id.living).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Log.i("rtmp", "onClick: ");
                // fuck 虚拟机共享网络 坑了 半天 连不上
              livePusher.startLive("rtmp://192.168.1.103/myapp/mystream");

            }
        });
    }


    public void switchCamera(View view) {
        livePusher.switchCamera();
    }

    public void startLive(View view) {
    }

    public void stopLive(View view) {
        livePusher.stopLive();
    }
}

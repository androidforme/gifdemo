package com.wangduoyu.gifmaniudmeo;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    GifHandler gifHandler;
    Bitmap bitmap;
    ImageView imageView;

    @SuppressLint("HandlerLeak")
    Handler myHandler = new Handler() {
        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);
            int delay = gifHandler.updateFrame(bitmap);
            myHandler.sendEmptyMessageDelayed(1, delay);
            imageView.setImageBitmap(bitmap);
        }
    };

    int nextFrameRenderTime;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        imageView = findViewById(R.id.imageView);
        verifyStoragePermissions(this);
        findViewById(R.id.gifBtn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ndkLoadGif(v);
            }
        });
    }

    public   void verifyStoragePermissions(Activity activity) {
        int REQUEST_EXTERNAL_STORAGE = 1;
        String[] PERMISSIONS_STORAGE = {
                "android.permission.READ_EXTERNAL_STORAGE",
                "android.permission.WRITE_EXTERNAL_STORAGE" };
        try {
            //检测是否有写的权限
            int permission = ActivityCompat.checkSelfPermission(activity,
                    "android.permission.WRITE_EXTERNAL_STORAGE");
            if (permission != PackageManager.PERMISSION_GRANTED) {
                // 没有写的权限，去申请写的权限，会弹出对话框
                ActivityCompat.requestPermissions(activity, PERMISSIONS_STORAGE,REQUEST_EXTERNAL_STORAGE);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void ndkLoadGif(View view) {
        File file=new File("/sdcard/demo.gif");
        Log.i("pvj", "file = " + file.getAbsolutePath());
        gifHandler = GifHandler.load(file.getAbsolutePath());

//        int width = gifHandler.getWidth();
//        int height = gifHandler.getHeight();
//        Log.i("pvj", "width = " + width + "  ;height = " + height);
//
//        bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
//
//        int delay = gifHandler.updateFrame(bitmap);
//
//        myHandler.sendEmptyMessageDelayed(1, delay);

    }


}

package com.wangduoyu.gifmaniudmeo;

import android.graphics.Bitmap;

public class GifHandler {

    //mNative 一般都用于表示 地址（指针类型，大小都是一样的 8） 用long接收
    private long mNativeGifHandler;

    static {
        System.loadLibrary("native-lib");
    }

    private GifHandler(long nativeGifHandler) {
        this.mNativeGifHandler = nativeGifHandler;
    }

    public static GifHandler load(String path) {
        long nativeGifHandler = loadGif(path);
        GifHandler gifHanlder = new GifHandler(nativeGifHandler);
        return gifHanlder;
    }

    //宽 高
    public int getWidth() {
        return getWidth(mNativeGifHandler);
    }

    //宽 高
    public int getHeight() {
        return getHeight(mNativeGifHandler);
    }

    public int updateFrame(Bitmap bitmap) {
        return updateFrame(mNativeGifHandler, bitmap);
    }

    //开始加载文件
    public static native long loadGif(String path);


    //宽 高
    public static native int getWidth(long nativeGifHandler);

    //宽 高
    public static native int getHeight(long nativeGifHandler);

    //宽 高
    public static native int updateFrame(long nativeGifHandler, Bitmap bitmap);

}

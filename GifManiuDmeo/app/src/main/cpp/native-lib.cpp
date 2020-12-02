#include <jni.h>
#include <string>
#include <android/bitmap.h>

extern "C" {
#include "gif_lib.h"
}

struct GifBean {
    int current_frame;
    int total_frame;
    int *delays;
};


#define argb(a, r, g, b)(((a)&0xff)<<24)|(((b)&0xff)<<16)|(((g)&0xff)<<8)|((r)&0xff)

void drawFrame1(GifFileType *pType, AndroidBitmapInfo info, void *pVoid);

extern "C"
JNIEXPORT jlong JNICALL
Java_com_wangduoyu_gifmaniudmeo_GifHandler_loadGif(JNIEnv *env, jclass clazz, jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);
//    path   java   --
    int Erro;//打开失败还是成功
    GifFileType * gifFileType= DGifOpenFileName(path, &Erro);
//初始化缓冲区  数组 SaveImages
    DGifSlurp(gifFileType);
    GifBean *gifBean = static_cast<GifBean *>(malloc(sizeof(GifBean)));
    memset(gifBean, 0, sizeof(GifBean));
    gifFileType->UserData = gifBean;
    gifBean->current_frame = 0;
//    总帧数
    gifBean->total_frame = gifFileType->ImageCount;

    env->ReleaseStringUTFChars(path_, path);
    return (jlong)(gifFileType);
}

void drawFrame1(GifFileType *gifFileType, AndroidBitmapInfo info, void *pixels) {
    GifBean *gifBean = static_cast<GifBean *>(gifFileType->UserData);
    SavedImage savedImage = gifFileType->SavedImages[gifBean->current_frame];

    // 图像分两部分，一份是像素，一份是描述
    GifImageDesc frameInfo = savedImage.ImageDesc;
    ColorMapObject *colorMapObject = frameInfo.ColorMap;
    savedImage.RasterBits;

    // 记录每一行首地址；
    int *px = static_cast<int *>(pixels);
    // 临时索引
    int *line;
    int pointPixel;//索引
    GifByteType gifByteType;
    GifColorType color;
    for (int y = frameInfo.Top; y < frameInfo.Top + frameInfo.Height; ++y) {
        line = px;
        for (int x = frameInfo.Left; x < frameInfo.Left + frameInfo.Width; ++x) {
            //定位 像素
            pointPixel = (y - frameInfo.Top) * frameInfo.Width + (x - frameInfo.Left);

            // 这是压缩数据
            gifByteType = savedImage.RasterBits[pointPixel];
            color = colorMapObject->Colors[gifByteType];

            line[x] = argb(255, color.Red, color.Green, color.Blue);
        }

        // 转到下一行
        px = reinterpret_cast<int *>((char *) px + info.stride);
    }
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_wangduoyu_gifmaniudmeo_GifHandler_getWidth(JNIEnv *env, jclass clazz,
                                                    jlong native_gif_hander) {
    GifFileType *gifFileType = reinterpret_cast<GifFileType *>(native_gif_hander);
    return gifFileType->SWidth;
}extern "C"
JNIEXPORT jint JNICALL
Java_com_wangduoyu_gifmaniudmeo_GifHandler_getHeight(JNIEnv *env, jclass clazz,
                                                     jlong native_gif_hander) {
    GifFileType *gifFileType = reinterpret_cast<GifFileType *>(native_gif_hander);
    return gifFileType->SHeight;
}extern "C"
JNIEXPORT jint JNICALL
Java_com_wangduoyu_gifmaniudmeo_GifHandler_updateFrame(JNIEnv *env, jclass clazz,
                                                       jlong native_gif_hander, jobject bitmap) {

    GifFileType *gifFileType = reinterpret_cast<GifFileType *>(native_gif_hander);
    int width = gifFileType->SWidth;
    int height = gifFileType->SHeight;

    AndroidBitmapInfo info;
    AndroidBitmap_getInfo(env, bitmap, &info);

    width = info.width;
    height = info.height;

    void *pixels;
    //bitmap --> 像素二维数据   锁住当前bitmap
    AndroidBitmap_lockPixels(env, bitmap, &pixels);

    // 绘制

    drawFrame1(gifFileType, info, pixels);

    AndroidBitmap_unlockPixels(env, bitmap);

    GifBean *gifBean = static_cast<GifBean *>(gifFileType->UserData);

    gifBean->current_frame++;

    if (gifBean->current_frame >= gifBean->total_frame - 1) {
        gifBean->current_frame = 0;
    }

    return 100;
}

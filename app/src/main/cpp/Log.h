//
// Created by huste on 2023-04-19.
//

#ifndef MEDIAPLAYER_LOG_H
#define MEDIAPLAYER_LOG_H



class Log {

};
#ifndef ANDROID
#include <android/log.h>
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "MediaPlayer", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "MediaPlayer", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "MediaPlayer", __VA_ARGS__)
#else
#define LOGD(...) print("MediaPlayer", __VA_ARGS__)
#define LOGI(...) print("MediaPlayer", __VA_ARGS__)
#define LOGE(...) print("MediaPlayer", __VA_ARGS__)
#endif
#endif //MEDIAPLAYER_LOG_H

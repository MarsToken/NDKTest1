//
// Created by 赵恩莉 on 2025/9/14.
//

#ifndef MY_APPLICATION_BASE_H
#define MY_APPLICATION_BASE_H
#define LOG_TAG "MY_APPLICATION"

#include <android/log.h>
#include <jni.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#endif //MY_APPLICATION_BASE_H

//
// Created by 赵恩莉 on 2025/9/16.
//

#include "jni_thread_callback.h"
#include <jvm.h>
#include <jni.h>
#include <cwchar>
#include <string>

static jclass threadClazz;
static jmethodID threadMethod;
static jobject threadObject;

void *threadCallback(void *);
/**
 * 如何在Jni的子线程回调Java的方法
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplication_JNIThreadCallback_nativeThreadCallback(
        /**
         * env无法跨线程，需要借助JavaVM来切线程
         */
        JNIEnv *env,
        jobject thiz,
        jobject callback
) {
    threadObject = env->NewGlobalRef(callback);
    threadClazz = env->GetObjectClass(callback);
    threadMethod = env->GetMethodID(threadClazz, "callback", "()V");
    pthread_t thread;
    pthread_create(&thread, nullptr, threadCallback, nullptr);
}

void *threadCallback(void *) {
    JavaVM* javaVm = getJvm();
    JNIEnv *env;
    if (javaVm->AttachCurrentThread(&env, nullptr) == 0) {
        env->CallVoidMethod(threadObject, threadMethod);
        javaVm->DetachCurrentThread();
    }
    return 0;
}
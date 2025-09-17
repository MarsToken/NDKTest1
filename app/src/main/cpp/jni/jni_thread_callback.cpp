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
         * JNIEnv无法跨线程，需要借助JavaVM来切线程
         * 它用来c/c++调用java世界的api
         * JVM在内部将JNIEnv与线程进行了关联、每个Java线程都有其对应的JNIEnv实例
         * 避免多线程同时操作同一个JNIEnv导致的数据竞争和内存安全问题
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
    /**
     * JavaVM是Java虚拟机的接口指针，代表整个Java虚拟机实例
     * 整个进程只有一个JavaVM实例，代表整个Java虚拟机
     */
    JavaVM *javaVm = getJvm();
    // 每个Java线程都有其对应的JNIEnv实例
    JNIEnv *env;
    // 将当前子线程附加到Java虚拟机，获取该线程的JNIEnv环境
    if (javaVm->AttachCurrentThread(&env, nullptr) == 0) {
        // 在子线程中调用Java层的回调方法
        env->CallVoidMethod(threadObject, threadMethod);
        // 调用完成后将当前线程从Java虚拟机分离，避免线程资源泄露
        javaVm->DetachCurrentThread();
    }
    return 0;
}
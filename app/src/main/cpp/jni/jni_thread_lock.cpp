//
// Created by 赵恩莉 on 2025/9/18.
//

#include "jni_thread_lock.h"
#include "base.h"
#include <jni.h>
#include <pthread.h>

// 互斥锁，用于保护共享资源，确保同一时刻只有一个线程能访问临界区
pthread_mutex_t mutex;
// 条件变量，用于线程间同步，实现等待/通知机制
pthread_cond_t cond;
pthread_t waitHandle;
pthread_t notifyHandle;

void *waitThread(void *);

void *notifyThread(void *);

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplication_JNIThreadLock_waitNativeThread(JNIEnv *env, jobject thiz) {
    // 创建一个线程互斥锁 mutex,用于保护共享资源，确保同一时刻只有一个线程能访问临界区
    pthread_mutex_init(&mutex, nullptr);
    // 创建一个条件变量 cond,用于线程间同步，允许线程等待某个条件成立或通知其他等待线程
    pthread_cond_init(&cond, nullptr);
    // 这两个同步原语通常配合使用，实现线程间的等待/通知机制，用于处理多线程环境下的协调工作。
    pthread_create(&waitHandle, nullptr, waitThread, nullptr);
}
int flag = 0;

void *waitThread(void *) {
    LOGI("waitThread is lock");
    pthread_mutex_lock(&mutex);
    while (flag == 0) {
        LOGI("waitThread waiting");
        // 阻塞等待，此时会释放掉互斥锁mutex
        pthread_cond_wait(&cond, &mutex);
    }
    LOGI("waitThread is unlock");
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
    // 终止或return nullptr;否则会抛异常
}

void *notifyThread(void *) {
    LOGI("notifyThread is lock.");
    pthread_mutex_lock(&mutex);
    flag = 1;
    LOGI("flag set 1.");
    pthread_mutex_unlock(&mutex);
    // 唤起线程，注意避免唤醒丢失，必须保证pthread_cond_signal比pthread_cond_wait晚执行
    pthread_cond_signal(&cond);
    LOGI("notifyThread is unlocked!");
    pthread_exit(0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplication_JNIThreadLock_notifyNativeThread(JNIEnv *env, jobject thiz) {
    pthread_create(&notifyHandle, nullptr, notifyThread, nullptr);
}
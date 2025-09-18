//
// Created by 赵恩莉 on 2025/9/18.
//

#include "jni_thread.h"
#include "base.h"
#include <jni.h>
#include <cwchar>
#include <string>
#include <sys/prctl.h>
#include <unistd.h>

void *printThreadHello(void *);

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplication_JNIThread_createNativeThread(
        JNIEnv *env,
        jobject thiz
) {
    pthread_t thread;
    /**
     * 1.线程对象句柄
     * 2.线程信息，包括线程堆栈、调度优先级等
     * 3.要执行的代码块
     * 4.传递给线程的参数，供参数3使用
     */
    if (pthread_create(&thread, nullptr, printThreadHello, nullptr) == 0) {
        LOGI("create thread success");
    } else {
        LOGI("create thread failed");
    }
}

void *printThreadHello(void *) {
    LOGI("hello thread");
    // return nullptr;
    // 表示线程退出
    pthread_exit(0);
}

struct ThreadRunArgs {
    int id;
    int result;
};

void *printThreadArgs(void *);

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplication_JNIThread_createNativeThreadWithArgs(
        JNIEnv *env,
        jobject thiz
) {
    pthread_t thread;
    /**
    方法2. 动态内存分配
    ThreadRunArgs *args = new ThreadRunArgs;
    args->id = 2;
    args->result = 100;
    delete args;
     推荐使用方法2
    */
    ThreadRunArgs args;
    args.id = 2;
    args.result = 200;
    if (pthread_create(&thread, nullptr, printThreadArgs, &args) == 0) {
        LOGI("create thread success");
        // 方法1：主线程在此阻塞，等待子线程执行完毕，确保args的线程共享
        pthread_join(thread, nullptr);
        LOGI("thread has exited, main thread continues");
    } else {
        LOGI("create thread failed");
    }
}

void *printThreadArgs(void *args) {
    ThreadRunArgs *runArgs = static_cast<ThreadRunArgs *>(args);
    LOGI("hello thread args, id: %d, result: %d", runArgs->id, runArgs->result);
    return nullptr;
}


void *printJoinThread(void *);

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplication_JNIThread_joinNativeThread(
        JNIEnv *env,
        jobject thiz
) {
    pthread_t pthread;
    ThreadRunArgs *args = new ThreadRunArgs;
    args->id = 101;
    args->result = 1002;
    if (pthread_create(&pthread, nullptr, printJoinThread, args) == 0) {
        // 拿到程序执行代码块[printJoinThread]的返回值
        void *result = nullptr;
        pthread_join(pthread, &result);
        LOGI("result is %d", result);
    }
}

void *printJoinThread(void *args) {
    ThreadRunArgs *threadRunArgs = (ThreadRunArgs *) args;
    // 模拟耗时操作
    timeval start;
    gettimeofday(&start, nullptr);
    LOGI("start time is %lld", start.tv_sec);
    sleep(3);
    timeval end;
    gettimeofday(&end, nullptr);
    LOGI("end time is %lld", end.tv_sec);
    LOGI("thread cost time %d", end.tv_sec - start.tv_sec);
    return (reinterpret_cast<void *>(threadRunArgs->result));
}
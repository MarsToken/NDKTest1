//
// Created by 赵恩莉 on 2025/9/19.
//

#include "jni_product_and_consume.h"
#include "base.h"
#include <jni.h>
#include <cwchar>
#include <string>
#include <queue>
#include <unistd.h>

using namespace std;

pthread_mutex_t mutex_m;
pthread_cond_t cond_m;
queue<int> data;

void *product(void *);

void *consume(void *);

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplication_JNIThreadLock_productAndConsume(
        JNIEnv *env,
        jobject thiz
) {
    pthread_t product_thread,product_thread2, consume_thread;
    pthread_create(&product_thread, nullptr, product, nullptr);
    pthread_create(&product_thread2, nullptr, product, nullptr);
    pthread_create(&consume_thread, nullptr, consume, nullptr);
}

void *product(void *args) {
    while (data.size() < 10) {
        LOGI("will product!");
        pthread_mutex_lock(&mutex_m);
        data.push(0);
        LOGI("producted one,total is %d", data.size());
        pthread_cond_signal(&cond_m);
        pthread_mutex_unlock(&mutex_m);
        sleep(1);
    }
    return nullptr;
}

void *consume(void *args) {
    while (JNI_TRUE) {
        LOGI("will consume!");
        pthread_mutex_lock(&mutex_m);
        if (data.size() <= 0) {
            LOGI("waiting for consume!");
            pthread_cond_wait(&cond_m, &mutex_m);
        } else {
            data.pop();
            LOGI("consumed one!");
        }
        sleep(1);
        pthread_mutex_unlock(&mutex_m);
    }
    pthread_exit(0);
}
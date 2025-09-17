//
// Created by 赵恩莉 on 2025/9/17.
//

#include <unistd.h>
#include "jni_reference.h"
#include "../base/base.h"

/**
 * 局部引用
 *
 * 不用回收，当程序结束后内存会被释放掉
 * 局部引用不能超过16777216，尽量少开辟局部应用内存，特别注意循环，可以自己手动释放掉
 * 通过FindClass, NewObject, NewStringUTF等JNI函数自动创建，或使用NewLocalRef函数显式创建
 * jobject obj = env->NewLocalRef(cls); // 基于一个引用创建新的局部引用，此方式较少，因为默认都是局部引用
 */
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_myapplication_JNIReference_errorCacheLocalReference(JNIEnv *env, jobject thiz) {
    // 512个 (Android 8.0之前)。Android 8.0及之后版本大幅提升至约8388608个,特别注意循环，可以自己手动释放掉
    // 本地测试阈值为：16777216-8
    for (int i = 0; i < 16777216 - 9; i++) { // 如果内存不释放 16777216-8 是崩溃的阈值
        jclass cls = env->FindClass("java/lang/String");
//        jobject obj = env->NewLocalRef(cls); // 此方式较少，因为默认都是局部引用
//        env->DeleteLocalRef(obj);
//        env->DeleteLocalRef(cls);
    }
    jclass localClass = env->FindClass("java/lang/String");
    jmethodID localMethod = env->GetMethodID(localClass, "<init>", "(Ljava/lang/String;)V");
    jstring localString = env->NewStringUTF("test from c++");
    return static_cast<jstring>(env->NewObject(localClass, localMethod, localString));
}
/**
 * 全局引用
 */
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_myapplication_JNIReference_cacheWithGlobalReference(JNIEnv *env, jobject thiz) {
    // 静态存储方式：在程序运行期间数据存放在`静态存储区`。
    // 它们在程序整个运行期间都不释放，故生命周期存在于程序的整个运行过程。
    //`局部变量`，使用static修饰以后，则使用静态存储方式。
    // 必须用static修饰,且用NewGlobalRef创建，确保静态存储区不会被释放。
    static jclass stringClass = nullptr;
    if (stringClass == nullptr) {
        // 创建局部引用
        jclass cls = env->FindClass("java/lang/String");
        // 将局部引用转换为全局引用，注意：jclass是jobject的子类
        stringClass = static_cast<jclass>(env->NewGlobalRef(cls));
        // 这样会导致野指针【不是空指针】，局部引用
        // stringClass = static_cast<jclass>(env->NewLocalRef(cls));
        // 删除局部引用，即使不删除也不会内存泄露，处于良好习惯及局部引用数量限制
        env->DeleteLocalRef(cls);
        LOGI("create 全局引用！");
    } else {
        LOGI("use cache!");
    }
    jmethodID localMethod = env->GetMethodID(stringClass, "<init>", "(Ljava/lang/String;)V");
    jstring localString = env->NewStringUTF("test from c++");
    jstring returnString = static_cast<jstring>(env->NewObject(stringClass, localMethod,
                                                               localString));
    // 如果要释放：通过此api释放全局引用内存
//    env->DeleteGlobalRef(stringClass);
//    stringClass = nullptr;
    return returnString;
}
/*
 * 弱全局引用
 * 它允许本地代码（C/C++）持有对 Java 对象的引用，但不会阻止垃圾回收器（GC）回收该对象
 * 相当于java中的弱引用
 * 使用弱引用，一般会先提升为局部引用，用完再释放！
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplication_JNIReference_useWeakGlobalReference(JNIEnv *env, jobject thiz) {
    static jclass stringClass = nullptr;
    if (stringClass == nullptr) {
        jclass cls = env->FindClass("java/lang/String");
        stringClass = static_cast<jclass>(env->NewWeakGlobalRef(cls));
        env->DeleteLocalRef(cls);
        LOGI("create 弱引用！");
    } else {
        LOGI("use cache!");
        // 模拟GC - 调用System.gc()尝试回收弱引用
        jclass systemClass = env->FindClass("java/lang/System");
        if (systemClass != nullptr) {
            jmethodID gcMethod = env->GetStaticMethodID(systemClass, "gc", "()V");
            if (gcMethod != nullptr) {
                env->CallStaticVoidMethod(systemClass, gcMethod);
                LOGI("System.gc() called to simulate garbage collection");
            }
            env->DeleteLocalRef(systemClass);
        }
        // 给GC一些时间执行
        usleep(2000000); // 等待2000ms
        // 检查弱引用是否已被回收
        jboolean isGc = env->IsSameObject(stringClass, nullptr);
        if (isGc) {
            LOGI("弱引用已被回收!");
            // 重新创建弱引用
            jclass cls = env->FindClass("java/lang/String");
            stringClass = static_cast<jclass>(env->NewWeakGlobalRef(cls));
            env->DeleteLocalRef(cls);
            LOGI("重新创建弱引用");
        } else {
            LOGI("弱引用仍然有效");
        }
    }
    jobject localRef = env->NewLocalRef(stringClass);
    if (localRef == nullptr) {
        LOGI("stringClass 已被回收!");
    } else {
        // 没有被回收可以安全使用了
        LOGI("stringClass 未被回收，你可以使用它。");
    }
}
//
// Created by 赵恩莉 on 2025/9/14.
//

#include "jni_dynamic_load.h"
#include <jvm.h>
#include <jni.h>
#include <base.h>

#define JAVA_CLASS_NAME "com/example/myapplication/JNIDynamicLoad"

extern "C" JNIEXPORT jstring JNICALL getMessage(JNIEnv *env, jobject obj) {
    return env->NewStringUTF("this is msg");
}

jint plus(JNIEnv *env, jobject obj, int a, int b) {
    return a + b;
}

static JNINativeMethod gMethods[] = {
        // java native method,方法签名，c/c++层方法名
        {"getNativeString", "()Ljava/lang/String;", (void *) getMessage},
        {"sum",             "(II)I",                (void *) plus}
};

/**
 * 注册本地方法封装
 */
int registerNativeMethods(
        JNIEnv *env,
        const char *className,
        const JNINativeMethod *methods,
        jint nMethods) {
    jclass jcls;
    jcls = env->FindClass(className);
    if (jcls == NULL) {
        return JNI_FALSE;
    }
    // java类名，方法映射数组名，方法数
    if (env->RegisterNatives(jcls, methods, nMethods) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

/**
 * jni 动态加载入口
 *
 * 如果有多个.so库，每个库可以有各自的JNI_OnLoad：
 */
JNIEXPORT int JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGI("JNI_OnLoad!");
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_FALSE;
    }
    setJvm(vm);
    int length = sizeof(gMethods) / sizeof(gMethods[0]);
    registerNativeMethods(env, JAVA_CLASS_NAME, gMethods, length);
    return JNI_VERSION_1_6;
}
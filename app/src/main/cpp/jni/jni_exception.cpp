//
// Created by 赵恩莉 on 2025/9/17.
//

#include "jni_exception.h"
#include "../base/base.h"
/**
 * 由于Java代码问题，native调用java代码时java层抛异常,由native层对Java的潜在异常做容错处理
 * native层捕捉异常
 * env->ExceptionDescribe();
 * env->ExceptionClear();
 *
 * 案例：
 * native层调用Java的operate()方法时，会抛出java层的异常，此时native层打印方法调用栈，并防止崩溃
 */
extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_JNIException_nativeInvokeJavaException(
        JNIEnv *env,
        jobject thiz
) {
    jclass jcls = env->FindClass("com/example/myapplication/JNIException");
    jmethodID operateMethodID = env->GetMethodID(jcls, "operate", "()I");
    jmethodID constructorMethodID = env->GetMethodID(jcls, "<init>", "()V");
    jobject obj = env->NewObject(jcls, constructorMethodID);
    env->CallIntMethod(obj, operateMethodID);
    jthrowable exception = env->ExceptionOccurred();
    if (exception) {
        // 打印崩溃日志
        env->ExceptionDescribe();
        // 清除掉异常，防止程序崩溃
        env->ExceptionClear();
    }
}
/**
 * 由于native代码问题，native主动抛Java异常，由Java层处理异常
 * Java层捕捉异常
 * env->ThrowNew(jcls,"native throw IllegalArgumentException")
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_example_myapplication_JNIException_nativeThrowException(
        JNIEnv *env,
        jobject thiz
) {
    jclass jcls = env->FindClass("java/lang/IllegalArgumentException");
    env->ThrowNew(jcls, "native throw IllegalArgumentException");
}
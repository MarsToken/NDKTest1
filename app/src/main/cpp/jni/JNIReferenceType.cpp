//
// Created by 赵恩莉 on 2025/9/15.
//

#include <jni.h>
#include <cwchar>
#include <string>
#include "JNIReferenceType.h"
#include <base.h>

#define JAVA_CLASS_NAME "com/example/myapplication/JNIReferenceType"

/**
 * 字符串数组
 */
extern "C" JNIEXPORT jstring JNICALL
callStringArray(JNIEnv *env, jobject obj, jobjectArray array) {
    int length = env->GetArrayLength(array);
    LOGI("length:%d", length);
    // 获取首元素，int类型如：GetIntArrayElement
    jstring firstStr = static_cast<jstring>(env->GetObjectArrayElement(array, 0));
    const char *str = env->GetStringUTFChars(firstStr, JNI_FALSE);
    LOGI("firstStr:%s", str);
    env->ReleaseStringUTFChars(firstStr, str);
    return firstStr;
}
/**
 * 访问并修改普通字段
 */
extern "C" JNIEXPORT void JNICALL
accessInstanceField(JNIEnv *env, jobject obj, jobject jobj) {
    // 1.获取类
    jclass jcls = env->GetObjectClass(jobj);
    // 2.根据类获取字段
    jfieldID jfieldID = env->GetFieldID(jcls, "protectedName", "Ljava/lang/String;");
    // 3.修改字段
    env->SetObjectField(jobj, jfieldID, env->NewStringUTF("hello from c++"));
}
/**
 * 访问并修改静态字段
 */
extern "C" JNIEXPORT void JNICALL
accessStaticField(JNIEnv *env, jobject obj, jobject jobj) {
    jclass jcls = env->GetObjectClass(jobj);
    jfieldID jfieldID = env->GetStaticFieldID(jcls, "staticNum", "I");
    env->SetStaticIntField(jcls, jfieldID, 100);
}

/**
 * 静态方法直接修改类属性
 * C++支持重载，但是c不支持，所以不能去掉extern "C"
 * 注意参数 jclass jcls
 */
extern "C" JNIEXPORT void JNICALL
accessStaticField2(JNIEnv *env, jclass jcls) {
    jfieldID jfieldID = env->GetStaticFieldID(jcls, "staticNum", "I");
    env->SetStaticIntField(jcls, jfieldID, 1001);
}

/**
 * 对象的普通方法调用-形参
 */
extern "C" JNIEXPORT void JNICALL
accessInstanceMethod(JNIEnv *env, jobject obj, jobject jobj) {
    jclass jcls = env->GetObjectClass(jobj);
    jmethodID jmethodID = env->GetMethodID(jcls, "accessInstanceMethod", "(I)V");
    env->CallVoidMethod(jobj, jmethodID, 1001);
}
/**
 * 类的静态方法调用-形参
 */
extern "C" JNIEXPORT void JNICALL
accessStaticMethod(JNIEnv *env, jobject obj, jobject jobj) {
    jclass jcls = env->GetObjectClass(jobj);
    jmethodID jmethodID = env->GetStaticMethodID(jcls, "accessStaticMethod",
                                                 "([Ljava/lang/String;)Ljava/lang/String;");
    jsize size = 2;
    jobjectArray jstrArray = env->NewObjectArray(size, env->FindClass("java/lang/String"), NULL);
    for (int i = 0; i < size; i++) {
        char str[128];
        snprintf(str, 14, "hello world %d", i);
        jstring jstr = env->NewStringUTF(str);
        env->SetObjectArrayElement(jstrArray, i, jstr);
    }
    jstring result = static_cast<jstring>(env->CallStaticObjectMethod(jcls, jmethodID, jstrArray));
    LOGI("result:%s", env->GetStringUTFChars(result, JNI_FALSE));
}

/**
 * 接口回调-不切换线程
 */
extern "C" JNIEXPORT void JNICALL
callback(JNIEnv *env, jobject obj, jobject callback) {
    jclass jcls = env->GetObjectClass(callback);
    jmethodID jmethodID = env->GetMethodID(jcls, "callback", "()V");
    env->CallVoidMethod(callback, jmethodID);
}

/**
 * 创建Java对象1
 */
extern "C" JNIEXPORT jobject JNICALL
animalConstructor(JNIEnv *env, jobject obj) {
    jclass jcls = env->FindClass("com/example/myapplication/Animal");
    // 构造函数同一叫：<init>
    jmethodID jmethodId = env->GetMethodID(jcls, "<init>", "(Ljava/lang/String;)V");
    jstring name = env->NewStringUTF("Dog");
    jobject animal = env->NewObject(jcls, jmethodId, name);
    return animal;
}

/**
 * 创建Java对象2
 */
extern "C" JNIEXPORT jobject JNICALL
objectConstructor(JNIEnv *env, jobject obj) {
    jclass jcls = env->FindClass("com/example/myapplication/Animal");
    jmethodID jmethodId = env->GetMethodID(jcls, "<init>", "(Ljava/lang/String;)V");
    jobject animal = env->AllocObject(jcls);
    jstring name = env->NewStringUTF("Cat");
    env->CallNonvirtualVoidMethod(animal, jcls, jmethodId, name);
    return animal;
}

static JNINativeMethod gMethods[] = {
        // java native method,方法签名，c/c++层方法名
        {"callNativeStringArray",   "([Ljava/lang/String;)Ljava/lang/String;",             (void *) callStringArray},
        {"accessInstanceField",     "(Lcom/example/myapplication/Animal;)V",               (void *) accessInstanceField},
        {"accessStaticField",       "(Lcom/example/myapplication/Animal;)V",               (void *) accessStaticField},
        // 静态方法直接修改类属性
        {"accessStaticField",       "()V",                                                 (void *) accessStaticField2},
        {"accessInstanceMethod",    "(Lcom/example/myapplication/Animal;)V",               (void *) accessInstanceMethod},
        {"accessStaticMethod",      "(Lcom/example/myapplication/Animal;)V",               (void *) accessStaticMethod},
        {"nativeCallback",          "(Lcom/example/myapplication/interfaces/ICallback;)V", (void *) callback},
        {"invokeAnimalConstructor", "()Lcom/example/myapplication/Animal;",                (void *) animalConstructor},
        {"allocObjectConstructor",  "()Lcom/example/myapplication/Animal;",                (void *) objectConstructor},
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
    int length = sizeof(gMethods) / sizeof(gMethods[0]);
    registerNativeMethods(env, JAVA_CLASS_NAME, gMethods, length);
    return JNI_VERSION_1_6;
}
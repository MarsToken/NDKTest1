//
// Created by 赵恩莉 on 2025/9/15.
//

#include <jni.h>
#include <string.h>
#include <malloc.h>
#include "jni_basic_type.h"
#include "base.h"

#define JAVA_CLASS_NAME "com/example/myapplication/JNIBasicType"
extern "C" JNIEXPORT jint JNICALL callInt(JNIEnv *env, jobject obj, jint value) {
    LOGI("callInt %d", value);
    jint returnValue = value + 1;
    return returnValue;
}
extern "C" JNIEXPORT jbyte JNICALL callByte(JNIEnv *env, jobject obj, jbyte value) {
    LOGI("callByte %d", value);
    jbyte returnByte = value + 1;
    return returnByte;
}
extern "C" JNIEXPORT jchar JNICALL callChar(JNIEnv *env, jobject obj, jchar value) {
    LOGI("callChar %c", value);
    jchar returnChar = value + 1;
    return returnChar;
}
extern "C" JNIEXPORT jshort JNICALL callShort(JNIEnv *env, jobject obj, jshort value) {
    LOGI("callShort %d", value);
    jshort returnShort = value + 1;
    return returnShort;
}
extern "C" JNIEXPORT jlong JNICALL callLong(JNIEnv *env, jobject obj, jlong value) {
    LOGI("callLong %ld", value);
    jlong returnLong = value + 1;
    return returnLong;
}
extern "C" JNIEXPORT jfloat JNICALL callFloat(JNIEnv *env, jobject obj, jfloat value) {
    LOGI("callFloat %f", value);
    jfloat returnFloat = value + 1.0f;
    return returnFloat;
}
extern "C" JNIEXPORT jdouble JNICALL callDouble(JNIEnv *env, jobject obj, jdouble value) {
    LOGI("callDouble %f", value);
    jdouble returnDouble = value + 1.0;
    return returnDouble;
}
extern "C" JNIEXPORT jboolean JNICALL callBoolean(JNIEnv *env, jobject obj, jboolean value) {
    LOGI("callBoolean %d", value);
    jboolean returnBoolean = !value;
    return returnBoolean;
}
extern "C" JNIEXPORT jstring JNICALL callString(JNIEnv *env, jobject obj, jstring value) {
    // isCopy=true 表示：JVM 创建了 Java 字符串内容的一个副本
    //特点：
    //返回的 C 字符串是新分配的内存
    //可以安全地修改返回的字符串内容，不会影响原始 Java 字符串
    //需要额外的内存和复制开销，
    //isCopy=false 表示：不能修改返回的字符串内容，因为这会影响原始 Java 字符串
    //一般会传false
    bool isCopy = JNI_FALSE;
    const char *original_str = env->GetStringUTFChars(value, JNI_FALSE);
    if (isCopy) {
        if (original_str == NULL) {
            return env->NewStringUTF("内存分配失败！");
        }
        // 因为你拥有的是副本，修改它是安全的（但注意，这不会影响原始的Java字符串）
        // 例如，我们可以修改副本的第一个字符（仅用于演示，实际中根据需求操作）
        char *mutable_copy = strdup(original_str); // 如需真正修改，需先复制（因为指针是const的）
        mutable_copy[0] = 'X';
        LOGI("修改后的副本: %s", mutable_copy);
        // free(mutable_copy);
        return env->NewStringUTF(mutable_copy);

        // 当 isCopy 为 true 时，我们可以安全地处理副本
        // 1. 计算原字符串长度
//        size_t len = strlen(original_str);
//        // 2. 分配新的内存空间用于修改
//        char *modified_str = new char[len + 1];
//        // 3. 复制原字符串内容到新内存
//        strcpy(modified_str, original_str);
//        // 4. 修改字符串内容（例如转换为大写）
//        for (size_t i = 0; i < len; i++) {
//            if (modified_str[i] >= 'a' && modified_str[i] <= 'z') {
//                modified_str[i] = modified_str[i] - 'a' + 'A';
//            }
//        }
//        // 5. 释放原始字符串资源
//        env->ReleaseStringUTFChars(value, original_str);
//        // 6. 创建新的 Java 字符串并返回
//        jstring result = env->NewStringUTF(modified_str);
//        delete[] modified_str;
//        return result;
    } else {
        // 当 isCopy 为 false 时，不能修改字符串，只能读取
        LOGI("String is not copied, cannot modify");
        env->ReleaseStringUTFChars(value, original_str);
        return env->NewStringUTF("this is string from c++");
    }

}

extern "C" JNIEXPORT void JNICALL processString(JNIEnv *env, jobject obj, jstring value) {
    const char *str = env->GetStringUTFChars(value, JNI_FALSE);
    char buffer[128];
    int length = env->GetStringLength(value);
    LOGI("String length: %d", length);
    // 获取指定范围【部分】字符串
    env->GetStringUTFRegion(value, 0, length, buffer);
    LOGI("String: %s", buffer);
    env->ReleaseStringUTFChars(value, str);
}

/**
 * 1.动态注册Java数据类型与描述符对照表

    Java类型	描述符
    boolean	Z
    byte	B
    char	C
    short	S
    int	    I
    long	J
    float	F
    double	D
    void	V
    String[] [Ljava/lang/String;
    引用类型	以 L 开头 ; 结尾，中间是 / 分隔的包名和类名。例如 String 的字段描述符为 Ljava/lang/String;
    String  Ljava/lang/String;

    2.Java/Jni/C++ 数据类型对照表

    Java类型	 JNI类型	   描述           长度（字节）
    boolean	 jboolean	unsigned char	1
    byte	 jbyte	    signed char	    1
    char	 jchar	    unsigned short	2
    short	 jshort	    signed short	2
    int	     jint、jsize signed int	    4
    long	 jlong	     signed long	8
    float	 jfloat	     signed float	4
    double	 jdouble	 signed double	8
    Class	 jclass	     Class类对象	    1
    String	 jstrting	  字符串对象	    /
    Object	 jobject	   对象	        /
    Object[] jobjectArray对象数组[包括String] /
    Throwable jthrowable	异常对象	    /
    boolean[] jbooleanArray  布尔数组	/
    byte[]	 jbyteArray	    byte 数组	/
    char[]	 jcharArray	    char 数组	/
    short[]	 jshortArray	short数组	/
    int[]	 jinitArray	    int 数组	    /
    long[]	 jlongArray	    long 数组	/
    float[]	 jfloatArray	float 数组	/
    double[] jdoubleArray	double 数组	/

 */
static JNINativeMethod gMethods[] = {
        // java native method,方法签名，c/c++层方法名
        {"callNativeInt",       "(I)I",                                   (void *) callInt},
        {"callNativeByte",      "(B)B",                                   (void *) callByte},
        {"callNativeChar",      "(C)C",                                   (void *) callChar},
        {"callNativeShort",     "(S)S",                                   (void *) callShort},
        {"callNativeLong",      "(J)J",                                   (void *) callLong},
        {"callNativeFloat",     "(F)F",                                   (void *) callFloat},
        {"callNativeDouble",    "(D)D",                                   (void *) callDouble},
        {"callNativeBoolean",   "(Z)Z",                                   (void *) callBoolean},
        {"callNativeString",    "(Ljava/lang/String;)Ljava/lang/String;", (void *) callString},
        {"processNativeString", "(Ljava/lang/String;)V",                   (void *) processString},
};

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

JNIEXPORT int JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGI("JNI_OnLoad basic type!");
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_FALSE;
    }
    int length = sizeof(gMethods) / sizeof(gMethods[0]);
    registerNativeMethods(env, JAVA_CLASS_NAME, gMethods, length);
    return JNI_VERSION_1_6;
}
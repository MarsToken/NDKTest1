package com.example.myapplication;

/**
 * Created by WangMaoBo.
 * Date: 2025/9/17
 */
public class JNIException {
    private static final String TAG = "JNIException";

    static {
        System.loadLibrary("exception-lib");
    }

    private int operate() {
        return 2 / 0;
    }

    /**
     * 由于Java代码问题，native调用java代码时java层抛异常,由native层对Java的潜在异常做容错处理
     * env->ExceptionDescribe();
     * env->ExceptionClear();
     * 案例：
     * native层调用nativeInvokeJavaException时，间接调用Java的operate()方法时，会抛出java层的异常，此时native层打印方法调用栈，并防止崩溃
     * 引用链:MainActivity#onClick[vBtn17]
     * ->JNIException#nativeInvokeJavaException
     * ->jni_exception#nativeInvokeJavaException
     * ->JNIException#operate[抛异常]
     * ->native层env->ExceptionDescribe()、env->ExceptionClear()打印方法调用栈并避免崩溃
     */
    public native void nativeInvokeJavaException();

    /**
     * 由于native代码问题，native主动抛Java异常，由Java层处理异常
     * env->ThrowNew(jcls,"native throw IllegalArgumentException")
     * 引用链:MainActivity#onClick[vBtn18]
     * ->JNIException#nativeThrowException
     * ->jni_exception#nativeThrowException
     * ->env->ThrowNew[抛IllegalArgumentException异常]
     * ->java层try catch 处理异常
     */
    public native void nativeThrowException() throws IllegalArgumentException;
}

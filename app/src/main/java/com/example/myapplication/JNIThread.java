package com.example.myapplication;

/**
 * 线程技术方案：
 * 1.c++11的thread库
 * 2.Posix API线程库【基于Linux】
 * <p>
 * Created by WangMaoBo.
 * Date: 2025/9/18
 */
public class JNIThread {
    private static final String TAG = "JNIThread";

    static {
        System.loadLibrary("thread-lib");
    }

    public native void createNativeThread();

    public native void createNativeThreadWithArgs();

    public native void joinNativeThread();
}

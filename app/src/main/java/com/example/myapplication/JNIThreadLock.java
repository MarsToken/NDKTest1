package com.example.myapplication;

/**
 * Created by WangMaoBo.
 * Date: 2025/9/18
 */
public class JNIThreadLock {
    private static final String TAG = "JNIThreadLock";

    static {
        System.loadLibrary("thread-lock-lib");
    }

    /**
     * 创建一个线程，并等待,充当消费者
     */
    public native void waitNativeThread();

    /**
     * 创建一个线程，动态设置标记，充当生产者
     */
    public native void notifyNativeThread();
}

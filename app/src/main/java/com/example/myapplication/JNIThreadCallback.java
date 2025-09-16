package com.example.myapplication;

import com.example.myapplication.interfaces.IThreadCallback;

/**
 * Created by WangMaoBo.
 * Date: 2025/9/16
 */
public class JNIThreadCallback {
    private static final String TAG = "JNIThreadCallback";

    static {
        System.loadLibrary("threadCallback-lib");
    }

    /**
     * 线程回调-切换线程
     */
    public native void nativeThreadCallback(IThreadCallback callback);
}

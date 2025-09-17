package com.example.myapplication;

/**
 * Created by WangMaoBo.
 * Date: 2025/9/17
 */
public class JNIReference {
    private static final String TAG = "JNIReference";

    static {
        System.loadLibrary("reference-lib");
    }

    /**
     * 局部引用
     */
    public native String errorCacheLocalReference();

    /**
     * 全引用
     */
    public native String cacheWithGlobalReference();

    /**
     * 弱引用
     */
    public native void useWeakGlobalReference();
}

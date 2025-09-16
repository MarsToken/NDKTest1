package com.example.myapplication;

/**
 * Created by WangMaoBo.
 * Date: 2025/9/14
 */
public class JNIDynamicLoad {
    private static final String TAG = "JNIDynamicLoad";
    static {
        System.loadLibrary("dynamic-lib");
    }
    public static native String getNativeString();

    public native int sum(int x, int y);
}

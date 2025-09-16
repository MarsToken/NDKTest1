package com.example.myapplication;

/**
 * Created by WangMaoBo.
 * Date: 2025/9/15
 */
public class JNIBasicType {
    static {
        System.loadLibrary("baseType-lib");
    }

    public native int callNativeInt(int num);

    public native byte callNativeByte(byte b);

    public native char callNativeChar(char ch);

    public native short callNativeShort(short num);

    public native long callNativeLong(long num);

    public native float callNativeFloat(float num);

    public native double callNativeDouble(double num);

    public native boolean callNativeBoolean(boolean flag);

    public native String callNativeString(String str);

    public native void processNativeString(String str);

}

package com.example.myapplication;

import com.example.myapplication.interfaces.ICallback;
import com.example.myapplication.interfaces.IThreadCallback;

/**
 * Created by WangMaoBo.
 * Date: 2025/9/15
 */
public class JNIReferenceType {
    private static final String TAG = "JNIReferenceType";
    private static int staticNum;

    public static int getStaticNum() {
        return staticNum;
    }

    static {
        System.loadLibrary("referenceType-lib");
    }

    /**
     * 访问String[]数组
     */
    public native String callNativeStringArray(String[] str);

    /**
     * 访问Animal的实例字段name
     */
    public native void accessInstanceField(Animal animal);

    /**
     * 修改Animal的静态字段staticNum
     */
    public native void accessStaticField(Animal animal);

    /**
     * 注意与 非静态的accessStaticField 的区别
     * 静态方法直接修改Animal的静态字段staticNum
     */
    public static native void accessStaticField();

    /**
     * 访问Animal参数的实例方法
     */
    public native void accessInstanceMethod(Animal animal);

    /**
     * 访问Animal参数的静态方法
     */
    public native void accessStaticMethod(Animal animal);

    /**
     * 接口回调-不切换线程
     *
     * @param callback
     */
    public native void nativeCallback(ICallback callback);

    /**
     * JNI调用java层构造函数1
     */
    public native Animal invokeAnimalConstructor();

    /**
     * JNI调用java层构造函数2
     */
    public native Animal allocObjectConstructor();
}

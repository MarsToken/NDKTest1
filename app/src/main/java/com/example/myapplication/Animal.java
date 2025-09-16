package com.example.myapplication;

import android.util.Log;

/**
 * Created by WangMaoBo.
 * Date: 2025/9/15
 */
public class Animal {
    private static final String TAG = "Animal";
    protected String protectedName;
    public static int staticNum;

    public String getProtectedName() {
        return protectedName;
    }

    public void setProtectedName(String protectedName) {
        this.protectedName = protectedName;
    }

    /**
     * 访问实例方法
     */
    public void accessInstanceMethod(int num) {
        Log.e("tag", "访问实例方法,参数：" + num);
    }

    /**
     * 访问静态方法
     */
    public static String accessStaticMethod(String[] array) {
        for (int i = 0; i < array.length; i++) {
            Log.e("tag", "array[" + i + "]:" + array[i]);
        }
        return "访问静态方法";
    }
}

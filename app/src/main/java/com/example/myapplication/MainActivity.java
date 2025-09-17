package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.nfc.Tag;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.example.myapplication.databinding.ActivityMainBinding;
import com.example.myapplication.interfaces.ICallback;
import com.example.myapplication.interfaces.IThreadCallback;

/**
 * 1.char *modified_str = new char[len + 1]; 为什么要+1
 * 2.函数 void*
 * 3.static作用:被其修饰的成员无法被其他文件引用及访问
 */
public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";

    // Used to load the 'myapplication' library on application startup.
    static {
        System.loadLibrary("myapplication-lib");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        binding.vBtn1.setOnClickListener(v -> {
            binding.vBtn1.setText(stringFromJNI());
        });
        JNIDynamicLoad jniDynamicLoad = new JNIDynamicLoad();
        binding.vBtn2.setOnClickListener(v -> {
            binding.vBtn2.setText(jniDynamicLoad.getNativeString());
        });
        binding.vBtn3.setOnClickListener(v -> {
            binding.vBtn3.setText("sum is" + jniDynamicLoad.sum(1, 2));
        });
        JNIBasicType jniBasicType = new JNIBasicType();
        binding.vBtn4.setOnClickListener(v -> {
            byte b = jniBasicType.callNativeByte((byte) 1);
            Log.e(TAG, "byte is " + b);
            boolean isFlag = jniBasicType.callNativeBoolean(true);
            Log.e(TAG, "boolean is " + isFlag);
            char ch = jniBasicType.callNativeChar('a');
            Log.e(TAG, "char is " + ch);
            double d = jniBasicType.callNativeDouble(2.0);
            Log.e(TAG, "double is " + d);
            float f = jniBasicType.callNativeFloat(1.0f);
            Log.e(TAG, "float is " + f);
            int i = jniBasicType.callNativeInt(100);
            Log.e(TAG, "int is " + i);
            long l = jniBasicType.callNativeLong(1000L);
            Log.e(TAG, "long is " + l);
            short s = jniBasicType.callNativeShort((short) 1);
            Log.e(TAG, "short is " + s);
            String originalString = "hello from java!";
            String str = jniBasicType.callNativeString(originalString);
            Log.e(TAG, "String is " + str);
            // 原始java字符串是常量，不会被c所修改
            Log.e(TAG, "original string is " + originalString);
            jniBasicType.processNativeString("hello world!");
        });
        JNIReferenceType jniReferenceType = new JNIReferenceType();
        binding.vBtn5.setOnClickListener(v -> {
            String result = jniReferenceType.callNativeStringArray(new String[]{"hello", "world"});
            Log.e(TAG, "result is " + result);
        });
        binding.vBtn6.setOnClickListener(v -> {
            Animal animal = new Animal();
            jniReferenceType.accessInstanceField(animal);
            jniReferenceType.accessStaticField(animal);
            Log.e(TAG, "protectedName is " + animal.protectedName);
            Log.e(TAG, "Animal.staticNum is " + Animal.staticNum);
        });
        binding.vBtn7.setOnClickListener(v -> {
            JNIReferenceType.accessStaticField();
            Log.e(TAG, "JNIReferenceType.staticNum is " + JNIReferenceType.getStaticNum());
        });
        binding.vBtn8.setOnClickListener(v -> {
            Animal animal = new Animal();
            jniReferenceType.accessInstanceMethod(animal);
        });
        binding.vBtn9.setOnClickListener(v -> {
            Animal animal = new Animal();
            jniReferenceType.accessStaticMethod(animal);
        });
        binding.vBtn10.setOnClickListener(v -> {
            jniReferenceType.nativeCallback(() -> Log.e(TAG, Thread.currentThread().getName() + ":callback is called."));
        });
        JNIThreadCallback jniThreadCallback = new JNIThreadCallback();
        binding.vBtn11.setOnClickListener(v -> {
            jniThreadCallback.nativeThreadCallback(() -> Log.e(TAG, Thread.currentThread().getName() + ":thread callback is called."));
        });
        binding.vBtn12.setOnClickListener(v -> {
            Animal animal = jniReferenceType.invokeAnimalConstructor();
            Log.e(TAG, "1:animal name is " + animal.getProtectedName());
        });
        binding.vBtn13.setOnClickListener(v -> {
            Animal animal = jniReferenceType.allocObjectConstructor();
            Log.e(TAG, "2-:animal name is " + animal.getProtectedName());
        });

    }

    /**
     * 静态注册，查找，耗时，java和jni类全名查找匹配
     *
     * @return
     */
    public native String stringFromJNI();
}
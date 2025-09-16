#include <jni.h>
#include <string>
// #include "people/People.h"
// CMakeList.txt include_directories(people/) 就可以<People.h>调用了。
#include <People.h>
/**
 * extern "C" :
 * 为什么需要它：C++支持函数重载，编译器会通过“名称修饰”将参数类型等信息编码到最终函数名中
 * （例如 func(int)可能变成 _Z4funci）。而C语言不支持重载，函数名保持不变。
 * JVM期望按照C语言的简单命名规则来查找函数
 *
 * JNIEXPORT:
 * 这是一个宏，在Windows等平台上通常扩展为 __declspec(dllexport)，
 * 用于显式地将函数标记为从动态链接库导出，这样外部程序（JVM）才能看到并调用它。
 * 在Linux/Android平台上，这个宏有时可能定义为空，但为了跨平台兼容性，始终加上它是好习惯
 *
 * JNICALL:
 * 这也是一个宏，用于指定函数的调用约定。调用约定规定了参数入栈顺序、栈维护责任（是调用者还是被调用者清理栈）等底层细节。
 * 不同平台和编译器可能有不同的默认约定，JNICALL确保了JNI函数使用JVM所期望的正确约定，避免了栈操作错误导致的崩溃
 */
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_myapplication_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    People people;
    std::string hello = people.getString();
    return env->NewStringUTF(hello.c_str());
}
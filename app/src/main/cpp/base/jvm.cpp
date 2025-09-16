// 飘红可能是CMakeList.txt没有引入此文件导致add_library{base/jvm.cpp}
// Created by 赵恩莉 on 2025/9/16.
//
#include "jvm.h"

/**
 * 为创建线程服务！
 */
static JavaVM *gVM = nullptr;
extern "C" {
/**
 * 动态库加载时调用
 * @param vm
 */
void setJvm(JavaVM *vm) {
    gVM = vm;
}
JavaVM *getJvm() {
    return gVM;
}
}
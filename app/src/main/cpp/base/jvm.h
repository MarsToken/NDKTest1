//
// Created by 赵恩莉 on 2025/9/16.
//

#ifndef MY_APPLICATION_JVM_H
#define MY_APPLICATION_JVM_H

#include <base.h>

#ifdef __cplusplus
extern "C" {
#endif
// 或者直接这样写：extern "C" void setJvm(JavaVM *jvm);它是兼容C的
void setJvm(JavaVM *jvm);

JavaVM *getJvm();

#ifdef __cplusplus
}
#endif

#endif //MY_APPLICATION_JVM_H

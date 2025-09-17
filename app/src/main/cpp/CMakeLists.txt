cmake_minimum_required(VERSION 3.22.1)

# Declares and names the project.
# 这个命令会设置一些有用的变量，例如 PROJECT_NAME和 PROJECT_SOURCE_DIR
project("myapplication")
# 将native-lib.cpp编译为静态库，名字叫myapplication-lib.so
add_library( # Sets the name of the library.
        myapplication-lib
        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        native-lib.cpp)
# 将people/People.cpp编译为静态库，名字叫people-lib.so
add_library(
        people-lib
        SHARED
        people/People.cpp
)

#模拟动态注册
add_library(
        dynamic-lib
        SHARED
        jni/jni_dynamic_load.cpp
        #        jni_invoke_method.cpp
        #        jni_thread.cpp
        base/jvm.cpp
)

add_library(
        baseType-lib
        SHARED
        jni/jni_basic_type.cpp
)

add_library(
        referenceType-lib
        SHARED
        jni/JNIReferenceType.cpp
)

add_library(
        threadCallback-lib
        SHARED
        jni/jni_thread_callback.cpp
)

add_library(
        reference-lib
        SHARED
        jni/jni_reference.cpp
)

add_library(
        exception-lib
        SHARED
        jni/jni_exception.cpp
)

# 将people文件夹引入到项目中，native-lib.cpp可通过#include<> 直接查找 people目录下所有源码文件
include_directories(people/)
# 调用时 #include<base.h>即可
include_directories(${CMAKE_SOURCE_DIR}/base/)
# 查找android系统的库
# find_library(<VAR> name1 [path1 path2 ...] [options])
# name1: 要查找的库的名称，不包含前缀（如 lib）和后缀（如 .so、.a）。例如，要查找 libmylib.so，只需写 mylib
find_library(
        # 找到android系统的默认库loglib.so，并命名为：变量名log-lib
        log-lib
        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
#将log-lib连接到myapplication-lib中，这样myapplication-lib就可以使用log-lib了。
target_link_libraries(
        # Specifies the target library.
        myapplication-lib
        # Links the target library to the log library
        # included in the NDK.
        people-lib
        # find_library中的log-lib变量
        ${log-lib})
# 将android的log库链接至dynamic-lib库中
target_link_libraries(
        # Specifies the target library.
        dynamic-lib
        # find_library中的log-lib变量
        ${log-lib})
target_link_libraries(
        # Specifies the target library.
        baseType-lib
        # find_library中的log-lib变量
        ${log-lib})
target_link_libraries(
        # Specifies the target library.
        referenceType-lib
        # find_library中的log-lib变量
        ${log-lib})
target_link_libraries(
        # Specifies the target library.
        threadCallback-lib
        # find_library中的log-lib变量
        ${log-lib}
        # 用到了JavaVM相关的类！jvm.cpp及动态链接库dynamic-lib的入口初始化
        dynamic-lib)
target_link_libraries(
        # Specifies the target library.
        reference-lib
        # find_library中的log-lib变量
        ${log-lib})

target_link_libraries(
        # Specifies the target library.
        exception-lib
        # find_library中的log-lib变量
        ${log-lib})

# 指定生成的库文件输出路径
set_target_properties(myapplication-lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/${ANDROID_ABI})
set_target_properties(people-lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/${ANDROID_ABI})
set_target_properties(dynamic-lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/${ANDROID_ABI})
set_target_properties(baseType-lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/${ANDROID_ABI})
set_target_properties(referenceType-lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/${ANDROID_ABI})
set_target_properties(threadCallback-lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/${ANDROID_ABI})
set_target_properties(reference-lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/${ANDROID_ABI})
set_target_properties(exception-lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/${ANDROID_ABI})
set(CMAKE_CXX_STANDARD 14)
# 打印日志 内置变量
message("1:" +${PROJECT_SOURCE_DIR})
message(${ANDROID_ABI})
# 给变量var赋值hello
set(var hello)
# 打印变量值
message(${var})
# 打so包：externalNativeBuildDebug
# 流程控制语句
if (TRUE)
    message("test")
endif ()
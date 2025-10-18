cmake_minimum_required(VERSION 3.22.1)
project("dominigames_testassignment")

add_library(${CMAKE_PROJECT_NAME} SHARED
        native-lib.cpp
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue
)

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log
        EGL
        GLESv2
)

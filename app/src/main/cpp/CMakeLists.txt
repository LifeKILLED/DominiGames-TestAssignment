cmake_minimum_required(VERSION 3.22.1)
project("dominigames_testassignment")

add_library(${CMAKE_PROJECT_NAME} SHARED
        src/native-lib.cpp
        src/Renderer/Renderer.cpp
        src/Renderer/GLES/ContextGLES.cpp
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        src
        ${ANDROID_NDK}/sources/android/native_app_glue
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE DCONTEXT_GLES)

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log
        EGL
        GLESv2
)

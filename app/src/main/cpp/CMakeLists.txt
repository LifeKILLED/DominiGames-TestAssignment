cmake_minimum_required(VERSION 3.22.1)
project("dominigames_testassignment")

include(FetchContent)

FetchContent_Declare(
        glm
        GIT_REPOSITORY	https://github.com/g-truc/glm.git
        GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

add_library(${CMAKE_PROJECT_NAME} SHARED
        src/native-lib.cpp
        src/Renderer/Renderer.cpp
        src/Renderer/GLES/ContextGLES.cpp
        src/Renderer/GLES/Mesh.cpp
        src/Renderer/GLES/Shader.cpp
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        src
        ${ANDROID_NDK}/sources/android/native_app_glue
)

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log
        EGL
        GLESv2
        glm::glm
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE DCONTEXT_GLES)

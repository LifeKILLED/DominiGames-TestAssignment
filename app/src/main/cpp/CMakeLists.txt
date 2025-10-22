cmake_minimum_required(VERSION 3.22.1)
project("dominigames_testassignment")

include(FetchContent)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863 # v1.0.1
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
        inih
        GIT_REPOSITORY https://github.com/benhoyt/inih.git
        GIT_TAG r58
)
FetchContent_Populate(inih)

add_library(inih STATIC
        ${inih_SOURCE_DIR}/ini.c
        ${inih_SOURCE_DIR}/cpp/INIReader.cpp
)

target_include_directories(inih PUBLIC
        ${inih_SOURCE_DIR}
        ${inih_SOURCE_DIR}/cpp
)

add_library(inih::INIReader ALIAS inih)

add_library(${CMAKE_PROJECT_NAME} SHARED
        src/native-lib.cpp
        src/Renderer/MeshRenderer.cpp
        src/Renderer/Renderer.cpp
        src/Renderer/GLES/ContextGLES.cpp
        src/Renderer/GLES/Mesh.cpp
        src/Renderer/GLES/Shader.cpp
        src/Scene/Scene.cpp
        src/Scene/Entity.cpp
        src/Scene/Transform.cpp
        src/Content/Loader.cpp
        src/Input/Input.cpp
        src/Util/IniUtils.cpp
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
        inih::INIReader
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE DCONTEXT_GLES)

cmake_minimum_required(VERSION 3.0)
project(OpenGLParse)


#third dependence
add_subdirectory(./external)


file(GLOB cppResource "src/*.cpp" "src/framework/*.cpp" "src/featureLab/*.cpp")
set(cppHeader "./external/glfw/include" "./external/glew/include" "./src")
add_executable(OpenGLSandbox ${cppResource})
target_include_directories(OpenGLSandbox PRIVATE ${cppHeader})
target_link_libraries(OpenGLSandbox opengl32.lib glfw glew_s)

if (WIN32)
    set(TARGET_DIR ${CMAKE_SOURCE_DIR}/resource)
    set(LINK_NAME ${CMAKE_BINARY_DIR}/resource)
    if(NOT IS_DIRECTORY ${TARGET_DIR})
        add_custom_command(TARGET OpenGLSandbox POST_BUILD
            COMMAND cmd.exe /c "mklink /J \"${LINK_NAME}\" \"${TARGET_DIR}\""
        )
    endif()
endif()
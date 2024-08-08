cmake_minimum_required(VERSION 3.0)
project(OpenGLParse)


#third dependence
add_subdirectory(./external)


file(GLOB cppResource src/*.cpp)
add_executable(OpenGLSandbox ${cppResource})
target_include_directories(OpenGLSandbox PRIVATE ./external/glfw/include ./external/glew/include)
target_link_libraries(OpenGLSandbox opengl32.lib glfw glew_s)


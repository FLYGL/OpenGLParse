cmake_minimum_required(VERSION 3.0)

#set cache test

#glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
add_subdirectory(glfw)

#glew
set(BUILD_UTILS OFF)
add_subdirectory(glew/build/cmake)
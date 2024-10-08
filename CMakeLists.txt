cmake_minimum_required(VERSION 3.0)
project(OpenGLParse)


#third dependence
add_subdirectory(./external)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE cppResource "src/*.cpp")
set(cppHeader 
    "./src"
    "./external/glfw/include" 
    "./external/glew/include" 
    "./external/glm" 
)
add_executable(OpenGLSandbox ${cppResource})
target_compile_definitions(OpenGLSandbox PRIVATE GLFW_INCLUDE_NONE)
target_include_directories(OpenGLSandbox PRIVATE ${cppHeader})
target_link_libraries(OpenGLSandbox glfw glew_s glm ${OPENGL_LIBRARY})

if (WIN32)
    set(TARGET_DIR ${CMAKE_SOURCE_DIR}/resource)
    set(LINK_NAME ${CMAKE_BINARY_DIR}/resource)
    set(BAT_CONTENT
        "@echo off\n"
        "if not exist \"${LINK_NAME}\" (\n"
        "  mklink /J \"${LINK_NAME}\" \"${TARGET_DIR}\"\n"
        ")\n"
    )
    set(BAT_FILE_PATH ${CMAKE_BINARY_DIR}/linkResource.bat)
    file(WRITE ${BAT_FILE_PATH} ${BAT_CONTENT})
    add_custom_command(TARGET OpenGLSandbox POST_BUILD
        COMMAND cmd /c ${BAT_FILE_PATH}
    )
endif()
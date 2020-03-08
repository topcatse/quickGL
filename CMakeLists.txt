cmake_minimum_required(VERSION 3.16.5)
project(QuickGL VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Dependencies

find_package(GLEW 2.1.0 REQUIRED)
if(GLEW_FOUND)
    message ("GLEW found")
else()
    message (FATAL_ERROR "Cannot find GLEW")
endif()

find_package(glfw3 3.3 REQUIRED)
if(glfw3_FOUND)
    message ("glfw3 found")
else()
    message (FATAL_ERROR "Cannot find glfw3")
endif()

find_package(glm 0.9.9 REQUIRED)
if(glm_FOUND)
    message ("glm found")
else()
    message (FATAL_ERROR "Cannot find glm")
endif()

find_package(OpenGL REQUIRED)
if(OpenGL_FOUND)
    message ("OpenGL found")
else()
    message (FATAL_ERROR "Cannot find OpenGL")
endif()

# Library

add_library(quickgl STATIC 
    src/camera_controller.cpp
    src/camera.cpp
    src/camera2d.cpp
    src/glinit.cpp
    src/palette.cpp
    src/shape.cpp
    src/tool_base.cpp
)

target_compile_options(quickgl 
    PRIVATE -fPIC -Wall -Wno-unused-variable
)

target_include_directories(quickgl
    PUBLIC
        /usr/local/include
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    # PRIVATE
    #     ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Executable

add_executable(quick 
    tests/test.cpp
)

target_compile_options(quickgl PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -fPIC -Wall -Wno-unused-variable>
)

# target_link_options(quick PRIVATE
#     $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
#         -framework Cocoa>  
#     )

target_link_libraries(quick
    PRIVATE 
        quickgl
    PUBLIC
        GLEW::glew glfw glm 
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
)


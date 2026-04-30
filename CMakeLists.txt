cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME F103_LED_Architecture)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME}
        Hardware/Inc/bsp_uart.h
        Hardware/Src/bsp_uart.c
        Service/Inc/service_cli.h
        Service/Src/service_cli.c
        Hardware/Inc/bsp_delay.h
        Hardware/Src/bsp_delay.c)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)
# ==============================================================================
# 自定义用户代码路径配置 (User Code Directories)
# ==============================================================================

# 1. 包含所有自定义模块的 Inc 头文件目录
include_directories(
        Application/Inc
        Hardware/Inc
        Service/Inc
)

# 2. 递归查找所有自定义模块的 Src 目录下的 .c 文件
file(GLOB_RECURSE USER_SOURCES
        "Hardware/Src/*.c"
        "Service/Src/*.c"
        "Application/Src/*.c"
)

# Add sources to executable 将扫描到的所有用户源码文件添加到最终的编译目标中
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    # 将刚刚扫描到的所有 .c 文件打包塞进工程
    ${USER_SOURCES}
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    # ===== [架构师新增：暴露业务层头文件] =====
    Hardware/Inc
    Service/Inc
    Application/Inc
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)

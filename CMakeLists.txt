cmake_minimum_required(VERSION 2.6)

project(SendAnywhereSDK)

set(CMAKE_VERBOSE_MAKEFILE on)
#SET(CMAKE_C_FLAGS "-g")

if(WIN32)
    if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
        link_directories(${CMAKE_SOURCE_DIR}/SDK/lib/windows/x64)
    else()
        link_directories(${CMAKE_SOURCE_DIR}/SDK/lib/windows/win32)
    endif()
elseif(APPLE)
    link_directories(${CMAKE_SOURCE_DIR}/SDK/lib/osx)
elseif(UNIX)
    EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/SDK/lib/linux/${ARCHITECTURE})
endif()

add_subdirectory(SDK)
add_subdirectory(example)


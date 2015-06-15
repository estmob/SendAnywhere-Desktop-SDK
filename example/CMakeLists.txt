cmake_minimum_required(VERSION 2.6)

project(example)

include_directories(../SDK/include)

IF(WIN32)
    link_directories(${PROJECT_SOURCE_DIR}/../SDK/lib/win32)
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
        link_directories(${PROJECT_SOURCE_DIR}/../SDK/lib/linux/x64)
    ELSE( CMAKE_SIZEOF_VOID_P EQUAL 8 )
        link_directories(${PROJECT_SOURCE_DIR}/../SDK/lib/linux/x86)
    ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
ELSE(WIN32)
endif(WIN32)

add_executable(sample_send sample_send.c)
target_link_libraries(sample_send SendAnywhereSDK)

add_executable(sample_receive sample_receive.c)
target_link_libraries(sample_receive SendAnywhereSDK)

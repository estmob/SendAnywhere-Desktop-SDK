cmake_minimum_required(VERSION 2.6)

project(example)

include_directories(../SDK/include)

add_executable(sample_send sample_send.c)
target_link_libraries(sample_send SendAnywhereSDK)

add_executable(sample_receive sample_receive.c)
target_link_libraries(sample_receive SendAnywhereSDK)

cmake_minimum_required(VERSION 2.6)

project(SendAnywhereSDK)

include_directories(include)

add_library(SendAnywhereSDK src/sendanywhere.c)

target_link_libraries(SendAnywhereSDK sendanywhere)

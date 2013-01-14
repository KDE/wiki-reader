cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

find_package(Qt4 4.8 REQUIRED)

if(BUILD_WIKIREADER_BLACKBERRY)
    add_subdirectory(blackberry)
endif()

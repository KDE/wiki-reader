cmake_minimum_required(VERSION 2.8.10)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(BBCascades REQUIRED)

include_directories(
    ${QT_INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BBCASCADES_INCLUDE_DIR}
)

set(WikiReader_SRCS
    main.cpp
    wikimodel.cpp
)

QT4_ADD_RESOURCES(WikiReader_RCC_SRCS
    wikireader.qrc
)

add_executable(wikireader ${WikiReader_SRCS} ${WikiReader_RCC_SRCS})

target_link_libraries(
    wikireader

    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTDECLARATIVE_LIBRARY}

    ${BBCASCADES_LIBRARY}
)

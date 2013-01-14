project(wikireader)
cmake_minimum_required(VERSION 2.8.10)

find_package(Qt4 4.8 REQUIRED)

include_directories(
    ${QT_INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${QNX_TARGET}/usr/include
)

set(WikiReader_SRCS
    main.cpp
    wikimodel.cpp
)

qt4_automoc(${WikiReader_SRCS})

add_executable(wikireader ${WikiReader_SRCS})

target_link_libraries(
    wikireader

    ${QT_QTCORE_LIBRARY}
    ${QT_QTNETWORK_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTDECLARATIVE_LIBRARY}

    -lbbcascades
)

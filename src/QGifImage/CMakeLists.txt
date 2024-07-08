cmake_minimum_required(VERSION 3.15)
project(QGifImage LANGUAGES C CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
add_definitions(-DQT_BUILD_GIFIMAGE_LIB)

add_library(${PROJECT_NAME} SHARED
    GifLib/dgif_lib.c
    GifLib/egif_lib.c
    GifLib/gif_err.c
    GifLib/gif_hash.c
    GifLib/gifalloc.c
    GifLib/quantize.c
    GifLib/gif_hash.h
    GifLib/gif_lib_private.h
    GifLib/gif_lib.h

    QGifImage/qgifglobal.h
    QGifImage/qgifimage_p.h
    QGifImage/qgifimage.cpp
    QGifImage/qgifimage.h
)

target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
)

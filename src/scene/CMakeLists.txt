cmake_minimum_required(VERSION 3.15)
project(SceneWidget LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets OpenGL OpenGLWidgets)
add_definitions(-DQT_BUILD_GIFIMAGE_LIB)

add_library(${PROJECT_NAME} STATIC
    scene.h
    scene.cc

    scene_builder.h
    scene_builder.cc
)

target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    Qt6::Core
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
)

cmake_minimum_required(VERSION 3.15)
project(ViewerTests LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-DGTEST_LANGUAGE_CXX17)

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)
find_package(GTest REQUIRED)

set(PROJECT_SOURCES
        main.cc 
        mesh_parser_tests.cc
        scene_tests.cc
        
        ../controller/controller.h
        ../controller/controller.cc
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ViewerCore
    gtest

    Qt6::Widgets
    Qt6::Core
)

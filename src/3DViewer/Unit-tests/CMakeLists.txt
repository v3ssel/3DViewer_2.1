cmake_minimum_required(VERSION 3.5)

project(Tests LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-DGTEST_LANGUAGE_CXX17)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)
find_package(Threads REQUIRED)

set(PROJECT_SOURCES
        main.cc 
        tst_parsing.cc
        ../Parsing/singleton.cc
        ../Parsing/singleton.h
        ../Controller/controller.h
        ../Controller/controller.cc
)

include_directories("C:\\msys64\\mingw64\\include\\") # CHANGE THE PATH
link_directories("C:\\msys64\\mingw64\\lib\\") # CHANGE THE PATH

if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
        set(CMAKE_CXX_FLAGS "--coverage")
endif()

add_executable(Tests ${PROJECT_SOURCES})
add_test(NAME Tests COMMAND Tests)
target_link_libraries(Tests PRIVATE Threads::Threads gtest Qt${QT_VERSION_MAJOR}::Widgets
                                                           Qt${QT_VERSION_MAJOR}::Core)

cmake_minimum_required(VERSION 3.5)

project(Parsing LANGUAGES CXX)

enable_testing()


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-DGTEST_LANGUAGE_CXX17)

find_package(Threads REQUIRED)

include_directories("C:\\msys64\\mingw64\\include\\") # CHANGE THE PATH
link_directories("C:\\msys64\\mingw64\\lib\\") # CHANGE THE PATH
add_executable(Parsing main.cpp tst_parsing.cpp)
add_test(NAME Parsing COMMAND Parsing)
target_link_libraries(Parsing PRIVATE Threads::Threads gtest)


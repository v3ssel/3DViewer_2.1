cmake_minimum_required(VERSION 3.15)
project(3DViewer_2_0 LANGUAGES CXX)

# option(COVERAGE OFF)
# option(TESTING OFF)

# add_compile_options(-Wall -Werror -Wextra)

# if (TESTING)
#     enable_testing()
# endif()

# if (COVERAGE)
#     add_compile_options(-g -O0 -fprofile-arcs -ftest-coverage)
#     add_link_options(--coverage)
# endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(gui)
add_subdirectory(QGifImage)

cmake_minimum_required(VERSION 3.15)
project(3DViewer VERSION 2.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(OpenGL REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

set(PROJECT_SOURCES
    main.cc
    Resources.qrc
    
    viewer.h
    viewer.cc
    viewer.ui
    
    ../controller/controller.h
    ../controller/controller.cc
)

qt_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
    MACOSX_BUNDLE

    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    ViewerCore
    QGifImage

    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE

    WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(${PROJECT_NAME})

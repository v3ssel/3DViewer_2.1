cmake_minimum_required(VERSION 3.15)
project(3DViewer VERSION 2.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(OpenGL REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets Core Gui)

set(MACOSX_BUNDLE_ICON_FILE 3d-icon.icns)
set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/3d-icon.icns")
set_source_files_properties(${app_icon_macos} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

set(PROJECT_SOURCES
    main.cc
    ${app_icon_macos}
    Resources.qrc
    
    Parsing/singleton.cc
    Parsing/singleton.h
    
    Controller/controller.h
    Controller/controller.cc
    
    GLWidget/scene.h
    GLWidget/scene.cc

    Viewer/viewer.h
    Viewer/viewer.cc
    Viewer/viewer.ui
)

qt_add_executable(${PROJECT_NAME}
    MANUAL_FINALIZATION
    MACOSX_BUNDLE

    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    QGifImage
    ${OPENGL_LIBRARIES}

    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::OpenGL
    Qt${QT_VERSION_MAJOR}::OpenGLWidgets
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE

    WIN32_EXECUTABLE TRUE
)

qt_finalize_executable(${PROJECT_NAME})

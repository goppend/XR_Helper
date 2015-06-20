cmake_minimum_required(VERSION 3.0)
project(XR_helper)

if(WIN32)
    if(NOT QT5_PATH)
        message(FATAL_ERROR "Please set QT5_PATH to the path of the relevant Qt 5 package (e.g. C:/Qt/5.4/msvc2013)")
    endif()
    
    set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${QT5_PATH}")
endif()

add_subdirectory(libsquish)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/libsquish)

add_executable(XR_helper WIN32
    CEntryLTX.cpp
    CFileDDS.cpp
    CFileLTX.cpp
    main.cpp
    MainWindow.cpp
    VArrowLineEdit.cpp
    VLogger.cpp
)
target_link_libraries(XR_helper Qt5::Core Qt5::Widgets Qt5::Gui squish)

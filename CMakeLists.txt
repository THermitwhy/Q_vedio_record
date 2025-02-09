cmake_minimum_required(VERSION 3.5)

project(q_vedio_rec VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)
qt_add_resources(RESOURCES_RCC ${CMAKE_SOURCE_DIR}/icon.qrc)
#set(TS_FILES vedio_rec_zh_CN.ts)

set(PROJECT_SOURCES
        vedio_meta.h
        vedio_meta.cpp
        global_config.h
        global_config.cpp
        encodeAudio.h
        utils.hpp
        threadPool.hpp
        bvedio.h
        bvedio.cpp
        dx_capture.hpp
        baudio.h
        vedio_record.hpp
        main.cpp
        main_dia.cpp
        main_diat.h
        main_dia.ui
        rec_setting.cpp
        rec_setting.h
        rec_setting.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${RESOURCES_RCC}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET vedio_rec APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(q_vedio_rec SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(q_vedio_rec
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
include_directories("E:/soft/ffmpeg-master-latest-win64-gpl-shared/include")#
include_directories("D:/lib/libyuv/include")
target_link_directories(${PROJECT_NAME} PUBLIC "E:/soft/ffmpeg-master-latest-win64-gpl-shared/lib")
target_link_directories(${PROJECT_NAME} PUBLIC "D:/lib/libyuv/Debug")
target_link_libraries(${PROJECT_NAME} PUBLIC "avcodec.lib" "avdevice.lib" "swscale.lib" "avformat.lib" "avutil.lib" "yuv.lib")
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()

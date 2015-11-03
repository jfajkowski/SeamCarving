cmake_minimum_required(VERSION 2.8.12)
set (seam_carving_VERSION_MAJOR 1)
set (seam_carving_VERSION_MINOR 0)

set(CMAKE_PREFIX_PATH "/home/fajqa/Apps/Qt/5.5/gcc_64")
set(PROJECT "seam_carving")

project(${PROJECT})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

find_package(Qt5Widgets REQUIRED)
if(NOT Qt5Widgets_FOUND)
    message(ERROR " Qt5Widgets not found!")
endif(NOT Qt5Widgets_FOUND)

find_package(OpenCV REQUIRED)
if(NOT OpenCV_FOUND)
    message(ERROR " OpenCV not found!")
endif(NOT OpenCV_FOUND)

set(SOURCE_FILES
    src/main.cpp
    src/energy_demo.h
    src/energy_demo.cpp
    src/gui.h
    src/gui.cpp
    src/image.h
    src/image.cpp
)

set(UI_FILES
    ui/gui.ui
)
source_group("UI Files" FILES ${UI_FILES})

qt5_wrap_ui(UI_HEADERS ${UI_FILES})
source_group("Generated UI Headers" FILES ${UI_HEADERS})
 
SET(LIBRARIES ${LIBRARIES} ${OPENGL_LIBRARIES})
 
INCLUDE_DIRECTORIES(${QT_QTOPENGL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} )

set(CMAKE_CXX_FLAGS "-march=native -std=c++11")

add_executable(${PROJECT}
    ${SOURCE_FILES}
    ${UI_HEADERS}
    ${UI_FILES}
)

target_link_libraries(${PROJECT} ${OpenCV_LIBS})
qt5_use_modules(${PROJECT} Widgets)

cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.4.1\\5.4\\msvc2013\\")
set(OpenCV_DIR "C:\\opencv\\build\\x86\\vc12\\lib\\")
set(PROJECT "seam_carving")

project(${PROJECT})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)
find_package(OpenCV REQUIRED)

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


add_executable(${PROJECT} Win32
    ${SOURCE_FILES}
    ${UI_HEADERS}
    ${UI_FILES}
)

target_link_libraries(${PROJECT} ${OpenCV_LIBS})
qt5_use_modules(${PROJECT} Widgets)

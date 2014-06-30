# Define project
cmake_minimum_required(VERSION 2.6)
project(cg1blatt6)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Limited Configs" FORCE)

# Include helpers
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(${CMAKE_SOURCE_DIR}/cmake/common.cmake)

# Set target name
set(target exercise6)

# Include QT5 libraries
set(CMAKE_AUTOMOC ON)
set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)

find_package(OpenGL REQUIRED)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED) # this is important in order to get uic workin

# Define source files
set(sources

    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mathmacros.h
    src/shaderutils.h
    src/shaderutils.cpp
    src/abstractexercise.cpp
    src/abstractexercise.h
    src/abstractGLexercise.cpp
    src/abstractGLexercise.h
    src/exercise20.cpp
    src/exercise20.h
    src/exercise21.cpp
    src/exercise21.h
    src/objio.cpp
    src/objio.h
    src/polygonaldrawable.cpp
    src/polygonaldrawable.h
    src/halfedgestructure.cpp
    src/halfedgestructure.h
    src/vertexreuse.cpp
    src/vertexreuse.h
)

# Create executable
add_executable(${target} ${sources})

# Create executable
include_directories(
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(${target}
    ${OPENGL_LIBRARIES}
)

qt5_use_modules(${target} OpenGL Gui Widgets)

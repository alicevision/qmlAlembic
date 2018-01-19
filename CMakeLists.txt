cmake_minimum_required(VERSION 2.6.2)
project(alembicEntityQmlPlugin)

# Cmake module path
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Qt dependency
if(POLICY CMP0043)
    cmake_policy(SET CMP0043 OLD)
endif()
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
add_definitions(-DQT_NO_KEYWORDS)

# Alembic dependency
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-register")
find_package(Alembic REQUIRED)
include_directories(${ABC_INCLUDE_DIR})

add_subdirectory(src)

cmake_minimum_required(VERSION 3.17)

project(
  libmueb
  VERSION 4.0
  DESCRIPTION "Schönherz Mátrix network library written in C++ using Qt"
  LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_WEBSOCKET FALSE)
option(ENABLE_TESTS FALSE)

find_package(Qt5)

add_subdirectory(src)
if(ENABLE_WEBSOCKET)
  add_subdirectory(websocket)
endif()
if(ENABLE_TESTS)
  add_subdirectory(tests)
endif()

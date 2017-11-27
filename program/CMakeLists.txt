cmake_minimum_required(VERSION 3.8)
project(untitled)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake-build-debug)

find_package(SFML 2 REQUIRED system window graphics network audio)

include_directories(${SFML_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp Application.cpp QuadBoard.cpp Random.cpp Config.h FindWay.cpp)

add_executable(untitled ${SOURCE_FILES})
target_link_libraries(untitled ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

#set(CMAKE_CXX_STANDARD 11)

#set(SOURCE_FILES main.cpp)
#add_executable(untitled ${SOURCE_FILES})
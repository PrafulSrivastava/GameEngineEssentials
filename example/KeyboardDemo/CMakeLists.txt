cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_BUILD_TYPE Debug)
project(KeyboardDemo)
set(TARGET ${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH "/usr/share/SFML/cmake/Modules/" ${CMAKE_MODULE_PATH})

find_package(SFML 2
  COMPONENTS 
    system window graphics network audio REQUIRED)

set(LIBS pthread sfml-graphics sfml-audio sfml-window sfml-system)

include_directories(
  ${CMAKE_SOURCE_DIR}/*
  ) 
file(GLOB SOURCES *.cpp)

add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} ${LIBS})

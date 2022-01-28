cmake_minimum_required(VERSION 3.16)
project(factorio)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        src/main.cpp
        src/item.cpp
        src/recipe.cpp
        src/factory.cpp
        src/technology.cpp
        src/event.cpp)

set(TARGET FactorioSim)

add_executable(${TARGET} ${SOURCES})

target_include_directories(${TARGET} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
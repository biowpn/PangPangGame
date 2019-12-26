
cmake_minimum_required(VERSION 3.1)
project(PangPangGame)

set(CMAKE_CXX_STANDARD 11)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

add_executable(main
    ${PROJECT_SOURCE_DIR}/src/main.cc
    ${PROJECT_SOURCE_DIR}/src/game.cc
)

add_library(HumanStrat SHARED
    ${PROJECT_SOURCE_DIR}/strategies/HumanStrat.cc
    ${PROJECT_SOURCE_DIR}/src/game.cc
)

add_library(MachineStrat0 SHARED
    ${PROJECT_SOURCE_DIR}/strategies/MachineStrat0.cc
)

add_library(MachineStrat1 SHARED
    ${PROJECT_SOURCE_DIR}/strategies/MachineStrat1.cc
)

cmake_minimum_required(VERSION 3.5.0)

project(ChessEngineCPP VERSION 0.1.0 LANGUAGES CXX)

include(CTest)
enable_testing()

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(ChessEngineCPP ${SOURCES})

set_property(TARGET ChessEngineCPP PROPERTY CXX_STANDARD 17)

target_include_directories(ChessEngineCPP PRIVATE include)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(ChessEngineCPP sfml-graphics sfml-window sfml-system)
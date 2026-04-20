cmake_minimum_required(VERSION 3.5.0)

project(ChessEngineCPP VERSION 0.1.0 LANGUAGES CXX)

include(CTest)
enable_testing()

add_executable(ChessEngineCPP main.cpp board.cpp pieces.cpp debug.cpp)

set_property(TARGET ChessEngineCPP PROPERTY CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
target_link_libraries(ChessEngineCPP sfml-graphics sfml-window sfml-system)
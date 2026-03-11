cmake_minimum_required(VERSION 3.20)
project(SwwwSetter CXX)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(swww-setter src/main.cpp)

target_include_directories(swww-setter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(swww-setter PRIVATE argumentsea)

install(TARGETS swww-setter RUNTIME DESTINATION /usr/bin)
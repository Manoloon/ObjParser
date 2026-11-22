cmake_minimum_required(VERSION 3.25.0)
project(Parser VERSION 0.1.0 LANGUAGES C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_executable(Parser main.c)
add_compile_options(-std=c11 -Wall -Wextra -Wpedantic -Wshadow)
target_include_directories(Parser PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/External/include)
target_link_libraries(Parser PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/External/lib/libUtilityDS.a)


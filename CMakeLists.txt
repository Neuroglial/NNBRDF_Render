cmake_minimum_required(VERSION 3.30)

project(CMake_Learning CXX)

include_directories(include)
aux_source_directory(. SRC_CUR)
file(GLOB_RECURSE SRC_SUB "src/**/*.cpp")
add_executable(main ${SRC_CUR} ${SRC_SUB})

find_package(glad CONFIG REQUIRED)
target_link_libraries(main PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE glfw)

include_directories($ENV{VCPKG_ROOT}/installed/x64-mingw-dynamic/include)
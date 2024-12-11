cmake_minimum_required(VERSION 3.30)

project(NNBRDF_ENDER CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
aux_source_directory(. SRC_CUR)
file(GLOB_RECURSE SRC_SUB "src/**/*.cpp")
file(GLOB_RECURSE SRC_SUR "source/**/*.cpp")
add_executable(main ${SRC_CUR} ${SRC_SUB} ${SRC_SUR})

find_package(glad CONFIG REQUIRED)
target_link_libraries(main PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE glfw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(main PRIVATE imgui::imgui)

include_directories($ENV{VCPKG_ROOT}/installed/x64-windows/include)
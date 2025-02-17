cmake_minimum_required(VERSION 3.30)

include($ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

project(NNBRDF_ENDER CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)
include_directories(resource)
include_directories(src/library)
aux_source_directory(. SRC_CUR)
file(GLOB_RECURSE SRC_SUB "src/**/*.cpp")
file(GLOB_RECURSE SRC_SUR "resource/**/*.cpp")
add_executable(main ${SRC_CUR} ${SRC_SUB} ${SRC_SUR})

find_package(glad CONFIG REQUIRED)
target_link_libraries(main PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE glfw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(main PRIVATE imgui::imgui)

find_package(assimp CONFIG REQUIRED)
target_link_libraries(main PRIVATE assimp::assimp)

find_package(EnTT CONFIG REQUIRED)
target_link_libraries(main PRIVATE EnTT::EnTT)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)

include_directories($ENV{VCPKG_ROOT}/installed/x64-windows/include)
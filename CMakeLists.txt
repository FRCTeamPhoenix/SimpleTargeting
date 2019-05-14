# to build run cmake ../ -G "NMake Makefiles" -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Release
# or on linux cmake ../ -DCMAKE_BUILD_TYPE=Release

cmake_minimum_required (VERSION 2.8)

project(findtarget)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(cv_findtarget findtarget.cpp)

target_link_libraries(cv_findtarget ${OpenCV_LIBS})
# CMakeList.txt : CMake project for bmp_program, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (bmp_tool "main.c" "main.h" "bitmap.c" "bitmap.h")

# TODO: Add tests and install targets if needed.

cmake_minimum_required(VERSION 3.23)
project(edge_detection)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_COMPILER  mpicc)
set(CMAKE_CXX_COMPILER  mpicxx)

find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

add_compile_definitions(STB_IMAGE_IMPLEMENTATION STB_IMAGE_WRITE_IMPLEMENTATION)
add_link_options(-lm)
include_directories(src/include)

add_executable(
        edge_detection
        src/main.cpp
        src/include/image.h
        src/include/kernels.h
        src/include/sobel.h
        src/include/stb_image.h
        src/include/stb_image_write.h
        src/include/threadArgs.h
)
target_link_libraries(edge_detection ${MPI_C_LIBRARIES})

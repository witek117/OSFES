cmake_minimum_required(VERSION 3.1)
set(NAME OSfES)
project(${NAME} CXX C )

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -static -Wno-unused-variable -O3 -Wno-missing-field-initializers")
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -Wextra -static -Wno-unused-variable -O3")

add_subdirectory(CImg)
add_subdirectory(osapi)
add_subdirectory(Render)
add_subdirectory(cxxopts)

add_executable(OSFES main.cpp)
target_link_libraries(OSFES PUBLIC render osapi cxxopts)


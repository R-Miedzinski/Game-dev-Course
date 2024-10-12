cmake_minimum_required(VERSION 3.22)

project(Project VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
configure_file(config.h.in config.h)

find_package(SFML 2.5.1
  COMPONENTS 
    system window graphics network audio REQUIRED
  )

add_executable(Project.out src/main.cpp)

target_include_directories(Project.out PUBLIC
                            "${PROJECT_BINARY_DIR}"
                            )

target_link_libraries(Project.out PRIVATE sfml-graphics)
target_compile_features(Project.out PRIVATE cxx_std_17)
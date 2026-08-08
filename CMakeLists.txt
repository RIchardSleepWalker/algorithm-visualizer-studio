cmake_minimum_required(VERSION 3.24)

project(AlgorithmVisualizerStudio
    VERSION 0.1.0
    DESCRIPTION "Desktop C++ application for interactive algorithm visualization"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

add_subdirectory(core)
add_subdirectory(visualization)
add_subdirectory(ui)
add_subdirectory(app)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

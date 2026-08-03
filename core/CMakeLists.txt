add_library(avs_core
    src/Version.cpp
    src/algorithm/AlgorithmStep.cpp
    src/algorithm/Timeline.cpp
    src/algorithm/AlgorithmRunner.cpp
    src/algorithm/sorting/BubbleSortStepper.cpp
    src/execution/ExecutionController.cpp
)

target_include_directories(avs_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(avs_core
    PUBLIC
        cxx_std_20
)

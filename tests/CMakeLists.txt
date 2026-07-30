include(FetchContent)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(avs_core_tests
    core/VersionTests.cpp
    core/AlgorithmStepTests.cpp
    core/TimelineTests.cpp
    core/AlgorithmStepperTests.cpp
    core/BubbleSortStepperTests.cpp
    core/AlgorithmRunnerTests.cpp
)

target_link_libraries(avs_core_tests
    PRIVATE
        avs_core
        GTest::gtest_main
)

target_compile_features(avs_core_tests
    PRIVATE
        cxx_std_20
)

include(GoogleTest)

gtest_discover_tests(avs_core_tests)

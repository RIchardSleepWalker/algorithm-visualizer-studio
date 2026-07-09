add_library(avs_visualization
    src/VisualizationModule.cpp
)

target_include_directories(avs_visualization
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(avs_visualization
    PUBLIC
        avs_core
)

target_compile_features(avs_visualization
    PUBLIC
        cxx_std_20
)

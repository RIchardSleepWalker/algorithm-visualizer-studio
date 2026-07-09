add_executable(avs_app
    src/main.cpp
)

target_link_libraries(avs_app
    PRIVATE
        avs_core
        avs_visualization
)

target_compile_features(avs_app
    PRIVATE
        cxx_std_20
)

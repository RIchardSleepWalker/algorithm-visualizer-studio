find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_library(avs_ui
    src/MainWindow.cpp
)

target_include_directories(avs_ui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(avs_ui
    PUBLIC
        Qt6::Widgets
        avs_visualization
        avs_core
)

target_compile_features(avs_ui
    PUBLIC
        cxx_std_20
)

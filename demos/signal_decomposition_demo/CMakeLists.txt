add_executable(signal_decomposition_demo
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(signal_decomposition_demo PRIVATE implot::implot LibDsp::GUI)
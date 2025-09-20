add_executable(reference_implot_demo
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(reference_implot_demo PRIVATE LibDsp::GUI)
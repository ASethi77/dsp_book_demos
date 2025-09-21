if (WIN32)
    add_executable(signal_decomposition_demo
            WIN32
            ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
#            ${PROJECT_SOURCE_DIR}/demos/platform/windows/win_main.cpp
    )
else()
    add_executable(signal_decomposition_demo
            ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )
endif()

target_link_libraries(signal_decomposition_demo
        PRIVATE
        implot::implot
        LibDsp::Stats
        LibDsp::Storage
        LibDsp::Signals
        LibDsp::GUI)
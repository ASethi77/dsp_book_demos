if (WIN32)
    add_executable(convolution_demo
            WIN32
            ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )
else()
    add_executable(convolution_demo
            ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )
endif()

target_link_libraries(convolution_demo
        PRIVATE
        implot::implot
        LibDsp::Stats
        LibDsp::Storage
        LibDsp::Signals
        LibDsp::GUI)
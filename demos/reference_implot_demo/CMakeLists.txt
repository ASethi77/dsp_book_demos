if (WIN32)
    add_executable(reference_implot_demo
            WIN32
            ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
#            ${PROJECT_SOURCE_DIR}/demos/platform/windows/win_main.cpp
    )
else()
    add_executable(reference_implot_demo
            ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )
endif()

target_link_libraries(reference_implot_demo PRIVATE LibDsp::GUI)
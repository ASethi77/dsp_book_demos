set(DSP_GUI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/imgui_window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui/implot_demo.cpp
)

add_library(dsp_gui ${DSP_GUI_SOURCES})
target_include_directories(dsp_gui
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/libdsp
)
target_link_libraries(dsp_gui PUBLIC imgui::imgui implot::implot OpenGL::GL)

add_library(LibDsp::GUI ALIAS dsp_gui)
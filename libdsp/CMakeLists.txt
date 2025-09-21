set(LIBDSP_SRC_DIR
    ${PROJECT_SOURCE_DIR}/libdsp/src
)

set(LIBDSP_INC_DIR
        ${PROJECT_SOURCE_DIR}/libdsp/include
)

set(DSP_GUI_SOURCES
    ${LIBDSP_SRC_DIR}/gui/imgui_window.cpp
    ${LIBDSP_SRC_DIR}/gui/implot_demo.cpp
        include/libdsp/statistics/sample.h
)
add_library(dsp_gui ${DSP_GUI_SOURCES})
target_include_directories(dsp_gui
    PUBLIC ${LIBDSP_INC_DIR}
)
target_link_libraries(dsp_gui PUBLIC imgui::imgui implot::implot OpenGL::GL)

set(DSP_STORAGE_SOURCES
    ${LIBDSP_SRC_DIR}/storage/buffer.cpp
)
add_library(dsp_storage ${DSP_STORAGE_SOURCES})
target_include_directories(dsp_storage
    PUBLIC ${LIBDSP_INC_DIR}
)

set(DSP_STATS_SOURCES
        ${LIBDSP_SRC_DIR}/statistics/sample.cpp
)
add_library(dsp_stats ${DSP_STATS_SOURCES})
target_include_directories(dsp_stats
        PUBLIC ${LIBDSP_INC_DIR}
)

add_library(dsp_signals INTERFACE)
target_include_directories(dsp_signals
        INTERFACE ${LIBDSP_INC_DIR}
)

# Alias targets for user friendliness
add_library(LibDsp::GUI ALIAS dsp_gui)
add_library(LibDsp::Storage ALIAS dsp_storage)
add_library(LibDsp::Stats ALIAS dsp_stats)
add_library(LibDsp::Signals ALIAS dsp_signals)
if (WIN32)
    add_executable(floating_point_error_demo
            WIN32
            ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
#            ${PROJECT_SOURCE_DIR}/demos/platform/windows/win_main.cpp
    )
else()
    add_executable(floating_point_error_demo
            ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )
endif()
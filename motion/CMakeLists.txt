add_library(lib_motion Motion.cpp Motion.hpp)

pico_generate_pio_header(lib_motion
        ${CMAKE_CURRENT_LIST_DIR}/stepperdriver.pio
)

target_link_libraries(lib_motion
        hardware_pio
        pico_stdlib
)

target_include_directories(lib_motion PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

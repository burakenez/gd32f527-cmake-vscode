project(GD32F527R_START LANGUAGES C CXX ASM)

add_library(GD32F527R_START OBJECT
    ${DRIVERS_DIR}/BSP/GD32F527R_START/gd32f527r_start.c
    )

target_include_directories(GD32F527R_START PUBLIC
    ${DRIVERS_DIR}/BSP/GD32F527R_START
    )

target_link_libraries(GD32F527R_START PUBLIC GD32F527_standard_peripheral)

project(GD32F527Z_START LANGUAGES C CXX ASM)

add_library(GD32F527Z_START OBJECT
    ${DRIVERS_DIR}/BSP/GD32F527Z_START/gd32f527z_start.c
    )

target_include_directories(GD32F527Z_START PUBLIC
    ${DRIVERS_DIR}/BSP/GD32F527Z_START
    )

target_link_libraries(GD32F527Z_START PUBLIC GD32F527_standard_peripheral)

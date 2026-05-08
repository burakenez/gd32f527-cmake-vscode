project(GD32F527V_START LANGUAGES C CXX ASM)

add_library(GD32F527V_START OBJECT
    ${DRIVERS_DIR}/BSP/GD32F527V_START/gd32f527v_start.c
    )

target_include_directories(GD32F527V_START PUBLIC
    ${DRIVERS_DIR}/BSP/GD32F527V_START
    )

target_link_libraries(GD32F527V_START PUBLIC GD32F527_standard_peripheral)

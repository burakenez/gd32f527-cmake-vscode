project(GD32F527I_EVAL LANGUAGES C CXX ASM)

add_library(GD32F527I_EVAL OBJECT
    ${DRIVERS_DIR}/BSP/GD32F527I_EVAL/gd32f527i_eval.c
    )

target_include_directories(GD32F527I_EVAL PUBLIC
    ${DRIVERS_DIR}/BSP/GD32F527I_EVAL
    )

target_link_libraries(GD32F527I_EVAL PUBLIC GD32F527_standard_peripheral)

project(GD32F527I_EVAL LANGUAGES C CXX ASM)

add_library(GD32F527I_EVAL INTERFACE)

target_include_directories(GD32F527I_EVAL INTERFACE
    ${DRIVERS_DIR}/BSP/GD32F527I_EVAL
    )

target_link_libraries(GD32F527I_EVAL INTERFACE GD32F527_standard_peripheral)

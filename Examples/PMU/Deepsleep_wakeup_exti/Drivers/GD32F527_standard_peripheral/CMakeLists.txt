project(GD32F527_standard_peripheral LANGUAGES C CXX ASM)

# Comment-out unused source files.
add_library(GD32F527_standard_peripheral OBJECT
	${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_adc.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_can.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_cau.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_cau_aes.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_cau_des.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_cau_tdes.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_crc.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_ctc.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_dac.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_dbg.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_dci.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_dma.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_enet.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_exmc.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_exti.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_fmc.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_fwdgt.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_gpio.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_hau.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_hau_sha_md5.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_i2c.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_i2c_add.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_ipa.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_iref.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_misc.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_pkcau.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_pmu.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_rcu.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_rtc.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_sai.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_sdio.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_spi.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_syscfg.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_timer.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_tli.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_trng.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_usart.c
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Source/gd32f527_wwdgt.c
    )

target_include_directories(GD32F527_standard_peripheral PUBLIC
    ${DRIVERS_DIR}/GD32F527_standard_peripheral/Include
    )

# CMSIS header only library is linked.
target_link_libraries(GD32F527_standard_peripheral PUBLIC CMSIS)

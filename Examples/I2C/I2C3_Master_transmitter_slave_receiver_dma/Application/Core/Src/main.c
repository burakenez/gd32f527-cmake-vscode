/*!
    \file    main.c
    \brief   master transmitter slave receiver through DMA

    \version 2026-01-31, V1.4.0, firmware for GD32F527
*/

/*
    Copyright (c) 2026, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32f527.h"
#include <stdio.h>
#include "gd32f527i_eval.h"

#define I2C3_SLAVE_ADDRESS7    0x82
#define I2C4_SLAVE_ADDRESS7    0x72
#define ARRAYNUM(arr_nanme)    (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))
#define BUFFER_SIZE            (ARRAYNUM(i2c3_buff_tx)-1)

uint8_t i2c3_buff_tx[] = "I2C DMA test";
uint8_t i2c4_buff_rx[BUFFER_SIZE];
__IO ErrStatus state = ERROR;

void rcu_config(void);
void i2c3_gpio_config(void);
void i2c4_gpio_config(void);
void i2c_config(void);
ErrStatus memory_compare(uint8_t *src, uint8_t *dst, uint16_t length);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    dma_single_data_parameter_struct dma_init_struct;

    /* initialize LED1, LED2, as the transfer instruction */
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
    /* enable the peripheral clock */
    rcu_config();
    /* configure the I2C3 and I2C4 GPIO ports */
    i2c3_gpio_config();
    i2c4_gpio_config();
    /* configure I2C3 and I2C4 */
    i2c_config();

    /* initialize DMA channel 1 */
    dma_deinit(DMA0, DMA_CH1);
    dma_single_data_para_struct_init(&dma_init_struct);
    dma_init_struct.direction     = DMA_PERIPH_TO_MEMORY;
    dma_init_struct.memory0_addr  = (uint32_t)i2c4_buff_rx;
    dma_init_struct.memory_inc    = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number        = BUFFER_SIZE;
    dma_init_struct.periph_addr   = (uint32_t)&I2C_ADD_RDATA(I2C4);
    dma_init_struct.periph_inc    = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority      = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA0, DMA_CH1, &dma_init_struct);
    /* select I2C4_RX as DMA peripheral */
    dma_channel_subperipheral_select(DMA0, DMA_CH1, DMA_SUBPERI1);
    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH1);
    /* enable DMA channel 1 */
    dma_channel_enable(DMA0, DMA_CH1);

    /* initialize DMA channel6 for I2C3_TX */
    dma_deinit(DMA0, DMA_CH6);

    dma_init_struct.direction    = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.memory0_addr = (uint32_t)i2c3_buff_tx;
    dma_init_struct.periph_addr  = (uint32_t)&I2C_ADD_TDATA(I2C3);
    dma_init_struct.priority     = DMA_PRIORITY_HIGH;
    dma_single_data_mode_init(DMA0, DMA_CH6, &dma_init_struct);
    /* select I2C3_TX as DMA peripheral */
    dma_channel_subperipheral_select(DMA0, DMA_CH6, DMA_SUBPERI0);
    /* configure DMA mode */
    dma_circulation_disable(DMA0, DMA_CH6);

    /* enable DMA channel 6 */
    dma_channel_enable(DMA0, DMA_CH6);

    /* enable I2C4 receive DMA */
    i2c_add_dma_enable(I2C4, I2C_ADD_DMA_RECEIVE);
    /* enable I2C3 transmit DMA */
    i2c_add_dma_enable(I2C3, I2C_ADD_DMA_TRANSMIT);
    /* wait until I2C bus is idle */
    while(i2c_add_flag_get(I2C3, I2C_ADD_FLAG_I2CBSY));

    /* send a start condition to I2C bus */
    i2c_add_start_on_bus(I2C3);
    /* wait until ADDSEND bit is set*/
    while(!i2c_add_flag_get(I2C4, I2C_ADD_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_add_flag_clear(I2C4, I2C_ADD_FLAG_ADDSEND);

    /* DMA channel6 full transfer finish flag */
    while(!dma_flag_get(DMA0, DMA_CH6, DMA_FLAG_FTF));
    /* DMA channel1 full transfer finish flag */
    while(!dma_flag_get(DMA0, DMA_CH1, DMA_FLAG_FTF));
    while(!i2c_add_flag_get(I2C3, I2C_ADD_FLAG_TC));
    /* send a stop condition to I2C bus*/
    i2c_add_stop_on_bus(I2C3);
    /* wait until stop condition generate */
    while(!i2c_add_flag_get(I2C4, I2C_ADD_FLAG_STPDET));
    /* clear the STPDET bit */
    i2c_add_flag_clear(I2C4, I2C_ADD_FLAG_STPDET);
    /* wait until stop condition generate */
    while(!i2c_add_flag_get(I2C3, I2C_ADD_FLAG_STPDET));
    /* clear the STPDET bit */
    i2c_add_flag_clear(I2C3, I2C_ADD_FLAG_STPDET);
    state = memory_compare(i2c3_buff_tx, i2c4_buff_rx, BUFFER_SIZE);
    if(SUCCESS == state) {
        gd_eval_led_on(LED1);
        gd_eval_led_on(LED2);
    } else {
        gd_eval_led_off(LED1);
        gd_eval_led_off(LED2);
    }
    while(1) {
    }
}

/*!
    \brief      memory compare function
    \param[in]  src : source data
    \param[in]  dst : destination data
    \param[in]  length : the compare data length
    \param[out] none
    \retval     ErrStatus : ERROR or SUCCESS
*/
ErrStatus memory_compare(uint8_t *src, uint8_t *dst, uint16_t length)
{
    while(length--) {
        if(*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}

/*!
    \brief      enable the peripheral clock
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* enable GPIOF clock */
    rcu_periph_clock_enable(RCU_GPIOF);
    /* enable I2C3 clock */
    rcu_periph_clock_enable(RCU_I2C3);
    /* enable I2C4 clock */
    rcu_periph_clock_enable(RCU_I2C4);
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA0);
}

/*!
    \brief      configure the I2C3 GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c3_gpio_config(void)
{
    /* connect PF14 to I2C3_SCL */
    gpio_af_set(GPIOF, GPIO_AF_4, GPIO_PIN_14);
    /* connect PF15 to I2C3_SDA */
    gpio_af_set(GPIOF, GPIO_AF_4, GPIO_PIN_15);
    /* configure GPIO pins of I2C3 */
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_14);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_14);
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_15);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
}

/*!
    \brief      configure the I2C4 GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c4_gpio_config(void)
{
    /* connect PF1 to I2C4_SCL */
    gpio_af_set(GPIOF, GPIO_AF_6, GPIO_PIN_1);
    /* connect PF0 to I2C4_SDA */
    gpio_af_set(GPIOF, GPIO_AF_6, GPIO_PIN_0);
    /* configure GPIO pins of I2C4 */
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_1);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
}

/*!
    \brief      configure the I2C3 and I2C4 interfaces
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c_config(void)
{
    /* configure I2C timing */
    i2c_add_timing_config(I2C3, 0x0, 0x5, 0x0);
    i2c_add_master_clock_config(I2C3, 0x1D, 0x59);
    /* configure I2C address */
    i2c_add_address_config(I2C3, I2C3_SLAVE_ADDRESS7, I2C_ADD_ADDFORMAT_7BITS);
    /* configure slave address */
    i2c_add_master_addressing(I2C3, I2C4_SLAVE_ADDRESS7, I2C_ADD_MASTER_TRANSMIT);
    /* configure number of bytes to be transferred */
    i2c_add_transfer_byte_number_config(I2C3, BUFFER_SIZE);
    /* enable I2C3 */
    i2c_add_enable(I2C3);

    /* configure I2C address */
    i2c_add_address_config(I2C4, I2C4_SLAVE_ADDRESS7, I2C_ADD_ADDFORMAT_7BITS);
    /* enable I2C4 */
    i2c_add_enable(I2C4);
}

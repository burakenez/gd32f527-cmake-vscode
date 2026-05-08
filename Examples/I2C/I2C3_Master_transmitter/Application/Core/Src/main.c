/*!
    \file    main.c
    \brief   master transmitter

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
#include "gd32f527i_eval.h"

#define I2C3_OWN_ADDRESS7 0x72
#define I2C3_SLAVE_ADDRESS7 0x82

uint8_t i2c_transmitter[16];

void rcu_config(void);
void i2c_gpio_config(void);
void i2c_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    int i;

    /* initialize LEDS */
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);

    /* enable the peripheral clock */
    rcu_config();
    /* configure the I2C GPIO ports */
    i2c_gpio_config();
    /* configure I2C */
    i2c_config();

    for(i = 0; i < 16; i++) {
        i2c_transmitter[i] = i + 0x80;
    }

    /* wait until I2C bus is idle */
    while(i2c_add_flag_get(I2C3, I2C_ADD_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_add_start_on_bus(I2C3);
    /* wait until the transmit data buffer is empty */
    I2C_ADD_STAT(I2C3) |= I2C_ADD_STAT_TBE;

    for(i = 0; i < 16; i++) {
        /* wait until the TBE bit is set */
        while(!i2c_add_flag_get(I2C3, I2C_ADD_FLAG_TBE));
        /* data transmission */
        i2c_add_data_transmit(I2C3, i2c_transmitter[i]);
    }
    /* wait for transfer complete flag */
    while(!i2c_add_flag_get(I2C3, I2C_ADD_FLAG_TC));
    /* send a stop condition to I2C bus */
    i2c_add_stop_on_bus(I2C3);
    /* wait until stop condition generate */
    while(!i2c_add_flag_get(I2C3, I2C_ADD_FLAG_STPDET));
    /* clear the STPDET bit */
    i2c_add_flag_clear(I2C3, I2C_ADD_FLAG_STPDET);

    gd_eval_led_on(LED1);
    gd_eval_led_on(LED2);

    /* infinite loop */
    while(1) {
    }
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
}

/*!
    \brief      configure the I2C3 GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c_gpio_config(void)
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
    \brief      configure the I2C3 interface
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c_config(void)
{
    /* configure I2C timing */
    i2c_add_timing_config(I2C3, 0, 0x5, 0);
    i2c_add_master_clock_config(I2C3, 0x1D, 0x59);

    /* configure I2C address */
    i2c_add_address_config(I2C3, I2C3_OWN_ADDRESS7, I2C_ADD_ADDFORMAT_7BITS);
    /* send slave address to I2C bus */
    i2c_add_master_addressing(I2C3, I2C3_SLAVE_ADDRESS7, I2C_ADD_MASTER_TRANSMIT);
    /* configure number of bytes to be transferred */
    i2c_add_transfer_byte_number_config(I2C3, 16);
    /* enable I2C3 */
    i2c_add_enable(I2C3);
}

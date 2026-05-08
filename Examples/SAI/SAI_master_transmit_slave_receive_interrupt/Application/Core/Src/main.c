/*!
    \file    main.c
    \brief   sai master send and slave receive data use interrupt mode

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
#include "main.h"

uint32_t sai_send_array[ARRAYSIZE];
uint32_t sai_receive_array[ARRAYSIZE];

__IO uint32_t send_n = 0U, receive_n = 0U;
/* function declarations */
void rcu_config(void);
void gpio_config(void);
void sai_config(void);
void nvic_config(void);
ErrStatus memory_compare(uint32_t *src, uint32_t *dst, uint32_t length);

static void fill_array_value(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/

int main(void)
{
    /* initilized led3 */
    gd_eval_led_init(LED3);
    /* initilized the array value */
    fill_array_value();
    /* enable peripheral clock */
    rcu_config();
    /* configure GPIO */
    gpio_config();
    /* configure SAI */
    sai_config();
    /* NVIC configuration */
    nvic_config();
    /* enable the SAI interrupt */
    sai_interrupt_enable(SAI_BLOCK0, SAI_INT_FFREQ);
    sai_interrupt_enable(SAI_BLOCK1, SAI_INT_FFREQ);
    /* sai enable*/
    sai_enable(SAI_BLOCK1);
    sai_enable(SAI_BLOCK0);
    /* wait transmit complete */
    while(receive_n < ARRAYSIZE) {
    }

    sai_disable(SAI_BLOCK0);
    sai_disable(SAI_BLOCK1);
    sai_flag_clear(SAI_BLOCK0, SAI_FLAG_OUERR);
    sai_flag_clear(SAI_BLOCK1, SAI_FLAG_OUERR);

    /* compare receive data with send data */
    if(memory_compare(sai_receive_array, sai_send_array, ARRAYSIZE)) {
        gd_eval_led_on(LED3);
    } else {
        gd_eval_led_off(LED3);
    }
    /* Infinite loop */
    while(1) {
    }
}

/*!
    \brief      configure the system clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOF);

    rcu_pllsai_q_config(150U, 3U);
    rcu_sai_clock_config(RCU_SAISRC_PLLSAIQ);
    rcu_osci_on(RCU_PLLSAI_CK);

    if(ERROR == rcu_osci_stab_wait(RCU_PLLSAI_CK)) {
        while(1) {
        }
    }
    /* enable SAI clock */
    rcu_periph_clock_enable(RCU_SAI);
}

/*!
    \brief      configure the GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* configure GPIO pins of SAI_BLOCK0: SAI_MCLK(PE2) SAI_FS(PE4) SAI_SCK(PE5) SAI_SD(PD6) */
    gpio_af_set(GPIOE, GPIO_AF_6, GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5);
    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,
                            GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5);
    gpio_af_set(GPIOD, GPIO_AF_6, GPIO_PIN_6);
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_6);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

    /* configure GPIO pins of SAI_BLOCK1: SAI_SD(PF6) SAI_MCLK(PF7) SAI_SCK(PF8) SAI_FS(PF9) */
    gpio_af_set(GPIOF, GPIO_AF_6, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9);
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_PULLUP,
                  GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,
                            GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9);
}

/*!
    \brief      configure the sai peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void sai_config(void)
{
    sai_parameter_struct sai_structure;
    sai_frame_parameter_struct sai_frame_structure;
    sai_slot_parameter_struct sai_slot_structure;

    sai_struct_para_init(&sai_structure);
    sai_frame_struct_para_init(&sai_frame_structure);
    sai_slot_struct_para_init(&sai_slot_structure);

    /* initialize SAI frame */
    sai_frame_structure.frame_width            = 128U;
    sai_frame_structure.frame_sync_width       = 64U;
    sai_frame_structure.frame_sync_function    = SAI_FS_FUNC_START_CHANNEL;
    sai_frame_structure.frame_sync_polarity    = SAI_FS_POLARITY_LOW;
    sai_frame_structure.frame_sync_offset      = SAI_FS_OFFSET_BEGINNING;
    sai_frame_init(SAI_BLOCK1, &sai_frame_structure);
    sai_frame_init(SAI_BLOCK0, &sai_frame_structure);

    /* initialize SAI slot */
    sai_slot_structure.slot_number             = 4U;
    sai_slot_structure.slot_width              = SAI_SLOT_WIDTH_32BIT;
    sai_slot_structure.data_offset             = 0U;
    sai_slot_structure.slot_active             = SAI_SLOT_ACTIVE_ALL;
    sai_slot_init(SAI_BLOCK1, &sai_slot_structure);
    sai_slot_init(SAI_BLOCK0, &sai_slot_structure);

    /* initialize SAI_B0  */
    sai_structure.operating_mode               = SAI_MASTER_TRANSMITTER;
    sai_structure.protocol                     = SAI_PROTOCOL_POLYMORPHIC;
    sai_structure.data_width                   = SAI_DATAWIDTH_32BIT;
    sai_structure.shift_dir                    = SAI_SHIFT_MSB;
    sai_structure.sample_edge                  = SAI_SAMPEDGE_RISING;
    sai_structure.sync_mode                    = SAI_SYNCMODE_ASYNC;
    sai_structure.output_drive                 = SAI_OUTPUT_WITH_SAIEN;
    sai_structure.clk_div_bypass               = SAI_CLKDIV_BYPASS_OFF;
    sai_structure.mclk_div                     = SAI_MCLKDIV_4;
    sai_structure.mclk_oversampling            = SAI_MCLK_OVERSAMP_256;
    sai_structure.mclk_enable                  = SAI_MCLK_ENABLE;
    sai_structure.fifo_threshold               = SAI_FIFOTH_HALF;
    sai_init(SAI_BLOCK0, &sai_structure);

    /* initialize SAI_B1 */
    sai_structure.operating_mode               = SAI_SLAVE_RECEIVER;
    sai_init(SAI_BLOCK1, &sai_structure);
}

/**
    \brief      configure the nested vectored interrupt controller
    \param[in]  none
    \param[out] none
    \retval     none
  */
void nvic_config(void)
{
    nvic_irq_enable(SAI_IRQn, 1U, 0U);
}

/*!
    \brief      memory compare function
    \param[in]  src: source data pointer
    \param[in]  dst: destination data pointer
    \param[in]  length: the compare data length
    \param[out] none
    \retval     ErrStatus : ERROR or SUCCESS
*/
ErrStatus memory_compare(uint32_t *src, uint32_t *dst, uint32_t length)
{
    while(length--) {
        if(*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}

/*!
    \brief      fill the buffer with specified value
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void fill_array_value(void)
{
    uint16_t i;
    for(i = 0U; i < ARRAYSIZE; i++) {
        sai_send_array[i] = i;
    }
}

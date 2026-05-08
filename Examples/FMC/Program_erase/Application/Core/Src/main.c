/*!
    \file    main.c
    \brief   main flash program, erase

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
#include "fmc_operation.h"
#include "gd32f527i_eval.h"

#define ERASE_ADDRESS               ((uint32_t)0x08004000)
#define WRITE_ADDRESS_DOUBLE_WORD   ((uint32_t)0x08004000)
#define WRITE_ADDRESS_WORD          ((uint32_t)0x08005000)
#define WRITE_ADDRESS_HALF_WORD     ((uint32_t)0x08006000)
#define WRITE_ADDRESS_BYTE          ((uint32_t)0x08007000)

uint64_t data_64_1[10] = {0x1111111111111111U, 0x2222222222222222U, 0x3333333333333333U,
                          0x4444444444444444U, 0x5555555555555555U, 0x6666666666666666U, 0x7777777777777777U,
                          0x8888888888888888U, 0x9999999999999999U, 0xAAAAAAAAAAAAAAAAU
                         };
uint64_t data_64_2[10];
uint32_t data_32_1[10] = {0x5555AAAA, 0x5555AAAA, 0x5555AAAA, 0x5555AAAA, 0x5555AAAA, 0x5555AAAA, 0x5555AAAA, 0x5555AAAA, 0x5555AAAA, 0x5555AAAA};
uint32_t data_32_2[10];
uint16_t data_16_1[10] = {0x44BB, 0x44BB, 0x44BB, 0x44BB, 0x44BB, 0x44BB, 0x44BB, 0x44BB, 0x44BB, 0x44BB,};
uint16_t data_16_2[10];
uint8_t data_8_1[10] = {0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D};
uint8_t data_8_2[10];

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* configure COM port */
    gd_eval_com_init(EVAL_COM0);

    /* erases the sector of a given sector number */
    fmc_erase_sector_by_address(ERASE_ADDRESS);

    /* write 64 bit length data to a given address */
    fmc_write_64bit_data(WRITE_ADDRESS_DOUBLE_WORD, 10, data_64_1);
    /* read 64 bit length data from a given address */
    fmc_read_64bit_data(WRITE_ADDRESS_DOUBLE_WORD, 10, data_64_2);

    /* write 32 bit length data to a given address */
    fmc_write_32bit_data(WRITE_ADDRESS_WORD, 10, data_32_1);
    /* read 32 bit length data from a given address */
    fmc_read_32bit_data(WRITE_ADDRESS_WORD, 10, data_32_2);

    /* write 16 bit length data to a given address */
    fmc_write_16bit_data(WRITE_ADDRESS_HALF_WORD, 10, data_16_1);
    /* read 16 bit length data from a given address */
    fmc_read_16bit_data(WRITE_ADDRESS_HALF_WORD, 10, data_16_2);

    /* write 8 bit length data to a given address */
    fmc_write_8bit_data(WRITE_ADDRESS_BYTE, 10, data_8_1);
    /* read 8 bit length data from a given address */
    fmc_read_8bit_data(WRITE_ADDRESS_BYTE, 10, data_8_2);

    while(1) {
    }
}


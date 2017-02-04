/*
 * OchiMicro.cpp
 *
 * Created: 7/30/2016 2:13:54 PM
 * Author : John Smith
 */ 

#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include "config.h"
#include "main.h"
#include "pm.h"
#include "dma.h"
#include "io_spi.h"
#include "io_twi.h"
#include "io_usart.h"
#include "dev_mmcsd.h"
#include "cmd_parser.h"
#include "dev_ov7690.h"

unsigned long FCPU = F_CPU;

Pm pm;
Dma dma;
Io_Usart usart(&USE_UART2, UART_SPEED, true, true, usart.c_mode_async, usart.p_mode_none, usart.s_bits_one, NULL, -1, false);
Io_Usart card_usart(&USE_UART0, 0, true, true, usart.c_mode_mspi, usart.p_mode_none, usart.s_bits_one, &CARD_CS_PORT, 1 << CARD_CS_PIN, true);
//Io_Spi card_spi(&USE_SPI0, 0, card_spi.mode_3, &CARD_CS_PORT, 1 << CARD_CS_PIN);
Io_Twi sensor_twi(&USE_TWI0);
SD_Spi sd_spi(NULL, &card_usart);
//SD_Spi sd_spi(&card_spi, NULL);
Cmd_Parser cmd_parser(&usart);
Ov7690 ov7690(&sensor_twi, &EDMA.CH2);
Main_App main_app;

volatile unsigned long buffer_change_count;
volatile unsigned long frame_count;
	unsigned char buffer[512];

Main_App::Main_App()
{
#ifdef USE_UART0
	card_usart.dmarx = &EDMA.CH1;
	card_usart.dmatx = &EDMA.CH0;
#endif
	memset(buffer, 0, 512);
	sd_spi.read(buffer,0, 1);
}

void terminal_print_hex(unsigned char data)
{
	unsigned char ch1 = data >> 4;
	unsigned char ch2 = data & 0x0F;
	if(ch1 < 10)
	usart.transmit(ch1 + '0');
	else
	usart.transmit((ch1 - 10) + 'A');
	if(ch2 < 10)
	usart.transmit(ch2 + '0');
	else
	usart.transmit((ch2 - 10) + 'A');
}

unsigned long last = 0;
#define MULTIPLYER 10
#define NR_OF_COLS (OV7690_BUFF_LEN / MULTIPLYER)
unsigned char old_avarage = 0;
bool show_next_frame = false;


int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		if(buffer_change_count != ov7690.buffer_change_count && (ov7690.buffer_change_count & 0x07) == 0)
		{
			if(show_next_frame)
			{
				buffer_change_count = ov7690.buffer_change_count;
				unsigned int cnt = 0;
				for (; cnt < BYTES_PER_PIXEL * OV7690_BUFF_LEN;)
				{
					
					unsigned int cnt2 = 0;
					unsigned int tmp = 0;
					for (; cnt2 < (MULTIPLYER * BYTES_PER_PIXEL); cnt2++)
					{
							tmp += ov7690.buff1[cnt++];
					}
					terminal_print_hex(tmp / MULTIPLYER * BYTES_PER_PIXEL);
				}
				usart.Put_Str("\r\n");
			}
			//usart.transmit(254);
		}
		if(frame_count != ov7690.frame_count)
		{
			frame_count = ov7690.frame_count;
			unsigned long tmp = buffer_change_count - last;
			last = buffer_change_count;
			//usart.transmit(255);
			unsigned char avarage_level;
			ov7690.reg_read(0x04, &avarage_level);
			if(show_next_frame)
			{
				terminal_print_hex(avarage_level);
				usart.Put_Str("\n\r");
			}
			show_next_frame = false;
			if(old_avarage != avarage_level)
			{
				old_avarage = avarage_level;
				show_next_frame = true;
			}
		}
    }
}


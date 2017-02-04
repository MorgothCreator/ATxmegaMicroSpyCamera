/*
 * io_usart.h
 *
 * Created: 3/3/2015 6:24:35 PM
 *  Author: someone
 */ 


#ifndef IO_USART_H_
#define IO_USART_H_

#include <avr/io.h>
#include <stdio.h>
#include "config.h"

class Io_Usart
{
public:
	Io_Usart(volatile USART_t* port, unsigned long speed, bool Tx_Enable, bool Rx_Enable, unsigned char c_mode, unsigned char p_mode, unsigned char s_mode, PORT_t *cs_port, unsigned char cs_pin_mask
#ifdef _AVR_ATXMEGA32E5_H_INCLUDED
							, bool Uart0RelocatePins
#endif
);
	~Io_Usart();
	bool reinit(volatile USART_t* port, unsigned long speed, bool Tx_Enable, bool Rx_Enable, unsigned char c_mode, unsigned char p_mode, unsigned char s_mode, PORT_t *cs_port, unsigned char cs_pin_mask
#ifdef _AVR_ATXMEGA32E5_H_INCLUDED
							, bool Uart0RelocatePins
#endif
	);
	bool transmit(char data);
	bool txrx_byte(unsigned char *data);
	bool txrx_buff(unsigned int BytesSend, unsigned int BytesReceive);
	bool rx_buff(unsigned int BytesReceive);
	bool tx_buff(unsigned int BytesSend);
	void set_baud(unsigned int baud);
	void cs_assert();
	void cs_deasert();
	unsigned char cs_read();
	bool transmit_nb(char data);
	char receive(void);
	bool receive_nb(char * data);
	void Put_Str(const char *data);
	void Put_Hex_Char(unsigned char data);
	void Put_Hex_Buff(const unsigned char *data, unsigned int len);
	unsigned int write(const char *pcBuf, unsigned int len);
	unsigned int print(const char *pcString, ...);
	
	void (*rx_complete_func)(void *structure, char *rx_buff, unsigned int data_len);
	void *rx_complete_struct;
	void (*rx_char_receive_func)(void *structure, char data);
	void *rx_char_receive_struct;
	
	volatile USART_t *base_addr;
	Io_Usart *usart;
	volatile PORT_t *cs_port;
	EDMA_CH_t *dmarx;
	EDMA_CH_t *dmatx;
	unsigned char cs_pin_mask;
	char *rx_buffer;
	volatile unsigned int rx_cnt;
	volatile signed long uart_rx_timeout_cnt;
	unsigned char *spim_buff;
	bool control_cs_pin;

	enum
	{
		p_mode_none = USART_PMODE_DISABLED_gc,
		p_mode_even = USART_PMODE_EVEN_gc,
		p_mode_odd = USART_PMODE_ODD_gc,
	};
	enum
	{
		s_bits_one = 0,
		s_bits_two = USART_SBMODE_bm,
	};
	enum
	{
		c_mode_async = USART_CMODE_ASYNCHRONOUS_gc,
		c_mode_sync = USART_CMODE_SYNCHRONOUS_gc,
		c_mode_irda = USART_CMODE_IRDA_gc,
		c_mode_mspi = USART_CMODE_MSPI_gc,
	};

protected:
private:
	static void service(Io_Usart *structure);
};



#endif /* IO_USART_H_ */
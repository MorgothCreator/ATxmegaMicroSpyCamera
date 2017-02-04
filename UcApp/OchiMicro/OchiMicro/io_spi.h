/*
 * io_spi.h
 *
 * Created: 11/18/2015 5:27:47 PM
 *  Author: John Smith
 */ 


#ifndef IO_SPI_H_
#define IO_SPI_H_

#include <stdbool.h>
#include "config.h"
#include <avr/io.h>

class Io_Spi{
public:
	Io_Spi(volatile SPI_t* port, unsigned char prescaller, unsigned char mode, PORT_t *cs_port, unsigned char cs_pin_mask);
	~Io_Spi();
	bool reinit(volatile SPI_t* port, unsigned char prescaller, unsigned char mode, PORT_t *cs_port, unsigned char cs_pin_mask);
	bool txrx_byte(unsigned char *data);
	bool txrx_buff(unsigned int BytesSend, unsigned int BytesReceive);
	bool rx_buff(unsigned int BytesReceive);
	bool tx_buff(unsigned int BytesSend);
	void set_baud(unsigned int baud);
	void cs_assert();
	void cs_deasert();
	unsigned char cs_read();
	
	volatile SPI_t *base_addr;
	Io_Spi *spi;
	unsigned char cs_pin_mask;
	volatile PORT_t *cs_port;
	EDMA_CH_t *dmarx;
	EDMA_CH_t *dmatx;
	
	unsigned char *spim_buff;

	bool control_cs_pin;
	
	enum {
		prescaller_2 = SPI_PRESCALER_DIV4_gc | SPI_CLK2X_bm,
		prescaller_4 = SPI_PRESCALER_DIV4_gc,
		prescaller_8 = SPI_PRESCALER_DIV16_gc | SPI_CLK2X_bm,
		prescaller_16 = SPI_PRESCALER_DIV16_gc,
		prescaller_32 = SPI_PRESCALER_DIV64_gc | SPI_CLK2X_bm,
		prescaller_64 = SPI_PRESCALER_DIV64_gc,
		prescaller_128 = SPI_PRESCALER_DIV128_gc,
	};
	enum {
		mode_0 = SPI_MODE_0_gc,
		mode_1 = SPI_MODE_1_gc,
		mode_2 = SPI_MODE_2_gc,
		mode_3 = SPI_MODE_3_gc,
	};
};



#endif /* IO_SPI_H_ */
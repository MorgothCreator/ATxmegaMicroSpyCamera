/*
 * io_spi.cpp
 *
 * Created: 11/18/2015 5:28:01 PM
 *  Author: John Smith
 */ 

#include "config.h"
#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include "io_spi.h"

Io_Spi::Io_Spi(volatile SPI_t* port, unsigned char prescaller, unsigned char mode, PORT_t *cs_port, unsigned char cs_pin_mask)
{
	reinit(port, prescaller, mode, cs_port, cs_pin_mask);
}

Io_Spi::~Io_Spi()
{
	this->base_addr->CTRL &= ~(1<<SPI_ENABLE_bp);
	this->spi = NULL;
	this->base_addr = NULL;
}

bool Io_Spi::reinit(volatile SPI_t* port, unsigned char prescaller, unsigned char mode, PORT_t *cs_port, unsigned char cs_pin_mask)
{
	if(!port)
		return false;
#if defined(SPIC) && defined(USE_SPI0)
	if((int)port == (int)&SPIC)
	{
		PORTC.OUTSET = 1 << 5/* | 1 << 7*/;
		PORTC.DIRSET = 1 << 5 | 1 << 7;
		//PORTC.PIN6CTRL = 3 << 3;
		PORTC.PIN6CTRL = PORT_OPC_PULLUP_gc;
		PORTC.DIRCLR = 1<<6;
		PORTC.OUTSET = 1 << 4;
		PORTC.DIRSET = 1 << 4;
	}
#endif
#if defined(SPID) && defined(USE_SPI1)
	else if((int)port == (int)&SPID)
	{
		PORTD.OUTSET = 1 << 5 | 1 << 7;
		PORTD.DIRSET = 1 << 5 | 1 << 7;
		//PORTC.PIN6CTRL = 3 << 3;
		PORTD.PIN6CTRL = PORT_OPC_PULLUP_gc;
		PORTD.DIRCLR = 1<<6;
		PORTD.OUTSET = 1 << 4;
		PORTD.DIRSET = 1 << 4;
	}
#endif
#if defined(SPIE) && defined(USE_SPI2)
	else if((int)port == (int)&SPIE)
	{
		PORTE.OUTSET = 1 << 5 | 1 << 7;
		PORTE.DIRSET = 1 << 5 | 1 << 7;
		//PORTC.PIN6CTRL = 3 << 3;
		PORTE.PIN6CTRL = PORT_OPC_PULLUP_gc;
		PORTE.DIRCLR = 1<<6;
		PORTE.OUTSET = 1 << 4;
		PORTE.DIRSET = 1 << 4;
	}
#endif
#if defined(SPIF) && defined(USE_SPI3)
	else if((int)port == (int)&SPIF)
	{
		PORTF.OUTSET = 1 << 5 | 1 << 7;
		PORTF.DIRSET = 1 << 5 | 1 << 7;
		//PORTC.PIN6CTRL = 3 << 3;
		PORTF.PIN6CTRL = PORT_OPC_PULLUP_gc;
		PORTF.DIRCLR = 1<<6;
		PORTF.OUTSET = 1 << 4;
		PORTF.DIRSET = 1 << 4;
	}
#endif
	if(cs_port && cs_pin_mask)
	{
		cs_port->OUTSET = cs_pin_mask;
		cs_port->DIRSET = cs_pin_mask;
	}
	spi = this;
	base_addr = port;
	this->cs_port = cs_port;
	this->cs_pin_mask = cs_pin_mask;
	unsigned char tmp = port->CTRL;
	tmp = (tmp & (~SPI_MASTER_bm)) | (1 << SPI_MASTER_bp);
	tmp = (tmp & (~SPI_MODE_gm)) | (SPI_MODE_gm & mode);
	tmp = (tmp & (~(SPI_PRESCALER_gm | SPI_CLK2X_bm))) | ((SPI_PRESCALER_gm & prescaller) | SPI_CLK2X_bm);
	tmp = (tmp | (1<<SPI_ENABLE_bp));
	port->CTRL = tmp;
	return true;
}

bool Io_Spi::txrx_byte(unsigned char *data)
{
	volatile SPI_t* port = this->base_addr;
	/* Start transmission */
	port->DATA = *data;
	/* Wait for transmission complete */
	while(!(port->STATUS & SPI_IF_bm)) { }
	*data = port->DATA;
	return true;
}

bool Io_Spi::txrx_buff(unsigned int BytesSend, unsigned int BytesReceive)
{
	if(!spim_buff)
	return false;
	if(control_cs_pin)
	this->cs_port->OUTCLR = this->cs_pin_mask;
	unsigned long cnt = 0;
	for (; cnt < BytesSend + BytesReceive; cnt++)
	{
		if(cnt < BytesSend)
		{
			if(txrx_byte(&spim_buff[cnt]) == false)
			return false;
		}
		else
		{
			if(txrx_byte(&spim_buff[cnt]) == false)
			return false;
		}
	}
	if(control_cs_pin)
	this->cs_port->OUTSET = this->cs_pin_mask;
	return true;
}

bool Io_Spi::rx_buff(unsigned int BytesReceive)
{
	if(!spim_buff)
		return false;
	if(control_cs_pin)
		cs_port->OUTCLR = this->cs_pin_mask;
	if(!dmarx)
	{
		unsigned long cnt = 0;
		for (; cnt < BytesReceive; cnt++)
		{
			unsigned char tmp = 0xFF;
			if(txrx_byte(&tmp) == false)
			return false;
			spim_buff[cnt] = tmp;
		}
	}
	else
	{
		dmarx->ADDR = (unsigned int)spim_buff;
		dmarx->ADDRCTRL = EDMA_CH_RELOAD_NONE_gc | EDMA_CH_DIR_INC_gc;
		dmarx->TRIGSRC = EDMA_CH_TRIGSRC_SPIC_RXC_gc;
		unsigned int cnt = BytesReceive;
		for(; cnt >= 256; cnt -= 256)
		{
			dmarx->TRFCNT = 0;
			dmarx->CTRLA = EDMA_CH_SINGLE_bm;
			dmarx->CTRLA |= EDMA_CH_ENABLE_bm/* | EDMA_CH_BURSTLEN_bm*/;
			base_addr->DATA = 0xFF;
			while(!(dmarx->CTRLB & EDMA_CH_TRNIF_bm));
			dmarx->CTRLB |= EDMA_CH_TRNIF_bm;
		}
		if(cnt)
		{
			dmarx->TRFCNT = cnt;
			dmarx->CTRLA = EDMA_CH_SINGLE_bm;
			dmarx->CTRLA |= EDMA_CH_ENABLE_bm/* | EDMA_CH_BURSTLEN_bm*/;
			base_addr->DATA = 0xFF;
			while(!(dmarx->CTRLB & EDMA_CH_TRNIF_bm));
			dmarx->CTRLB |= EDMA_CH_TRNIF_bm;
		}
	}
	if(control_cs_pin)
		cs_port->OUTSET = this->cs_pin_mask;
	return true;
}

bool Io_Spi::tx_buff(unsigned int BytesSend)
{
	if(!spim_buff)
		return false;
	if(control_cs_pin)
		cs_port->OUTCLR = this->cs_pin_mask;
	if(!dmatx)
	{
		unsigned long cnt = 0;
		for (; cnt < BytesSend; cnt++)
		{
			unsigned char tmp = spim_buff[cnt];
			if(txrx_byte(&tmp) == false)
				return false;
		}
	}
	else
	{
		dmarx->ADDR = (unsigned int)spim_buff;
		dmarx->ADDRCTRL = EDMA_CH_RELOAD_NONE_gc | EDMA_CH_DIR_INC_gc;
		dmarx->TRIGSRC = EDMA_CH_TRIGSRC_SPIC_DRE_gc;
		dmarx->TRFCNT = BytesSend;
		dmarx->CTRLA = EDMA_CH_ENABLE_bm;
		while(!(dmarx->CTRLB & EDMA_CH_TRNIF_bm));
	}
	if(control_cs_pin)
		cs_port->OUTSET = this->cs_pin_mask;
	return true;
}

void Io_Spi::set_baud(unsigned int baud)
{
	unsigned char tmp = base_addr->CTRL;
	tmp = (tmp & (~(SPI_PRESCALER_gm | SPI_CLK2X_bm))) | ((SPI_PRESCALER_gm & baud) | SPI_CLK2X_bm);
	base_addr->CTRL = tmp;
}

void Io_Spi::cs_assert()
{
	cs_port->OUTCLR = cs_pin_mask;
}

void Io_Spi::cs_deasert()
{
	cs_port->OUTSET = cs_pin_mask;
}

unsigned char Io_Spi::cs_read()
{
	return cs_port->IN & cs_pin_mask;
}
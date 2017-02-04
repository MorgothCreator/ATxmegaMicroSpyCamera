/*
 * io_usart.cpp
 *
 * Created: 3/3/2015 6:24:22 PM
 *  Author: someone
 */ 

#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include <avr/xmega.h>
#include "io_usart.h"

static const char g_pcHex[] PROGMEM = {"0123456789abcdef"};

#if defined(USARTC0) && defined(USE_UART0)
Io_Usart *io_usart0_service;
#endif
#if defined(USARTC1) && defined(USE_UART1)
Io_Usart *io_usart1_service;
#endif
#if defined(USARTD0) && defined(USE_UART2)
Io_Usart *io_usart2_service;
#endif
#if defined(USARTD1) && defined(USE_UART3)
Io_Usart *io_usart3_service;
#endif
#if defined(USARTE0) && defined(USE_UART4)
Io_Usart *io_usart4_service;
#endif
#if defined(USARTE1) && defined(USE_UART5)
Io_Usart *io_usart5_service;
#endif
#if defined(USARTF0) && defined(USE_UART6)
Io_Usart *io_usart6_service;
#endif
#if defined(USARTF1) && defined(USE_UART7)
Io_Usart *io_usart7_service;
#endif

Io_Usart::Io_Usart (volatile USART_t* port, unsigned long speed, bool Tx_Enable, bool Rx_Enable, unsigned char c_mode, unsigned char p_mode, unsigned char s_mode, PORT_t *cs_port, unsigned char cs_pin_mask
#ifdef _AVR_ATXMEGA32E5_H_INCLUDED
, bool Uart0RelocatePins
#endif
)
{
	reinit(port, speed, Tx_Enable, Rx_Enable, c_mode, p_mode, s_mode, cs_port, cs_pin_mask
#ifdef _AVR_ATXMEGA32E5_H_INCLUDED
	, Uart0RelocatePins
#endif
	);
	//htimer.service_add((void *)&service, this);
}

Io_Usart::~Io_Usart ()
{
	base_addr->CTRLB = 0;
	//htimer.service_remove((void *)&service, this);
	usart = NULL;
	base_addr = NULL;
}

bool Io_Usart::reinit (volatile USART_t* port, unsigned long speed, bool Tx_Enable, bool Rx_Enable, unsigned char c_mode, unsigned char p_mode, unsigned char s_mode, PORT_t *cs_port, unsigned char cs_pin_mask
#ifdef _AVR_ATXMEGA32E5_H_INCLUDED
	, bool Uart0RelocatePins
#endif
) 
{
	if(!port)
		return false;
#if defined(USARTC0) && defined(USE_UART0)
	if((int)port == (int)&USARTC0)
	{
#ifdef _AVR_ATXMEGA32E5_H_INCLUDED
		if(Uart0RelocatePins == false)
		{
			PORTC.REMAP &= ~PORT_USART0_bm;
#endif
			if(c_mode == this->c_mode_mspi/* || c_mode == this->c_mode_sync*/)
			{
				PORTC.DIRSET = 1<<1;
				PORTC.OUTSET = 1<<1;
			}
			PORTC.DIRSET = 1<<3;
			PORTC.OUTSET = 1<<3;
			PORTC.PIN2CTRL = PORT_OPC_PULLUP_gc;
#ifdef _AVR_ATXMEGA32E5_H_INCLUDED
		}
		else
		{
			PORTC.REMAP |= PORT_USART0_bm;
			if(c_mode == this->c_mode_mspi/* || c_mode == this->c_mode_sync*/)
			{
				PORTC.DIRSET = 1<<5;
				PORTC.OUTSET = 1<<5;
			}
			PORTC.DIRSET = 1<<7;
			PORTC.OUTSET = 1<<7;
			PORTC.PIN6CTRL = PORT_OPC_PULLUP_gc;
		}
#endif
		io_usart0_service = this;
	}
	else 
#endif
#if defined(USARTC1) && defined(USE_UART1)
	if((int)port == (int)&USARTC1)
	{
		if(c_mode == this->c_mode_mspi/* || c_mode == this->c_mode_sync*/)
		{
			PORTC.DIRSET = 1<<5;
			PORTC.OUTSET = 1<<5;
		}
		PORTC.DIRSET = 1<<7;
		PORTC.OUTSET = 1<<7;
		PORTC.PIN6CTRL = PORT_OPC_PULLUP_gc;
		io_usart1_service = this;
	}
	else 
#endif
#if defined(USARTD0) && defined(USE_UART2)
	if((int)port == (int)&USARTD0)
	{
#ifdef _AVR_ATXMEGA32E5_H_INCLUDED
		if(Uart0RelocatePins == false)
		{
			PORTD.REMAP &= ~PORT_USART0_bm;
#endif
			if(c_mode == this->c_mode_mspi/* || c_mode == this->c_mode_sync*/)
			{
				PORTD.DIRSET = 1<<1;
				PORTD.OUTSET = 1<<1;
			}
			PORTD.DIRSET = 1<<3;
			PORTD.OUTSET = 1<<3;
			PORTD.PIN2CTRL = PORT_OPC_PULLUP_gc;
#ifdef _AVR_ATXMEGA32E5_H_INCLUDED
		}
		else
		{
			PORTD.REMAP |= PORT_USART0_bm;
			if(c_mode == this->c_mode_mspi/* || c_mode == this->c_mode_sync*/)
			{
				PORTD.DIRSET = 1<<5;
				PORTD.OUTSET = 1<<5;
			}
			PORTD.DIRSET = 1<<7;
			PORTD.OUTSET = 1<<7;
			PORTD.PIN6CTRL = PORT_OPC_PULLUP_gc;
		}
#endif
		io_usart2_service = this;
	}
	else 
#endif
#if defined(USARTD1) && defined(USE_UART3)
	if((int)port == (int)&USARTD1)
	{
		if(c_mode == this->c_mode_mspi || c_mode == this->c_mode_sync)
		{
			PORTD.DIRSET = 1<<5;
			PORTD.OUTSET = 1<<5;
		}
		PORTD.DIRSET = 1<<7;
		PORTD.OUTSET = 1<<7;
		PORTD.PIN6CTRL = PORT_OPC_PULLUP_gc;
		io_usart3_service = this;
	}
	else 
#endif
#if defined(USARTE0) && defined(USE_UART4)
	if((int)port == (int)&USARTE0)
	{
		if(c_mode == this->c_mode_mspi || c_mode == this->c_mode_sync)
		{
			PORTE.DIRSET = 1<<1;
			PORTE.OUTSET = 1<<1;
		}
		PORTE.DIRSET = 1<<3;
		PORTE.OUTSET = 1<<3;
		PORTE.PIN2CTRL = PORT_OPC_PULLUP_gc;
		io_usart4_service = this;
	}
#endif
#if defined(USARTE1) && defined(USE_UART5)
	else if((int)port == (int)&USARTE1)
	{
		if(c_mode == this->c_mode_mspi || c_mode == this->c_mode_sync)
		{
			PORTE.DIRSET = 1<<5;
			PORTE.OUTSET = 1<<5;
		}
		PORTE.DIRSET = 1<<7;
		PORTE.OUTSET = 1<<7;
		PORTE.PIN6CTRL = PORT_OPC_PULLUP_gc;
		io_usart5_service = this;
	}
	else 
#endif
#if defined(USARTF0) && defined(USE_UART6)
	if((int)port == (int)&USARTF0)
	{
		if(c_mode == this->c_mode_mspi || c_mode == this->c_mode_sync)
		{
			PORTF.DIRSET = 1<<1;
			PORTF.OUTSET = 1<<1;
		}
		PORTF.DIRSET = 1<<3;
		PORTF.OUTSET = 1<<3;
		PORTF.PIN2CTRL = PORT_OPC_PULLUP_gc;
		io_usart6_service = this;
	}
	else 
#endif
#if defined(USARTF1) && defined(USE_UART7)
	if((int)port == (int)&USARTF1)
	{
		if(c_mode == this->c_mode_mspi || c_mode == this->c_mode_sync)
		{
			PORTF.DIRSET = 1<<5;
			PORTF.OUTSET = 1<<5;
		}
		PORTF.DIRSET = 1<<7;
		PORTF.OUTSET = 1<<7;
		PORTF.PIN6CTRL = PORT_OPC_PULLUP_gc;
		io_usart7_service = this;
	}
	else 
#endif
	
	{
			usart = NULL;
			base_addr = NULL;
			return false;
	}
	if(cs_port && cs_pin_mask)
	{
		cs_port->OUTSET = cs_pin_mask;
		cs_port->DIRSET = cs_pin_mask;
	}
	usart = this;
	base_addr = port;
	this->cs_port = cs_port;
	this->cs_pin_mask = cs_pin_mask;
	//base_addr->CTRLB &= ~(USART_RXEN_bm | USART_TXEN_bm);
	if(c_mode == this->c_mode_mspi)
	{
		base_addr->BAUDCTRLA = (unsigned char)speed;
		base_addr->BAUDCTRLB = (unsigned char)((speed>>8) & 0x0F);
		base_addr->CTRLA = USART_RXCINTLVL_OFF_gc | USART_TXCINTLVL_OFF_gc | USART_DREINTLVL_OFF_gc;
		base_addr->CTRLC = USART_CMODE_MSPI_gc | (p_mode & USART_PMODE_gm) | (s_mode & USART_SBMODE_bm) | USART_CHSIZE_5BIT_gc;
		base_addr->CTRLB = (USART_RXEN_bm & (Rx_Enable << USART_RXEN_bp)) | (USART_TXEN_bm & (Tx_Enable << USART_TXEN_bp));
	}
	else
	{
		unsigned int ubrr = ((F_CPU / 8) / speed)-1;
		base_addr->BAUDCTRLA = (unsigned char)ubrr;
		base_addr->BAUDCTRLB = (unsigned char)((ubrr>>8) & 0x0F);
		base_addr->CTRLA = USART_RXCINTLVL_LO_gc | USART_TXCINTLVL_OFF_gc | USART_DREINTLVL_OFF_gc;
		base_addr->CTRLC = (USART_CMODE_gm & c_mode) | (p_mode & USART_PMODE_gm) | (s_mode & USART_SBMODE_bm) | USART_CHSIZE_8BIT_gc;
		base_addr->CTRLB = (USART_RXEN_bm & (Rx_Enable << USART_RXEN_bp)) | (USART_TXEN_bm & (Tx_Enable << USART_TXEN_bp)) | USART_CLK2X_bm;
	}
	return true;
}

void Io_Usart::service(Io_Usart *structure) 
{
	if(structure->uart_rx_timeout_cnt > 0) {
		structure->uart_rx_timeout_cnt--;
		} else if(structure->uart_rx_timeout_cnt == 0) {
		structure->rx_cnt = 0;
		structure->uart_rx_timeout_cnt--;
	}
}


bool Io_Usart::transmit(char data)
{
	volatile unsigned long timeout = UART_TX_TIMEOUT;
	/* Wait for empty transmit buffer */
	volatile USART_t* port = this->base_addr;
	while (!(port->STATUS & (USART_DREIF_bm))) {
		if (!timeout--)
			return false;
	}
	/* Put data into buffer, sends the data */
	port->DATA = data;
	return true;
}

bool Io_Usart::transmit_nb(char data)
{
	/* Wait for empty transmit buffer */
	volatile USART_t* port = this->base_addr;
	if(!( port->STATUS & (USART_DREIF_bm))) {
		return false;
	}
	/* Put data into buffer, sends the data */
	port->DATA = data;
	return true;
}

bool Io_Usart::txrx_byte(unsigned char *data)
{
	volatile USART_t* port = this->base_addr;
	port->DATA = *data;
	while((~port->STATUS) & USART_RXCIF_bm);
	*data = port->DATA;
	return true;
}

bool Io_Usart::txrx_buff(unsigned int BytesSend, unsigned int BytesReceive)
{
	if(!spim_buff || !this->cs_port || !this->cs_pin_mask)
		return false;
	if(control_cs_pin)
		cs_port->OUTCLR = this->cs_pin_mask;
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
		cs_port->OUTSET = this->cs_pin_mask;
	return true;
}

bool Io_Usart::rx_buff(unsigned int BytesReceive)
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
		unsigned char tmp_ff = 0xFF;
		dmatx->ADDR = (unsigned int)&tmp_ff;
		dmatx->ADDRCTRL = EDMA_CH_RELOAD_NONE_gc | EDMA_CH_DIR_FIXED_gc;
		dmatx->TRIGSRC = EDMA_CH_TRIGSRC_USARTC0_DRE_gc;

		dmarx->ADDR = (unsigned int)spim_buff;
		dmarx->ADDRCTRL = EDMA_CH_RELOAD_NONE_gc | EDMA_CH_DIR_INC_gc;
		dmarx->TRIGSRC = EDMA_CH_TRIGSRC_USARTC0_RXC_gc;
		unsigned int cnt = BytesReceive;
		for(; cnt >= 512; cnt -= 512)
		{
			dmarx->TRFCNT = 0;
			dmarx->CTRLA = EDMA_CH_SINGLE_bm | EDMA_CH_REPEAT_bm;
			dmarx->CTRLA |= EDMA_CH_ENABLE_bm/* | EDMA_CH_BURSTLEN_bm*/;
			dmatx->TRFCNT = 0;
			dmatx->CTRLA = EDMA_CH_SINGLE_bm | EDMA_CH_REPEAT_bm;
			dmatx->CTRLA |= EDMA_CH_ENABLE_bm/* | EDMA_CH_BURSTLEN_bm*/;
			while(!(dmarx->CTRLB & EDMA_CH_TRNIF_bm));
			dmarx->CTRLB |= EDMA_CH_TRNIF_bm;
			dmarx->CTRLB |= EDMA_CH_TRNIF_bm;
		}
		if(cnt)
		{
			dmarx->TRFCNT = cnt;
			dmarx->CTRLA = EDMA_CH_SINGLE_bm | EDMA_CH_REPEAT_bm;
			dmarx->CTRLA |= EDMA_CH_ENABLE_bm/* | EDMA_CH_BURSTLEN_bm*/;
			dmatx->TRFCNT = cnt;
			dmatx->CTRLA = EDMA_CH_SINGLE_bm | EDMA_CH_REPEAT_bm;
			dmatx->CTRLA |= EDMA_CH_ENABLE_bm/* | EDMA_CH_BURSTLEN_bm*/;
			while(!(dmarx->CTRLB & EDMA_CH_TRNIF_bm));
			dmarx->CTRLB |= EDMA_CH_TRNIF_bm;
			dmarx->CTRLB |= EDMA_CH_TRNIF_bm;
		}
	}
	if(control_cs_pin)
		cs_port->OUTSET = this->cs_pin_mask;
	return true;
}

bool Io_Usart::tx_buff(unsigned int BytesSend)
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
		dmatx->ADDR = (unsigned int)spim_buff;
		dmatx->ADDRCTRL = EDMA_CH_RELOAD_NONE_gc | EDMA_CH_DIR_INC_gc;
		dmatx->TRIGSRC = EDMA_CH_TRIGSRC_USARTC0_DRE_gc;
		unsigned int cnt = BytesSend;
		for(; cnt >= 512; cnt -= 512)
		{
			dmatx->TRFCNT = 0;
			dmatx->CTRLA = EDMA_CH_SINGLE_bm | EDMA_CH_REPEAT_bm;
			dmatx->CTRLA |= EDMA_CH_ENABLE_bm/* | EDMA_CH_BURSTLEN_bm*/;
			while(!(dmatx->CTRLB & EDMA_CH_TRNIF_bm));
			dmatx->CTRLB |= EDMA_CH_TRNIF_bm;
		}
		if(cnt)
		{
			dmatx->TRFCNT = cnt;
			dmatx->CTRLA = EDMA_CH_SINGLE_bm | EDMA_CH_REPEAT_bm;
			dmatx->CTRLA |= EDMA_CH_ENABLE_bm/* | EDMA_CH_BURSTLEN_bm*/;
			while(!(dmatx->CTRLB & EDMA_CH_TRNIF_bm));
			dmatx->CTRLB |= EDMA_CH_TRNIF_bm;
		}
	}
	if(control_cs_pin)
	cs_port->OUTSET = this->cs_pin_mask;
	return true;
}

void Io_Usart::set_baud(unsigned int speed)
{
	base_addr->BAUDCTRLA = (unsigned char)speed;
	base_addr->BAUDCTRLB = (unsigned char)((speed>>8) & 0x0F);
}

void Io_Usart::cs_assert()
{
	cs_port->OUTCLR = cs_pin_mask;
}

void Io_Usart::cs_deasert()
{
	cs_port->OUTSET = cs_pin_mask;
}

unsigned char Io_Usart::cs_read()
{
	return cs_port->IN & cs_pin_mask;
}

char Io_Usart::receive(void)
{
	/* Wait for data to be received */
	volatile USART_t* port = this->base_addr;
	while ((port->STATUS & (USART_RXCIF_bm)) == 0)
	{}
	/* Get and return received data from buffer */
	return port->DATA;
}

bool Io_Usart::receive_nb(char *data)
{
	/* Wait for data to be received */
	volatile USART_t* port = this->base_addr;
	if ((port->STATUS & (USART_RXCIF_bm)) == 0)
		return false;
	/* Get and return received data from buffer */
	*data = port->DATA;
	return true;
}

void Io_Usart::Put_Str(const char *data/*, unsigned char len*/)
{
	//unsigned int cnt = 0;
	while(*data)
	{
		//if(/*len == -1 && */*data == 0)
		//	break;
		transmit((unsigned char)*data++);
	}
}

/*void Io_Usart::Put_Hex_Char(unsigned char data)
{
	char buff[4];
	utoa(data, buff, 16);
	Put_Str(buff);
}*/

void Io_Usart::Put_Hex_Char(unsigned char data)
{
	unsigned char ch1 = data >> 4;
	unsigned char ch2 = data & 0x0F;
	if(ch1 < 10)
		transmit(ch1 + '0');
	else
		transmit((ch1 - 10) + 'A');
	if(ch2 < 10)
		transmit(ch2 + '0');
	else
		transmit((ch2 - 10) + 'A');
}


void  Io_Usart::Put_Hex_Buff(const unsigned char *data, unsigned int len)
{
	unsigned int cnt = 0;
	for(; cnt < len; cnt++)
	{
		this->Put_Hex_Char(data[cnt]);
		//transmit('.');
	}
}

unsigned int Io_Usart::write(const char *pcBuf, unsigned int len)
{
    unsigned int uIdx = 0;
	/* Send the characters */
	for(uIdx = 0; uIdx < len; uIdx++)
	{
		/* Send the character to the UART output. */
	   transmit(pcBuf[uIdx]);
	}
    /* Return the number of characters written. */
    return(uIdx);
}

/**
 * A simple UART based printf function supporting \%c, \%d, \%p, \%s, \%u,
 * \%x, and \%X.
 *
 * \param pcString is the format string.
 * \param ... are the optional arguments, which depend on the contents of the
 * format string.
 *
 * This function is very similar to the C library <tt>fprintf()</tt> function.
 * All of its output will be sent to the UART.  Only the following formatting
 * characters are supported:
 *
 * - \%c to print a character
 * - \%d to print a decimal value
 * - \%s to print a string
 * - \%u to print an unsigned decimal value
 * - \%x to print a hexadecimal value using lower case letters
 * - \%X to print a hexadecimal value using lower case letters (not upper case
 * letters as would typically be used)
 * - \%p to print a pointer as a hexadecimal value
 * - \%\% to print out a \% character
 *
 * For \%s, \%d, \%u, \%p, \%x, and \%X, an optional number may reside
 * between the \% and the format character, which specifies the minimum number
 * of characters to use for that value; if preceded by a 0 then the extra
 * characters will be filled with zeros instead of spaces.  For example,
 * ``\%8d'' will use eight characters to print the decimal value with spaces
 * added to reach eight; ``\%08d'' will use eight characters as well but will
 * add zeroes instead of spaces.
 *
 * The type of the arguments after \e pcString must match the requirements of
 * the format string.  For example, if an integer was passed where a string
 * was expected, an error of some kind will most likely occur.
 *
 * \return None.
 */
unsigned int Io_Usart::print(const char *pcString, ...) {
    unsigned long idx, pos, count, base, neg;
    char *pcStr, pcBuf[16], cFill;
    va_list vaArgP;
    unsigned long _value;
	unsigned int uIdx = 0;

    /* Start the varargs processing. */
    va_start(vaArgP, pcString);

    /* Loop while there are more characters in the string. */
    while(*pcString)
    {
        /* Find the first non-% character, or the end of the string. */
        for(idx = 0; (pcString[idx] != '%') && (pcString[idx] != '\0'); idx++)
        {
        }

        /* Write this portion of the string. */
        if(!(uIdx += write(pcString, idx)))
			return 0;

        /* Skip the portion of the string that was written. */
        pcString += idx;

        /* See if the next character is a %. */
        if(*pcString == '%')
        {
            /* Skip the %. */
            pcString++;

            /* Set the digit count to zero, and the fill character to space
             * (i.e. to the defaults). */
            count = 0;
            cFill = ' ';

            /* It may be necessary to get back here to process more characters.
             * Goto's aren't pretty, but effective.  I feel extremely dirty for
             * using not one but two of the beasts. */
again:

            /* Determine how to handle the next character. */
            switch(*pcString++)
            {
                /* Handle the digit characters. */
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    /* If this is a zero, and it is the first digit, then the
                     * fill character is a zero instead of a space. */
                    if((pcString[-1] == '0') && (count == 0))
                    {
                        cFill = '0';
                    }

                    /* Update the digit count. */
                    count *= 10;
                    count += pcString[-1] - '0';

                    /* Get the next character. */
                    goto again;
                }

                /* Handle the %c command. */
                case 'c':
                {
                    /* Get the value from the varargs. */
                    _value = va_arg(vaArgP, unsigned int);

                    /* Print out the character. */
                    if(!(uIdx += write((char *)&_value, 1)))
						return 0;

                    /* This command has been handled. */
                    break;
                }

                /* Handle the %d command. */
                case 'd':
                {
                    /* Get the value from the varargs. */
                    _value = va_arg(vaArgP, signed long);

                    /* Reset the buffer position. */
                    pos = 0;

                    /* If the value is negative, make it positive and indicate
                     * that a minus sign is needed. */
                    if((long)_value < 0)
                    {
                        /* Make the value positive. */
                        _value = -(long)_value;

                        /* Indicate that the value is negative. */
                        neg = 1;
                    }
                    else
                    {
                        /* Indicate that the value is positive so that a minus
                         * sign isn't inserted. */
                        neg = 0;
                    }

                    /* Set the base to 10. */
                    base = 10;

                    /* Convert the value to ASCII. */
                    goto convert;
                }

                /* Handle the %s command. */
                case 's':
                {
                    /* Get the string pointer from the varargs. */
                    pcStr = va_arg(vaArgP, char *);

                    /* Determine the length of the string. */
                    for(idx = 0; pcStr[idx] != '\0'; idx++)
                    {
                    }

                    /* Write the string. */
                    if(!(uIdx += write(pcStr, idx)))
						return 0;

                    /* Write any required padding spaces */
                    if(count > idx)
                    {
                        count -= idx;
                        while(count--)
                        {
                            if(!(uIdx += write((const char *)" ", 1)))
								return 0;
                        }
                    }
                    /* This command has been handled. */
                    break;
                }

                /* Handle the %u command. */
                case 'u':
                {
                    /* Get the value from the varargs. */
                    _value = va_arg(vaArgP, unsigned long);

                    /* Reset the buffer position. */
                    pos = 0;

                    /* Set the base to 10. */
                    base = 10;

                    /* Indicate that the value is positive so that a minus sign
                     * isn't inserted. */
                    neg = 0;

                    /* Convert the value to ASCII. */
                    goto convert;
                }

                /* Handle the %x and %X commands.  Note that they are treated
                 * identically; i.e. %X will use lower case letters for a-f
                 * instead of the upper case letters is should use.  We also
                 * alias %p to %x. */
                case 'x':
                case 'X':
                case 'p':
                {
                    /* Get the value from the varargs. */
                    _value = va_arg(vaArgP, unsigned long);

                    /* Reset the buffer position. */
                    pos = 0;

                    /* Set the base to 16. */
                    base = 16;

                    /* Indicate that the value is positive so that a minus sign
                     * isn't inserted. */
                    neg = 0;

                    /* Determine the number of digits in the string version of
                     * the value. */
convert:
                    for(idx = 1;
                        (((idx * base) <= _value) &&
                         (((idx * base) / base) == idx));
                        idx *= base, count--)
                    {
                    }

                    /* If the value is negative, reduce the count of padding
                     * characters needed. */
                    if(neg)
                    {
                        count--;
                    }

                    /* If the value is negative and the value is padded with
                     * zeros, then place the minus sign before the padding. */
                    if(neg && (cFill == '0'))
                    {
                        /* Place the minus sign in the output buffer. */
                        pcBuf[pos++] = '-';

                        /* The minus sign has been placed, so turn off the
                         * negative flag. */
                        neg = 0;
                    }

                    /* Provide additional padding at the beginning of the
                     * string conversion if needed. */
                    if((count > 1) && (count < 16))
                    {
                        for(count--; count; count--)
                        {
                            pcBuf[pos++] = cFill;
                        }
                    }

                    /* If the value is negative, then place the minus sign
                     * before the number. */
                    if(neg)
                    {
                        /* Place the minus sign in the output buffer. */
                        pcBuf[pos++] = '-';
                    }

                    /* Convert the value into a string. */
                    for(; idx; idx /= base)
                    {
                        pcBuf[pos++] = pgm_read_byte(&g_pcHex[(_value / idx) % base]);
                    }

                    /* Write the string. */
                    if(!(uIdx += write(pcBuf, pos)))
						return 0;

                    /* This command has been handled. */
                    break;
                }

                /* Handle the %% command. */
                case '%':
                {
                    /* Simply write a single %. */
                    if(!(uIdx += write(pcString - 1, 1)))
						return 0;

                    /* This command has been handled. */
                    break;
                }

                /* Handle all other commands. */
                default:
                {
                    /* Indicate an error. */
                    //terminal_send(PSTR("ERROR"));
					if(!(uIdx += 5))
						return 0;

                    /* This command has been handled. */
                    break;
                }
            }
        }
    }

    /* End the varargs processing. */
    va_end(vaArgP);
	return uIdx;
}


void uart_int_handler(Io_Usart *data) 
{
	if(!data)
		return;
	//Io_Usart usart;
	char RxChr = data->base_addr->DATA;
	//SniffSmart.char_capture(NULL, RxChr);
	if(data->rx_char_receive_func)
		data->rx_char_receive_func(data->rx_char_receive_struct, RxChr);
	//if(RxChr == '\r') RxChr = 0;
	/*if (RxChr > 128)
	{
		data->rx_cnt = 0;
		return;
	}*/
	/*data->rx_buff[data->rx_cnt] = RxChr;
	data->uart_rx_timeout_cnt = UART_RX_TIMEOUT;
	if (RxChr == 0)
	{
		unsigned int rx_cnt = data->rx_cnt;
		data->rx_cnt = 0;
		if(data->rx_complete_func) 
			data->rx_complete_func(data->rx_complete_struct, data->rx_buff, rx_cnt);
		//terminal_parse(rx_buff);
		return;
	}
	data->rx_cnt++;
	if(data->rx_cnt > RxBuffLen) data->rx_cnt = 0;*/
}

#if defined(USARTC0) && defined(USE_UART0)
ISR(USARTC0_RXC_vect)
{
	if(io_usart0_service) 
		uart_int_handler(io_usart0_service);
}
#endif

#if defined(USARTC1) && defined(USE_UART1)
ISR(USARTC1_RXC_vect)
{
	if(io_usart1_service)
		uart_int_handler(io_usart1_service);
}
#endif

#if defined(USARTD0) && defined(USE_UART2)
ISR(USARTD0_RXC_vect)
{
	if(io_usart2_service)
		uart_int_handler(io_usart2_service);
}
#endif

#if defined(USARTD1) && defined(USE_UART3)
ISR(USARTD1_RXC_vect)
{
	if(io_usart3_service)
		uart_int_handler(io_usart3_service);
}
#endif

#if defined(USARTE0) && defined(USE_UART4)
ISR(USARTE0_RXC_vect)
{
	if(io_usart4_service)
		uart_int_handler(io_usart4_service);
}
#endif

#if defined(USARTE1) && defined(USE_UART5)
ISR(USARTE1_RXC_vect)
{
	if(io_usart5_service)
		uart_int_handler(io_usart5_service);
}
#endif

#if defined(USARTF0) && defined(USE_UART6)
ISR(USARTF0_RXC_vect)
{
	if(io_usart6_service)
		uart_int_handler(io_usart6_service);
}
#endif

#if defined(USARTF1) && defined(USE_UART7)
ISR(USARTF1_RXC_vect)
{
	if(io_usart7_service)
		uart_int_handler(io_usart7_service);
}
#endif


/*
 * dev_sd.cpp
 *
 * Created: 7/30/2016 11:05:19 PM
 *  Author: John Smith
 */ 

#include "config.h"
#include <util/delay.h>
#include "dev_mmcsd.h"
#include "io_spi.h"
#include "io_usart.h"

 //#######################################################################################
unsigned char SD_Spi::sd_io_data(unsigned char Value)
{
	unsigned char tmp = Value;
	if(io_spi_port)
		io_spi_port->txrx_byte(&tmp);//usart_spi_data((Uart_t *)Struct, Value);
	else if(io_usart_port)
		io_usart_port->txrx_byte(&tmp);//usart_spi_data((Uart_t *)Struct, Value);
	return tmp;
}
//#######################################################################################
/*static void sd_delay(uint16_t DelayMs)
{
	unsigned int cnt = 0;
	for(cnt = 0; cnt < 1000; cnt++);
	//Sysdelay(DelayMs);
}*/
//#######################################################################################
void SD_Spi::sd_cs_assert()
{
	if(io_spi_port)
		io_spi_port->cs_assert();
	else if(io_usart_port)
		io_usart_port->cs_assert();
}
//#######################################################################################
void SD_Spi::sd_cs_deassert()
{
	if(io_spi_port)
		io_spi_port->cs_deasert();
	else if(io_usart_port)
		io_usart_port->cs_deasert();
}
//#######################################################################################
unsigned char SD_Spi::sd_cs_readstate()
{
	if(io_spi_port)
		return io_spi_port->cs_read();
	else if(io_usart_port)
		return io_usart_port->cs_read();
	return 0;
}
//#######################################################################################
/*static uint8_t sd_read_idle_bytes(SD_Struct_t *SD_Struct, uint16_t BytesNr)
{
	uint8_t Temp = 255;
	for (uint16_t IdleBytesCount = 0; IdleBytesCount < BytesNr;IdleBytesCount++)
	{
		Temp = sd_io_data(SD_Struct,255);
		if(Temp != 255)
			break;
	}
	 return Temp;
}
//#######################################################################################
static void sd_idle_bytes(SD_Struct_t *SD_Struct, uint16_t BytesNr)
{
	for (uint16_t IdleBytesCount = 0; IdleBytesCount < BytesNr;IdleBytesCount++)
	{
		sd_io_data(SD_Struct,255);
	}
}*/
//#######################################################################################
/*-----------------------------------------------------------------------*/
/* Wait for card ready                                                   */
/*-----------------------------------------------------------------------*/
int SD_Spi::wait_ready (	/* 1:Ready, 0:Timeout */
	unsigned long wt			/* Timeout [ms] */
)
{
	unsigned char d;

	unsigned long Timer2 = wt;
	do {
		d = sd_io_data(0xFF);
		/* This loop takes a time. Insert rot_rdq() here for multitask envilonment. */
		//_delay_ms(1);
	} while (d != 0xFF && Timer2--);	/* Wait for card goes ready or timeout */

	return (d == 0xFF) ? 1 : 0;
}

/*-----------------------------------------------------------------------*/
/* Deselect card and release SPI                                         */
/*-----------------------------------------------------------------------*/

void SD_Spi::deselect ()
{
	sd_cs_deassert();		/* CS = H */
	sd_io_data(0xFF);	/* Dummy clock (force DO hi-z for multiple slave SPI) */
}



/*-----------------------------------------------------------------------*/
/* Select card and wait for ready                                        */
/*-----------------------------------------------------------------------*/

int SD_Spi::_select ()	/* 1:OK, 0:Timeout */
{
	sd_cs_assert();
	sd_io_data(0xFF);	/* Dummy clock (force DO enabled) */

	if (wait_ready(50000))
		return 1;	/* OK */
	deselect();
	return 0;	/* Timeout */
}
/*-----------------------------------------------------------------------*/
/* Send a command packet to the MMC                                      */
/*-----------------------------------------------------------------------*/

unsigned char SD_Spi::send_cmd (		/* Return value: R1 resp (bit7==1:Failed to send) */
	unsigned char cmd,		/* Command index */
	unsigned long arg		/* Argument */
)
{
	unsigned char n, res;


	if (cmd & 0x80)
	{	/* Send a CMD55 prior to ACMD<n> */
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);
		if (res > 1) 
			return res;
	}

	/* Select card */
	if (cmd != CMD12) 
	{
		deselect();
		if (!_select())
			return 0xFF;
	}

	/* Send command packet */
	sd_io_data(0x40 | cmd);				/* Start + command index */
	sd_io_data((unsigned char)(arg >> 24));		/* Argument[31..24] */
	sd_io_data((unsigned char)(arg >> 16));		/* Argument[23..16] */
	sd_io_data((unsigned char)(arg >> 8));			/* Argument[15..8] */
	sd_io_data((unsigned char)arg);				/* Argument[7..0] */
	n = 0x01;							/* Dummy CRC + Stop */
	if (cmd == CMD0)
		n = 0x95;			/* Valid CRC for CMD0(0) */
	if (cmd == CMD8)
		n = 0x87;			/* Valid CRC for CMD8(0x1AA) */
	sd_io_data(n);

	/* Receive command resp */
	if (cmd == CMD12)
		sd_io_data(0xFF);	/* Diacard following one byte when CMD12 */
	n = 255;								/* Wait for response (10 bytes max) */
	do
		res = sd_io_data(0xFF);
	while ((res & 0x80) && --n);

	return res;							/* Return received response */
}

bool SD_Spi::_mmcsd_spi_init()
{
	if(io_spi_port)
	{
		io_spi_port->set_baud(3);			/* Set slow clock */
	}
	else if(io_usart_port)
	{
		io_usart_port->set_baud(80);			/* Set slow clock */
	}
	sd_cs_deassert();
	unsigned char n, cmd, ty, ocr[4];
	SD_Hc = IsSd;
	for (n = 14; n; n--)
		sd_io_data(0xFF);	/* Send 80 dummy clocks */

	ty = 0;
	if (send_cmd(CMD0, 0) == 1)
	{			/* Put the card SPI/Idle state */
		unsigned long Timer1 = 100000;						/* Initialization timeout = 1 sec */
		if (send_cmd(CMD8, 0x1AA) == 1)
		{	/* SDv2? */
			for (n = 0; n < 4; n++) 
				ocr[n] = sd_io_data(0xFF);	/* Get 32 bit return value of R7 resp */
			if (ocr[2] == 0x01 && ocr[3] == 0xAA)
			{				/* Is the card supports vcc of 2.7-3.6V? */
				while (Timer1-- && send_cmd(ACMD41, 1UL << 30)) ;	/* Wait for end of initialization with ACMD41(HCS) */
				if (Timer1 && send_cmd(CMD58, 0) == 0)
				{		/* Check CCS bit in the OCR */
					for (n = 0; n < 4; n++)
						ocr[n] = sd_io_data(0xFF);
					ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;	/* Card id SDv2 */
					if(ty & CT_BLOCK)
						SD_Hc = IsSdhc;
				}
			}
		} else {	/* Not SDv2 card */
			if (send_cmd(ACMD41, 0) <= 1)
			{	/* SDv1 or MMC? */
				ty = CT_SD1; cmd = ACMD41;	/* SDv1 (ACMD41(0)) */
			} else
			{
				ty = CT_MMC; cmd = CMD1;	/* MMCv3 (CMD1(0)) */
			}
			while (Timer1-- && send_cmd(cmd, 0)) ;		/* Wait for end of initialization */
			if (!Timer1 || send_cmd(CMD16, 512) != 0)	/* Set block length: 512 */
				ty = 0;
		}
	}
	deselect();

	if (ty)
	{			/* OK */
		if(io_spi_port)
		{
			io_spi_port->set_baud(1);			/* Set fast clock */
		}
		else if(io_usart_port)
		{
			io_usart_port->set_baud(1);			/* Set fast clock */
		}
		SD_Init_OK = true;
	} else
	{			/* Failed */
		//power_off();
		SD_Init_OK = false;
	}

	//return Stat;

	//SD_Struct->HardUnitSetBaudFunc((void *)SD_Struct->HardUnitStruct, 4000000);
	sd_cs_deassert();
	return SD_Init_OK;
}

//#######################################################################################
bool SD_Spi::rcvr_datablock(unsigned char *buff, unsigned int bytes_to_read)
{
	unsigned char token;
	//unsigned char Timer1 = 10;
	do
	{							/* Wait for data packet in timeout of 100ms */
		token = sd_io_data(255);
	} while ((token == 0xFF)/* && Timer1--*/);

	if(token != MMC_DATA_TOKEN)
		return false;	/* If not valid data token, return with error */

	//do
	//{							/* Receive the data block into buffer */
	//	*buff++ = sd_io_data(255);
	//} while (--bytes_to_read);
	if(io_spi_port)
	{
		io_spi_port->spim_buff = buff;
		io_spi_port->rx_buff(bytes_to_read);
	}
	else if(io_usart_port)
	{
		io_usart_port->spim_buff = buff;
		io_usart_port->rx_buff(bytes_to_read);
	}

	sd_io_data(255);						/* Discard CRC */
	sd_io_data(255);

	return true;					/* Return with success */
}
//#######################################################################################
unsigned int SD_Spi::_sd_read_page(unsigned char *_Buffer, unsigned long block, unsigned int nblks)
{
	if (/*drv || */!nblks) 
		return false;		/* Check parameter */
	if (SD_Init_OK == false) 
		return false;	/* Check if drive is ready */

	if (SD_Hc == IsSd)
		block *= 512;	/* LBA ot BA conversion (byte addressing cards) */

	if (nblks == 1)
	{	/* Single sector read */
		if ((send_cmd(CMD17, block) == 0)	/* READ_SINGLE_BLOCK */
			&& rcvr_datablock(_Buffer, 512))
			nblks = 0;
	}
	else
	{				/* Multiple sector read */
		if (send_cmd(CMD18, block) == 0)
		{	/* READ_MULTIPLE_BLOCK */
			do
			{
				if (!rcvr_datablock(_Buffer, 512)) break;
				_Buffer += 512;
			} while (--block);
			send_cmd(CMD12, 0);				/* STOP_TRANSMISSION */
		}
	}
	deselect();
	return nblks ? false : true;	/* Return result */
}
//#######################################################################################
unsigned int SD_Spi::read(unsigned char *_Buffer, unsigned long _block, unsigned int nblks)
{
	unsigned long block = _block;
	unsigned char* Buffer = (unsigned char*)_Buffer;
	//unsigned int TransferedBlocks = 0;
	do
	{
		if(!_sd_read_page(Buffer/* + (512*TransferedBlocks)*/, block++, 1)) return false;
		Buffer += 512;
		//TransferedBlocks++;
	} while (--nblks);
	return true;
}
//#######################################################################################
/* Send multiple byte */
void SD_Spi::xmit_spi_multi (
	unsigned char *buff,	/* Pointer to the data */
	unsigned int btx			/* Number of bytes to send (512) */
)
{
	//do {						/* Transmit data block */
	//	sd_io_data(*buff++);
	//} while (btx -= 1);
	if(io_spi_port)
	{
		io_spi_port->spim_buff = buff;
		io_spi_port->tx_buff(btx);
	}
	else if(io_usart_port)
	{
		io_usart_port->spim_buff = buff;
		io_usart_port->tx_buff(btx);
	}
}
/*-----------------------------------------------------------------------*/
/* Send a data packet to the MMC                                         */
/*-----------------------------------------------------------------------*/

int SD_Spi::xmit_datablock (	/* 1:OK, 0:Failed */
	unsigned char *buff,	/* Ponter to 512 byte data to be sent */
	unsigned char token			/* Token */
)
{
	unsigned char resp;


	if (!wait_ready(500)) 
		return 0;		/* Wait for card ready */

	sd_io_data(token);					/* Send token */
	if (token != 0xFD) {				/* Send data if token is other than StopTran */
		xmit_spi_multi(buff, 512);		/* Data */
		sd_io_data(0xFF);
		sd_io_data(0xFF);	/* Dummy CRC */

		resp = sd_io_data(0xFF);				/* Receive data resp */
		if ((resp & 0x1F) != 0x05)		/* Function fails if the data packet was not accepted */
			return 0;
	}
	return 1;
}


unsigned int SD_Spi::_sd_write_page(unsigned char *_Buffer, unsigned long block, unsigned int nblks)
{
	if (/*drv || */!nblks) 
		return false;		/* Check parameter */
	if (SD_Init_OK == false) 
		return false;	/* Check if drive is ready */

	if (SD_Hc == IsSd)
		block *= 512;	/* LBA ot BA conversion (byte addressing cards) */

	if (nblks == 1)
	{	/* Single sector write */
		if ((send_cmd(CMD24, block) == 0)	/* WRITE_BLOCK */
			&& xmit_datablock(_Buffer, 0xFE))
			nblks = 0;
	}
	else {				/* Multiple sector write */
		if (SD_Hc == IsSd)
			send_cmd(ACMD23, nblks);	/* Predefine number of sectors */
		if (send_cmd(CMD25, block) == 0)
		{	/* WRITE_MULTIPLE_BLOCK */
			do
			{
				if (!xmit_datablock(_Buffer, 0xFC)) break;
				_Buffer += 512;
			} while (--nblks);
			if (!xmit_datablock(0, 0xFD))	/* STOP_TRAN token */
				nblks = 1;
		}
	}
	deselect();
	return nblks ? false : true;	/* Return result */
}
//#######################################################################################
unsigned int SD_Spi::write(unsigned char *_Buffer, unsigned long _block, unsigned int nblks)
{
	unsigned long block = _block;
	unsigned char* Buffer = (unsigned char*)_Buffer;
	//unsigned int TransferedBlocks = 0;
	do
	{
		if(!_sd_write_page(Buffer/* + (512*TransferedBlocks)*/, block++, 1)) return false;
		Buffer += 512;
		//TransferedBlocks++;
	} while (--nblks);
	return true;
}
//#######################################################################################
SD_Spi::SD_Spi(Io_Spi *io_port, Io_Usart *io_usart_port)
{
	this->io_spi_port = io_port;
	this->io_usart_port = io_usart_port;
	_mmcsd_spi_init();
	return;
}
//#######################################################################################

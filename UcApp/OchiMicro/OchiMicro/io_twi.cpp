/*
 * io_twi.cpp
 *
 * Created: 11/18/2015 11:08:55 PM
 *  Author: John Smith
 */ 

#include "config.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "main.h"
#include "stdlib.h"
#include "io_twi.h"

#if defined(USE_TWI0) && defined (TWIC_TWIM_vect)
Io_Twi* io_twi0_service;
#endif
#if defined(USE_TWI1) && defined (TWID_TWIM_vect)
Io_Twi* io_twi1_service;
#endif
#if defined(USE_TWI2) && defined (TWIE_TWIM_vect)
Io_Twi* io_twi2_service;
#endif
#if defined(USE_TWI3) && defined (TWIF_TWIM_vect)
Io_Twi* io_twi3_service;
#endif


void Io_Twi::Reinit()
{
	volatile TWI_t* port = this->base_addr;
	port->MASTER.CTRLA &= (~TWI_MASTER_ENABLE_bm);
#if defined(USE_TWI0) && defined (TWIC_TWIM_vect)
	if((int)port == (int)&TWIC)
	{
		PORTC.DIRSET = 1<<0 | 1<<1;
		PORTC.OUTCLR = 1<<0 | 1<<1;
		_delay_us(100);
		PORTC.DIRCLR = 1<<0 | 1<<1;
		PORTC.PIN0CTRL = PORT_OPC_PULLUP_gc;
		PORTC.PIN1CTRL = PORT_OPC_PULLUP_gc;
	}
#endif
#if defined(USE_TWI1) && defined (TWID_TWIM_vect)
	if((int)port == (int)&TWIC)
	{
		PORTD.DIRSET = 1<<0 | 1<<1;
		PORTD.OUTCLR = 1<<0 | 1<<1;
		_delay_us(100);
		PORTD.DIRCLR = 1<<0 | 1<<1;
		PORTD.PIN0CTRL = PORT_OPC_PULLUP_gc;
		PORTD.PIN1CTRL = PORT_OPC_PULLUP_gc;
	}
#endif
#if defined(USE_TWI2) && defined (TWIE_TWIM_vect)
	if((int)port == (int)&TWIC)
	{
		PORTE.DIRSET = 1<<0 | 1<<1;
		PORTE.OUTCLR = 1<<0 | 1<<1;
		_delay_us(100);
		PORTE.DIRCLR = 1<<0 | 1<<1;
		PORTE.PIN0CTRL = PORT_OPC_PULLUP_gc;
		PORTE.PIN1CTRL = PORT_OPC_PULLUP_gc;
	}
#endif
#if defined(USE_TWI3) && defined (TWIF_TWIM_vect)
	if((int)port == (int)&TWIC)
	{
		PORTF.DIRSET = 1<<0 | 1<<1;
		PORTF.OUTCLR = 1<<0 | 1<<1;
		_delay_us(100);
		PORTF.DIRCLR = 1<<0 | 1<<1;
		PORTF.PIN0CTRL = PORT_OPC_PULLUP_gc;
		PORTF.PIN1CTRL = PORT_OPC_PULLUP_gc;
	}
#endif
	port->MASTER.BAUD = (FCPU/(100000<<1))-5;
	unsigned char Temp = port->MASTER.CTRLA;
	Temp = (Temp&(~TWI_MASTER_INTLVL_gm))|(TWI_MASTER_INTLVL_LO_gc);
	Temp = (Temp&(~TWI_MASTER_RIEN_bm))|(1<<TWI_MASTER_RIEN_bp);
	Temp = (Temp&(~TWI_MASTER_WIEN_bm))|(1<<TWI_MASTER_WIEN_bp);
	Temp = (Temp&(~TWI_MASTER_ENABLE_bm))|(1<<TWI_MASTER_ENABLE_bp);
	port->MASTER.CTRLA = Temp;
	port->MASTER.CTRLB |= TWI_MASTER_TIMEOUT_gm;
	port->MASTER.STATUS = TWI_MASTER_BUSSTATE_IDLE_gc;
	this->RxBuffSize = 16;
	this->TxBuffSize = 16;
	this->RxBuff = (unsigned char *)malloc(16);
	this->TxBuff = (unsigned char *)malloc(16);
}

Io_Twi::Io_Twi(volatile TWI_t* port)
{
	this->base_addr = port;
	io_twi0_service = this;
	this->twi = this;
	Reinit();
}

Io_Twi::~Io_Twi()
{
	
}

/*bool twi_reset(void)
{
	TWIC.MASTER.CTRLA &= ~TWI_MASTER_ENABLE_bm;
	_delay_us(10);
	TWIC.MASTER.CTRLA |= TWI_MASTER_ENABLE_bm;
	return true;
}*/


//#####################################################
/*! \brief TWI master arbitration lost and bus error interrupt handler.
 *
 *  Handles TWI responses to lost arbitration and bus error.
 *
 *  \param twi  The TWI_Master_t struct instance.
 */
void Io_Twi::MasterTransactionFinished(Io_Twi *twi, unsigned char result)
{
	twi->result = result;
	twi->status = TWIM_STATUS_READY;
}

void Io_Twi::MasterWriteHandler(Io_Twi *twi)
{
	TWI_t *ModuleAddr = (TWI_t *)twi->base_addr;
	/* Local variables used in if tests to avoid compiler warning. */
	unsigned char bytesToWrite  = twi->bytesToWrite;
	unsigned char bytesToRead   = twi->bytesToRead;

	/* If NOT acknowledged (NACK) by slave cancel the transaction. */
	/*if (ModuleAddr->MASTER.STATUS & TWI_MASTER_RXACK_bm) {
		ModuleAddr->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
		twi->result = TWIM_RESULT_NACK_RECEIVED;
		twi->status = TWIM_STATUS_READY;
	}*/

	/* If more bytes to write, send data. */
	/*else */if (twi->tCount < bytesToWrite) {
		unsigned char data = twi->TxBuff[twi->tCount];
		ModuleAddr->MASTER.DATA = data;
		++twi->tCount;
	}

	/* If bytes to read, send repeated START condition + Address +
	 * 'R/_W = 1'
	 */
	else if (twi->rCount < bytesToRead) {
		unsigned char readAddress = (twi->MasterSlaveAddr<<1) | 0x01;
		ModuleAddr->MASTER.ADDR = readAddress;
	}

	/* If transaction finished, send STOP condition and set RESULT OK. */
	else {
		ModuleAddr->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
		MasterTransactionFinished(twi, TWIM_RESULT_OK);
	}
}

/*! \brief TWI master read interrupt handler.
 *
 *  This is the master read interrupt handler that takes care of
 *  reading bytes from the TWI slave.
 *
 *  \param twi The TWI_Master_t struct instance.
 */
void Io_Twi::MasterReadHandler(Io_Twi *twi)
{
	TWI_t *ModuleAddr = (TWI_t *)twi->base_addr;
	/* Fetch data if bytes to be read. */
	if (twi->rCount < twi->RxBuffSize) {
		unsigned char data = ModuleAddr->MASTER.DATA;
		twi->RxBuff[twi->rCount] = data;
		twi->rCount++;
	}

	/* If buffer overflow, issue STOP and BUFFER_OVERFLOW condition. */
	else {
		ModuleAddr->MASTER.CTRLC = TWI_MASTER_CMD_STOP_gc;
		MasterTransactionFinished(twi, TWIM_RESULT_BUFFER_OVERFLOW);
	}

	/* Local variable used in if test to avoid compiler warning. */
	unsigned char bytesToRead = twi->bytesToRead;

	/* If more bytes to read, issue ACK and start a byte read. */
	if (twi->rCount < bytesToRead) {
		ModuleAddr->MASTER.CTRLC = TWI_MASTER_CMD_RECVTRANS_gc;
	}

	/* If transaction finished, issue NACK and STOP condition. */
	else {
		ModuleAddr->MASTER.CTRLC = TWI_MASTER_ACKACT_bm |
		                               TWI_MASTER_CMD_STOP_gc;
		MasterTransactionFinished(twi, TWIM_RESULT_OK);
	}
}

void Io_Twi::MasterArbitrationLostBusErrorHandler(Io_Twi *twi)
{
	TWI_t *ModuleAddr = (TWI_t *)twi->base_addr;
	unsigned char currentStatus = ModuleAddr->MASTER.STATUS;

	/* If bus error. */
	if (currentStatus & TWI_MASTER_BUSERR_bm) {
		twi->result = TWIM_RESULT_BUS_ERROR;
	}
	/* If arbitration lost. */
	else {
		twi->result = TWIM_RESULT_ARBITRATION_LOST;
	}

	/* Clear interrupt flag. */
	ModuleAddr->MASTER.STATUS = currentStatus | TWI_MASTER_ARBLOST_bm;

	twi->status = TWIM_STATUS_READY;
}

void Io_Twi::MasterInterruptHandler(Io_Twi *twi)
{
	TWI_t *ModuleAddr = (TWI_t *)twi->base_addr;
	unsigned char currentStatus = ModuleAddr->MASTER.STATUS;

	/* If arbitration lost or bus error. */
	if ((currentStatus & TWI_MASTER_ARBLOST_bm) ||
	    (currentStatus & TWI_MASTER_BUSERR_bm)) {

		MasterArbitrationLostBusErrorHandler(twi);
	}

	/* If master write interrupt. */
	else if (currentStatus & TWI_MASTER_WIF_bm) {
		MasterWriteHandler(twi);
	}

	/* If master read interrupt. */
	else if (currentStatus & TWI_MASTER_RIF_bm) {
		MasterReadHandler(twi);
	}

	/* If unexpected state. */
	else {
		MasterTransactionFinished(twi, TWIM_RESULT_FAIL);
	}
}
//#####################################################
//#####################################################
//############ Interrupt vectors insertion ############
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
//#####################################################
#if defined(USE_TWI0) && defined (TWIC_TWIM_vect)
		ISR(TWIC_TWIM_vect)
		{
			io_twi0_service->MasterInterruptHandler(io_twi0_service);
		}
 		/*/*ISR(TWIC_TWIS_vect)
 		{
 			TWI_SlaveInterruptHandler(Twi_Int_Service_t.TWI0RamServiceAddr);
 		}*/
#endif
//-----------------------------------------------------
#if defined(USE_TWI1) && defined (TWID_TWIM_vect)
		ISR(TWID_TWIM_vect)
		{
			io_twi1_service->MasterInterruptHandler(io_twi1_service);
		}
		/*ISR(TWID_TWIS_vect)
		{
			TWI_SlaveInterruptHandler(Twi_Int_Service_t.TWI1RamServiceAddr);
		}*/
#endif
//-----------------------------------------------------
#if defined(USE_TWI2) && defined (TWIE_TWIM_vect)
		ISR(TWIE_TWIM_vect)
		{
			io_twi2_service->MasterInterruptHandler(io_twi2_service);
		}
		/*ISR(TWIE_TWIS_vect)
		{
			TWI_SlaveInterruptHandler(Twi_Int_Service_t.TWI2RamServiceAddr);
		}*/
#endif
//-----------------------------------------------------
#if defined(USE_TWI3) && defined (TWIF_TWIM_vect)
		ISR(TWIF_TWIM_vect)
		{
			io_twi3_service->MasterInterruptHandler(io_twi3_service);
		}
		/*ISR(TWIF_TWIS_vect)
		{
			TWI_SlaveInterruptHandler(Twi_Int_Service_t.TWI3RamServiceAddr);
		}*/
#endif
//#####################################################
//#####################################################
/*! \brief TWI write and/or read transaction.
 *
 *  This function is a TWI Master write and/or read transaction. The function
 *  can be used to both write and/or read bytes to/from the TWI Slave in one
 *  transaction.
 *
 *  \param twi            The TWI_Master_t struct instance.
 *  \param address        The slave address.
 *  \param writeData      Pointer to data to write.
 *  \param bytesToWrite   Number of bytes to write.
 *  \param bytesToRead    Number of bytes to read.
 *
 *  \retval true  If transaction could be started.
 *  \retval false If transaction could not be started.
 */
bool Io_Twi::MasterWriteRead(Io_Twi *twi, unsigned int bytesToWrite, unsigned int bytesToRead)
{
	/*Parameter sanity check. */
	if (bytesToWrite > twi->TxBuffSize) 
	{
		return false;
	}
	if (bytesToRead > twi->RxBuffSize) 
	{
		return false;
	}

	/*Initiate transaction if bus is ready. */
	if (twi->status == TWIM_STATUS_READY) 
	{

		twi->status = TWIM_STATUS_BUSY;
		twi->result = TWIM_RESULT_UNKNOWN;

		//twi->DeviceAddress = DeviceAddress<<1;

		/* Fill write data buffer. */
		//memcpy(twi->, DataToWrite, bytesToWrite);
		//for (uint8_t bufferIndex=0; bufferIndex < bytesToWrite; bufferIndex++) 
		//{
		//	twi->WriteBuff[bufferIndex] = DataToWrite[bufferIndex];
		//}

		twi->bytesToWrite = bytesToWrite;
		twi->bytesToRead = bytesToRead;
		twi->tCount = 0;
		twi->rCount = 0;

		/* If write command, send the START condition + Address +
		 * 'R/_W = 0'
		 */
		if (twi->bytesToWrite > 0) {
			uint8_t writeAddress = (twi->MasterSlaveAddr<<1) & ~0x01;
			this->base_addr->MASTER.ADDR = writeAddress;
		}

		/* If read command, send the START condition + Address +
		 * 'R/_W = 1'
		 */
		else if (twi->bytesToRead > 0) {
			uint8_t readAddress = (twi->MasterSlaveAddr<<1) | 0x01;
			this->base_addr->MASTER.ADDR = readAddress;
		}

		unsigned int timeout = 1000;
		while(twi->status != TWIM_STATUS_READY && --timeout)
		{
			_delay_us(100);
			asm("WDR");
		}
		if(timeout == 0) 
			reset(twi);
	if(twi->result != TWIM_RESULT_OK || timeout == 0) 
		return false;
	return true;
	} 
	else 
	{
		return false;
	}
}
//#####################################################
bool Io_Twi::reset(Io_Twi* twi)
{
	//TWI_t* Return = Twi_GetSetBaseAddress(TwiStruct);
	twi->base_addr->MASTER.CTRLA &= ~TWI_MASTER_ENABLE_bm;
	_delay_us(10);
	twi->base_addr->MASTER.CTRLA |= TWI_MASTER_ENABLE_bm;
	return true;
}

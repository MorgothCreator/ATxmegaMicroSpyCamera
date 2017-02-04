/*
 * io_twi.h
 *
 * Created: 11/18/2015 11:08:43 PM
 *  Author: John Smith
 */ 


#ifndef IO_TWI_H_
#define IO_TWI_H_

/*#####################################################*/
/*! Transaction status defines. */
#define TWIM_STATUS_READY              0
#define TWIM_STATUS_BUSY               1
//#####################################################
//#define TWIM_WRITE_BUFFER_SIZE         8
//#define TWIM_READ_BUFFER_SIZE          8

typedef enum TWIM_RESULT_enum {
	TWIM_RESULT_UNKNOWN          = (0x00<<0),
	TWIM_RESULT_OK               = (0x01<<0),
	TWIM_RESULT_BUFFER_OVERFLOW  = (0x02<<0),
	TWIM_RESULT_ARBITRATION_LOST = (0x03<<0),
	TWIM_RESULT_BUS_ERROR        = (0x04<<0),
	TWIM_RESULT_NACK_RECEIVED    = (0x05<<0),
	TWIM_RESULT_FAIL             = (0x06<<0),
} TWIM_RESULT_t;
/*#####################################################*/

class Io_Twi
{
public:
	Io_Twi(volatile TWI_t* port);
	~Io_Twi();
	void Reinit(void);
	bool MasterWriteRead(Io_Twi* structure, unsigned int bytesToWrite, unsigned int bytesToRead);
	bool reset(Io_Twi* twi);
	void MasterTransactionFinished(Io_Twi* twi, unsigned char result);
	void MasterWriteHandler(Io_Twi* twi);
	void MasterReadHandler(Io_Twi* twi);
	void MasterArbitrationLostBusErrorHandler(Io_Twi* twi);
	void MasterInterruptHandler(Io_Twi* twi);


	volatile TWI_t *base_addr;
	Io_Twi *twi;
	unsigned char MasterSlaveAddr;
	unsigned int tCount;
	unsigned int rCount;
	volatile unsigned char status;
	volatile unsigned char result;
	unsigned int bytesToWrite;
	unsigned int bytesToRead;
	unsigned int RxBuffSize;
	unsigned int TxBuffSize;
	unsigned char* RxBuff;
	unsigned char* TxBuff;
protected:
private:
};



#endif /* IO_TWI_H_ */
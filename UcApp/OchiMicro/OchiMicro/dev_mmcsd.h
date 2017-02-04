/*
 * dev_sd.h
 *
 * Created: 7/30/2016 11:05:07 PM
 *  Author: John Smith
 */ 


#ifndef DEV_SD_H_
#define DEV_SD_H_

 #include "io_spi.h"
 #include "io_usart.h"

/* MMC/SD command */
#define CMD0	(0)			/* GO_IDLE_STATE */
#define CMD1	(1)			/* SEND_OP_COND (MMC) */
#define	ACMD41	(0x80+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(8)			/* SEND_IF_COND */
#define CMD9	(9)			/* SEND_CSD */
#define CMD10	(10)		/* SEND_CID */
#define CMD12	(12)		/* STOP_TRANSMISSION */
#define ACMD13	(0x80+13)	/* SD_STATUS (SDC) */
#define CMD16	(16)		/* SET_BLOCKLEN */
#define CMD17	(17)		/* READ_SINGLE_BLOCK */
#define CMD18	(18)		/* READ_MULTIPLE_BLOCK */
#define CMD23	(23)		/* SET_BLOCK_COUNT (MMC) */
#define	ACMD23	(0x80+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(24)		/* WRITE_BLOCK */
#define CMD25	(25)		/* WRITE_MULTIPLE_BLOCK */
#define CMD32	(32)		/* ERASE_ER_BLK_START */
#define CMD33	(33)		/* ERASE_ER_BLK_END */
#define CMD38	(38)		/* ERASE */
#define CMD55	(55)		/* APP_CMD */
#define CMD58	(58)		/* READ_OCR */

/* Card type flags (CardType) */
#define CT_MMC		0x01		/* MMC ver 3 */
#define CT_SD1		0x02		/* SD ver 1 */
#define CT_SD2		0x04		/* SD ver 2 */
#define CT_SDC		(CT_SD1|CT_SD2)	/* SD */
#define CT_BLOCK	0x08		/* Block addressing */

/*#####################################################*/
#define MMC_DATA_TOKEN              	0xFE
/*#####################################################*/
typedef enum
{
	IsNoCard = 0,
	IsSd,
	IsSdhc,
}sd_type_e;
/*#####################################################*/
//timer(sd_timer_delay);
/*#####################################################*/
typedef struct SD_Init_Return
{
	unsigned char SD_Init_OK;
	void* SD_Structure_Addr;
}SD_Init_Return_t;
/*#####################################################*/
/*#####################################################*/
class SD_Spi
{
public:
	SD_Spi(Io_Spi *io_port, Io_Usart *io_usart_port);
	unsigned int read(unsigned char *_Buffer, unsigned long _block, unsigned int nblks);
	unsigned int write(unsigned char *_Buffer, unsigned long _block, unsigned int nblks);
	bool SD_Init_OK;
	sd_type_e SD_Hc;
protected:
private:
	unsigned char sd_io_data(unsigned char Value);
	void sd_cs_assert();
	void sd_cs_deassert();
	unsigned char sd_cs_readstate();
	int wait_ready (	/* 1:Ready, 0:Timeout */
	unsigned long wt			/* Timeout [ms] */
	);
	void deselect ();
	int _select ();	/* 1:OK, 0:Timeout */
	unsigned char send_cmd (		/* Return value: R1 resp (bit7==1:Failed to send) */
	unsigned char cmd,		/* Command index */
	unsigned long arg		/* Argument */
	);
	bool rcvr_datablock(unsigned char *buff, unsigned int bytes_to_read);
	unsigned int _sd_read_page(unsigned char *_Buffer, unsigned long block, unsigned int nblks);
	void xmit_spi_multi (
	unsigned char *buff,	/* Pointer to the data */
	unsigned int btx			/* Number of bytes to send (512) */
	);
	int xmit_datablock (	/* 1:OK, 0:Failed */
	unsigned char *buff,	/* Ponter to 512 byte data to be sent */
	unsigned char token			/* Token */
	);
	unsigned int _sd_write_page(unsigned char *_Buffer, unsigned long block, unsigned int nblks);
	bool _mmcsd_spi_init();

	//void* SD_Structure_Addr;
	//void* SD_SPI_Struct_Addr;
	bool connected;
	unsigned int initFlg;
	unsigned char DriveNr;
	Io_Spi* io_spi_port;
	Io_Usart* io_usart_port;
};




#endif /* DEV_SD_H_ */
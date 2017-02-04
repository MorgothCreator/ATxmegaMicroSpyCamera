/*
 * main.h
 *
 * Created: 7/30/2016 2:44:08 PM
 *  Author: John Smith
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include "pm.h"
#include "dma.h"
#include "io_spi.h"
#include "io_twi.h"
#include "io_usart.h"
#include "dev_mmcsd.h"
#include "cmd_parser.h"
#include "dev_ov7690.h"

extern unsigned long FCPU;

class Main_App
{
public:
	Main_App();
protected:
private:
};

extern Pm pm;
extern Dma dma;
extern Io_Usart usart;
extern Io_Spi card_spi;
extern Io_Twi sensor_twi;
extern SD_Spi sd_spi;
extern Cmd_Parser cmd_parser;
extern Ov7690 ov7690;


#endif /* MAIN_H_ */
/*
 * ov7690.h
 *
 * Created: 7/31/2016 1:43:58 AM
 *  Author: John Smith
 */ 


#ifndef OV7690_H_
#define OV7690_H_

#include <avr/io.h>
#include <stdbool.h>
#include "io_twi.h"

#define BYTES_PER_PIXEL	1
#define OV7690_BUFF_LEN	(640 * BYTES_PER_PIXEL)
#define OV7690_I2C_ADDR	(0x42 >> 1)

typedef struct Ov7690_regval_list_s {
	unsigned char reg_num;
	unsigned char value;
}Ov7690_regval_list_t;

class Ov7690
{
public:
	Ov7690(Io_Twi *io_twi, EDMA_CH_t *dma);
	bool reg_read(unsigned char reg, unsigned char *value);
	bool reg_write(unsigned char reg, unsigned char value);
	bool write_array(const Ov7690_regval_list_t *vals);
	bool detect();
	bool set_window(unsigned short h_start,unsigned short h_end,unsigned short v_start,unsigned short v_end);
	void set_effect(unsigned char effect);
	volatile EDMA_CH_t *dma;
	volatile unsigned long buffer_change_count;
	volatile unsigned long frame_count;
	volatile unsigned short cols_on_line;
	unsigned char buff1[OV7690_BUFF_LEN * BYTES_PER_PIXEL];
	//unsigned char buff2[OV7690_BUFF_LEN * BYTES_PER_PIXEL];
	
	enum cameraEffect {
		CAMERA_EFFECT_MONO,
		CAMERA_EFFECT_NEGATIVE,
		CAMERA_EFFECT_SEPIA,
		CAMERA_EFFECT_OFF,
	}cameraEffect;
protected:
private:
	Io_Twi *io_twi;
};



#endif /* OV7690_H_ */
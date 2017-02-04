/*
 * ov7690.cpp
 *
 * Created: 7/31/2016 1:44:14 AM
 *  Author: John Smith
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "config.h"
#include <util/delay.h>
#include "main.h"
#include "dev_ov7690.h"
#include "io_twi.h"

static const Ov7690_regval_list_t ov7690_Set[] PROGMEM = {
	{0x12, 0x80},
	//{0x12, 0x00},
	//{0x15, 0x80},
	/*{0x49, 0x0c},
	{0x0c, 0x06},
	{0x48, 0x42},
	{0x41, 0x43},
	{0x4c, 0x73},
	{0x81, 0xff},
	{0x21, 0x44},
	{0x16, 0x03},
	{0x39, 0x80},
	{0x1e, 0xb1},
// formate	
	{0x12, 0x00},
	{0x82, 0x03},
	{0xd0, 0x48},
	{0x80, 0x7F},
	{0x3e, 0x30},
	{0x22, 0x00},
// resolution	
	{0x17, 0x69},
	{0x18, 0xa4},
	{0x19, 0x0c},
	{0x1a, 0xf6},
	
	{0xc8, 0x02},
	{0xc9, 0x80},
	{0xca, 0x01},
	{0xcb, 0xe0},
	
	{0xcc, 0x02},
	{0xcd, 0x80},
	{0xce, 0x01},
	{0xcf, 0xe0},
// Lens Correction	
	{0x85, 0x90},
	{0x86, 0x00},
	{0x87, 0x00},
	{0x88, 0x10},
	{0x89, 0x30},
	{0x8a, 0x29},
	{0x8b, 0x26},
// Color Matrix	
	{0xbb, 0x80},
	{0xbc, 0x62},
	{0xbd, 0x1e},
	{0xbe, 0x26},
	{0xbf, 0x7b},
	{0xc0, 0xac},
	{0xc1, 0x1e},
// Edge + Denoise	
	{0xb7, 0x05c},
	{0xb8, 0x09},
	{0xb9, 0x00},       //0x07//0x27
	{0xba, 0x18},
// 	UVAdjust
	{0x5a, 0x4a},
	{0x5b, 0x9f},
	{0x5c, 0x48},
	{0x5d, 0x32},
// AEC/AGC target	
	{0x24, 0x78},
	{0x25, 0x68},
	{0x26, 0xb3},
// Gamma	
	{0xa3, 0x0b},
	{0xa4, 0x15},
	{0xa5, 0x2a},
	{0xa6, 0x51},
	{0xa7, 0x63},
	{0xa8, 0x74},
	{0xa9, 0x83},
	{0xaa, 0x91},
	{0xab, 0x9e},
	{0xac, 0xaa},
	{0xad, 0xbe},
	{0xae, 0xce},
	{0xaf, 0xe5},
	{0xb0, 0xf3},
	{0xb1, 0xfb},
	{0xb2, 0x06},
//simple	
	{0x8e, 0x92},
	{0x96, 0xff},
	{0x97, 0x00},
// Advance	
	{0x8c, 0x5d},
	{0x8d, 0x11},
	{0x8e, 0x12},
	{0x8f, 0x11},
	{0x90, 0x50},
	{0x91, 0x22},
	{0x92, 0xd1},
	{0x93, 0xa7},
	{0x94, 0x23},
	{0x95, 0x3b},
	{0x96, 0xff},
	{0x97, 0x00},
	{0x98, 0x4a},
	{0x99, 0x46},
	{0x9a, 0x3d},
	{0x9b, 0x3a},
	{0x9c, 0xf0},
	{0x9d, 0xf0},
	{0x9e, 0xf0},
	{0x9f, 0xff},
	{0xa0, 0x56},
	{0xa1, 0x55},
	{0xa2, 0x13},
// General Control	
	{0x50, 0x4c},
	{0x51, 0x3f},
	{0x21, 0x57},
	{0x20, 0x00},
	
	{0x14, 0x29},
	{0x13, 0xf7},
	{0x11, 0x01},
	{0xd2, 0x06},
	
	{0x2a, 0x30},
	{0x2b, 0x0b},
	{0x2c, 0x00},*/



	{0x29, 0x00},
	//{0x38, 0x50},
	//{0x3E, 0x20},
	{0x12, 0x12},
	{0x13, 0x7F},
	//{0x0C, 0x01},

	{0xFF, 0xFF}
};


static const Ov7690_regval_list_t ov7690_fmt_yuv422[] PROGMEM = {

	{0xFF, 0xFF}
};

static const Ov7690_regval_list_t ov7690_res_qcif_v[] PROGMEM = {

	/* 144 x 176 */
	{0xc8, 0x02},
	{0xc9, 0x80},
	{0xca, 0x01},
	{0xcb, 0xe0},
	{0xcc, 0x00},
	{0xcd, 0x90},
	{0xce, 0x00},
	{0xcf, 0xB0},

	{0xFF, 0xFF}
};
static const Ov7690_regval_list_t ov7690_res_qcif[] PROGMEM = {
	/* 176 x 144 */
	{0x16,0x40},
	{0x17,0x83},
	{0x18,0x96},
	{0x19,0x06},
	{0x1a,0xf6},
	{0x22,0x10},
	{0xc8,0x02},
	{0xc9,0x4b},
	{0xca,0x00},
	{0xcb,0xf0},
	{0xcc,0x00},
	{0xcd,0xb0},
	{0xce,0x00},
	{0xcf,0x90},
	{0xFF, 0xFF}
};
static const Ov7690_regval_list_t ov7690_res_cif[] PROGMEM = {
	/* 352 x 288 */
	{0xc8, 0x02},
	{0xc9, 0x80},
	{0xca, 0x01},
	{0xcb, 0xe0},
	{0xcc, 0x01},
	{0xcd, 0x60},
	{0xce, 0x01},
	{0xcf, 0x20},

	{0xFF, 0xFF}
};

static const Ov7690_regval_list_t ov7690_res_qvga_v[] PROGMEM = {

	/* 240 x 320 */
	{0xc8, 0x02},
	{0xc9, 0x80},
	{0xca, 0x01},
	{0xcb, 0xe0},
	{0xcc, 0x00},
	{0xcd, 0xf0},
	{0xce, 0x01},
	{0xcf, 0x40},

	{0xFF, 0xFF}
};
static const Ov7690_regval_list_t ov7690_res_qvga[] PROGMEM = {
	/* 320 x 240 */
	{0x16,0x03},
	{0x17,0x69},
	{0x18,0xa4},
	{0x19,0x06},
	{0x1a,0xf6},
	{0x22,0x10},
	{0xc8,0x02},
	{0xc9,0x80},
	{0xca,0x00},
	{0xcb,0xf0},
	{0xcc,0x01},
	{0xcd,0x40},
	{0xce,0x00},
	{0xcf,0xf0},
	{0xFF, 0xFF}
};

static const Ov7690_regval_list_t ov7690_res_vga[] PROGMEM = {
	/* 640 x 480 */

	{0x16,0x03},
	{0x17,0x69},
	{0x18,0xa4},
	{0x19,0x0c},
	{0x1a,0xf6},
	{0x22,0x00},
	{0xc8,0x02},
	{0xc9,0x80},
	{0xca,0x01},
	{0xcb,0xe0},
	{0xcc,0x02},
	{0xcd,0x80},
	{0xce,0x01},

	{0xFF, 0xFF}
};

volatile unsigned char regs[256];

Ov7690::Ov7690(Io_Twi *io_twi, EDMA_CH_t *dma)
{
	this->io_twi = io_twi;
	this->dma = dma;
	/***** Setup PWDON *****/
	PORTR.DIRSET = 1 << 1;
	PORTR.OUTSET = 1 << 1;
	/***** Setup data lines *****/
	PORTA.DIRCLR = 0xFF;
	PORTA.PIN0CTRL = PORT_OPC_PULLDOWN_gc;
	PORTA.PIN1CTRL = PORT_OPC_PULLDOWN_gc;
	PORTA.PIN2CTRL = PORT_OPC_PULLDOWN_gc;
	PORTA.PIN3CTRL = PORT_OPC_PULLDOWN_gc;
	PORTA.PIN4CTRL = PORT_OPC_PULLDOWN_gc;
	PORTA.PIN5CTRL = PORT_OPC_PULLDOWN_gc;
	PORTA.PIN6CTRL = PORT_OPC_PULLDOWN_gc;
	PORTA.PIN7CTRL = PORT_OPC_PULLDOWN_gc;
	/***** Setup of HREF to PC3 and VSYNC to PC2 *****/
	PORTC.PIN2CTRL = PORT_OPC_PULLUP_gc;
	PORTC.PIN3CTRL = PORT_OPC_PULLUP_gc | PORT_ISC_BOTHEDGES_gc;
	PORTC.DIRCLR = 1 << 2 | 1 << 3;
	PORTC.INTMASK = 1 << 2 | 1 << 3;
	PORTC.INTCTRL = PORT_INTLVL_HI_gc;
	/***** Setup event system CH7 to take PCLK from pin D7 *****/
	PORTD.PIN7CTRL = PORT_OPC_PULLDOWN_gc | PORT_ISC_RISING_gc | PORT_INVEN_bm;
	EVSYS.CH0CTRL = 0;
	EVSYS.CH0MUX = EVSYS_CHMUX_PORTD_PIN7_gc;
	/***** Setup of DMA *****/
	dma->CTRLA = EDMA_CH_SINGLE_bm;
	dma->TRFCNT = OV7690_BUFF_LEN * BYTES_PER_PIXEL;
	dma->ADDR = (unsigned int)&PORTA.IN;
	dma->ADDRCTRL = EDMA_CH_RELOAD_NONE_gc | EDMA_CH_DIR_FIXED_gc;
	dma->DESTADDR = (unsigned int)buff1;
	dma->DESTADDRCTRL = EDMA_CH_RELOAD_TRANSACTION_gc | EDMA_CH_DIR_INC_gc;
	dma->TRIGSRC = EDMA_CH_TRIGSRC_EVSYS_CH0_gc;
	/***** Setup CLKper to PR0 pin *****/
	PORTR.DIRSET = 1 << 0;
	PORTR.PIN0CTRL = PORT_OPC_PULLUP_gc;
	//PORTCFG.CLKOUT = PORTCFG_CLKOUTSEL_CLK4X_gc | PORTCFG_CLKOUT_PR0_gc;
	EVSYS.CH1CTRL = 0;
	EVSYS.CH1MUX = EVSYS_CHMUX_PRESCALER_2_gc;
	PORTCFG.ACEVOUT = PORTCFG_EVOUT_PR0_gc | PORTCFG_EVOUTSEL_1_gc;
	_delay_ms(20);
	PORTR.OUTCLR = 1 << 1;

	/***** Setup Ov7690 *****/
	//detect();
	write_array(ov7690_Set);
	//write_array(ov7690_res_qvga);
	//set_window(0x11 << 3, 0x61 << 3, 0x03 << 2, 0x7B << 2);
	unsigned int cnt = 0;
	for (; cnt < 256; cnt++)
	{
		regs[cnt] = 0;
	}
	for (cnt = 0; cnt < 256; cnt++)
	{
		unsigned char v = 0;	
		reg_read(cnt, &v);
		regs[cnt] = v;
	}
}



bool Ov7690::reg_read(unsigned char reg, unsigned char *value)
{
	this->io_twi->MasterSlaveAddr = OV7690_I2C_ADDR;
	this->io_twi->TxBuff[0] = reg;
	if(!io_twi->MasterWriteRead(io_twi, 1, 1))
		return false;
	*value = this->io_twi->RxBuff[0];
	return true;
}

bool Ov7690::reg_write(unsigned char reg, unsigned char value)
{
	io_twi->MasterSlaveAddr = OV7690_I2C_ADDR;
	io_twi->TxBuff[0] = reg;
	io_twi->TxBuff[1] = value;
	if(!io_twi->MasterWriteRead(io_twi, 2, 0))
		return false;
	if (reg == 0x12/* && (value & 0x80)*/)
		_delay_ms(100); /* Wait for reset to run */
	return true;
}

bool Ov7690::write_array(const Ov7690_regval_list_t *vals)
{
	while (pgm_read_byte(&vals->reg_num) != 0xff) {
		if (!reg_write(pgm_read_byte(&vals->reg_num), pgm_read_byte(&vals->value)))
		{
			/*abnormal state occur.*/
			return false;
		}
		vals++;
	}
	return true;
}

bool Ov7690::detect()
{
	unsigned char v = 0;
	if(!reg_read(0x0A, &v))
		return false;
	if(v != 0x76)
		return false;
	if(!reg_read(0x0B, &v))
		return false;
	if(v != 0x91)
		return false;
	return true;
}


volatile unsigned char reg_vref;
volatile unsigned char reg_href;
volatile unsigned char reg1;
volatile unsigned char reg2;
volatile unsigned char reg3;
volatile unsigned char reg4;

bool Ov7690::set_window(unsigned short h_start,unsigned short h_end,unsigned short v_start,unsigned short v_end)
{
	unsigned char v = 0;	
	if(!reg_read(0x03, &v))
		return false;
	//v &= 0xF0;
	reg_vref = v;// + ((v_start & 0x03) << 2) + (v_end & 0x03);
	if(!reg_read(0x32, &v))
		return false;
	//v &= 0xC0;
	reg_href = v;// +  ((h_end & 0x07) << 3) + (h_start & 0x07);
	if(!reg_read(0x19, &v))
		return false;
	reg1 = v;
	if(!reg_read(0x18, &v))
		return false;
	reg2 = v;
	if(!reg_read(0x16, &v))
		return false;
	reg3 = v;
	if(!reg_read(0x17, &v))
		return false;
	reg4 = v;
	return true;
}

void Ov7690::set_effect(unsigned char effect)
{
	unsigned char i2c_buf = 0;

	switch (effect) {
		case CAMERA_EFFECT_MONO:
		reg_read(0x81, &i2c_buf);

		i2c_buf |= 0x20;
		reg_write(0x81, i2c_buf);
		reg_write(0x28, 0x02);
		reg_write(0xd2, 0x18);
		reg_write(0xda, 0x80);
		reg_write(0xdb, 0x80);

		break;
		case CAMERA_EFFECT_NEGATIVE:
		reg_read(0x81, &i2c_buf);

		i2c_buf |= 0x20;
		reg_write(0x81, i2c_buf);
		reg_write(0x28, 0x82);
		reg_write(0xd2, 0x00);
		reg_write(0xda, 0x80);
		reg_write(0xdb, 0x80);

		break;
		case CAMERA_EFFECT_SEPIA:
		reg_read(0x81, &i2c_buf);

		i2c_buf |= 0x20;
		reg_write(0x81, i2c_buf);
		reg_write(0x28, 0x02);
		reg_write(0xd2, 0x18);
		reg_write(0xda, 0x40);
		reg_write(0xdb, 0xa0);

		break;
		case CAMERA_EFFECT_OFF:
		reg_read(0x81, &i2c_buf);
		i2c_buf &= 0xdf;
		reg_write(0x81, i2c_buf);
		reg_write(0x28, 0x02);
		reg_write(0xd2, 0x00);
		reg_write(0xda, 0x80);
		reg_write(0xdb, 0x80);

		break;
	}

}

ISR(PORTC_INT_vect)
{
	if(PORTC.INTFLAGS & 1 << 3)
	{
		PORTC.INTFLAGS = 1 << 3;
		if((PORTC.IN) & 1 << 3)
		{
			ov7690.buffer_change_count++;
			ov7690.dma->CTRLA |= EDMA_CH_ENABLE_bm;
		}
	}
	if(PORTC.INTFLAGS & 1 << 2)
	{
		PORTC.INTFLAGS = 1 << 2;
		if((~PORTC.IN) & 1 << 2/* && line_cnt == 0*/)
		{
			ov7690.frame_count++;
		}
	}
}
/*
 * pm.cpp
 *
 * Created: 7/31/2016 2:35:17 AM
 *  Author: John Smith
 */ 

#include "pm.h"
#include <avr/io.h>
#include <avr/xmega.h>
#include "config.h"
#include "main.h"

//#define CALIBRATION 0x0A0C
#define CALIBRATION 0x0D28

Pm::Pm()
{
	OSC.CTRL |= OSC_RC32MEN_bm;
	DFLLRC32M.CALA = CALIBRATION;
	DFLLRC32M.CALB = CALIBRATION >> 8;
	while(!(OSC.STATUS & OSC_RC32MRDY_bm));
	PROTECTED_WRITE(CLK.CTRL, CLK_SCLKSEL_RC32M_gc);
	FCPU = 32000000;
	unsigned char pmic_reg = PMIC.CTRL;
	pmic_reg |= PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
	PROTECTED_WRITE(PMIC.CTRL, pmic_reg);
	asm("sei");
}
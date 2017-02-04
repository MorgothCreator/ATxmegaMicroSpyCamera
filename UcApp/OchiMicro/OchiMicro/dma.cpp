/*
 * dma.cpp
 *
 * Created: 8/2/2016 8:38:02 PM
 *  Author: John Smith
 */ 

#include <avr/io.h>
#include "dma.h"

Dma::Dma()
{
	EDMA.CTRL = EDMA_CHMODE_STD2_gc | EDMA_DBUFMODE_DISABLE_gc | EDMA_PRIMODE_RR0123_gc;
	EDMA.CTRL |= EDMA_ENABLE_bm;
}


/*
 * DMA.c
 *
 *  Created on: Apr 2, 2020
 *      Author: Norhan Nassar
 */

#include "STD_TYPES.h"
#include "RCC.h"
#include "NVIC.h"
#include "GPIO.h"
#include "DMA.h"
#include "DMA.h"

Dma1Ch1Cbf_t Dma1Ch1AppNotify;
Dma1Ch2Cbf_t Dma1Ch2AppNotify;
Dma1Ch3Cbf_t Dma1Ch3AppNotify;
Dma1Ch4Cbf_t Dma1Ch4AppNotify;
Dma1Ch5Cbf_t Dma1Ch5AppNotify;
Dma1Ch6Cbf_t Dma1Ch6AppNotify;
Dma1Ch7Cbf_t Dma1Ch7AppNotify;

Dma2Ch1Cbf_t Dma2Ch1AppNotify;
Dma2Ch2Cbf_t Dma2Ch2AppNotify;
Dma2Ch3Cbf_t Dma2Ch3AppNotify;
Dma2Ch4Cbf_t Dma2Ch4AppNotify;

STD_ERROR DMA_Init()
{
	/* momken yb2a fie 7aga t concatenate el DMA_CONTROOLLER 2ablha y7ot RCC_ */
	if(DMA_CONTROLLER == DMA1_t)
		return RCC_EnablePeri(RCC_DMA1);
	if(DMA_CONTROLLER == DMA2_t)
		return RCC_EnablePeri(RCC_DMA2);
	return NOT_OK;
}

STD_ERROR DMA_Configure(DMA_cfg_t* DMA_cfg)
{
	/*--------------Disable channel first to be able to configure it--------------*/
	DMA_CONTROLLER->CH[DMA_cfg->ChannelNum][CCR] &= ~(EN_CH);

	/*---------------------Set Peripheral and Memory Addresses-------------------*/
	DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CPAR] = DMA_cfg->PerAdd;
	DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CMAR] = DMA_cfg->MemAdd;

	/*----------------------------Set the Counter---------------------------------*/
	DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CNDTR] = DMA_cfg->counter;

	/*-----------------------------Configure Channel Priority---------------------*/
	DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CCR] &= ~(PIR_CLEAR);
	DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CCR] |= DMA_cfg->Periority;

	/*--------Configure Memory and peripheral Increment enabled or disabled ------*/
	if(!DMA_cfg->MemAutoInc)
		DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CCR] &= ~(MINC);
	else
		DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CCR] |= MINC;
	if(!DMA_cfg->PerAutoInc)
		DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CCR] &= ~(PINC);
	else
		DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CCR] |= PINC;
	/*----------------------Configure Direction of Copy---------------------------*/
	if(DMA_cfg->Direction == DIR_PRE2MEM)
		DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CCR] &= DIR_PRE2MEM;
	else
		DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CCR] |= DIR_MEM2PRE;

	/*---------------Configure MemorySize and PeripheralSize---------------------*/
	DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CCR] &= ~(MSIZE_CLEAR);
	DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CCR] &= ~(PSIZE_CLEAR);
	DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CCR] |= DMA_cfg->MemSize;			/* el mafro hena ne3ml macro yt2aked enna men el options el awel */
	DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CCR] |= DMA_cfg->PerSize;

	/*------------------------Configure Interrupts-------------------------------*/
	DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CCR] |= DMA_cfg->EnableInterrupts;
	DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CCR] &= ~(DMA_cfg->DisableInterrupts);	/* el mafrod hena yb2a fie macro e check bardo */

	/*-----------Configure Mem2Mem bit and Circular Mode-------------------------*/
	if(!DMA_cfg->CircularMode)
		DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CCR] &= ~(CIRC);
	else
		DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CCR] |= CIRC;
	if(!DMA_cfg->MemToMem)
		DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CCR] &= ~(MEM2MEM);
	else
		DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CCR] |= MEM2MEM;

	/*--------------Enable channel after configuration --------------*/
	DMA_CONTROLLER->CH[(DMA_cfg->ChannelNum-1)][CCR] |= EN_CH;

	return OK;
}

STD_ERROR DMA_setDma1Ch1Cbf(Dma1Ch1Cbf_t Dma1Ch1Cbf)
{
	if(Dma1Ch1Cbf)
	{
		Dma1Ch1AppNotify = Dma1Ch1Cbf;
		return OK;
	}
	return NOT_OK;
}
STD_ERROR DMA_setDma1Ch2Cbf(Dma1Ch2Cbf_t Dma1Ch2Cbf)
{
	if(Dma1Ch2Cbf)
	{
		Dma1Ch2AppNotify = Dma1Ch2Cbf;
		return OK;
	}
	return NOT_OK;
}
STD_ERROR DMA_setDma1Ch3Cbf(Dma1Ch3Cbf_t Dma1Ch3Cbf)
{
	if(Dma1Ch3Cbf)
	{
		Dma1Ch3AppNotify = Dma1Ch3Cbf;
		return OK;
	}
	return NOT_OK;
}
STD_ERROR DMA_setDma1Ch4Cbf(Dma1Ch4Cbf_t Dma1Ch4Cbf)
{
	if(Dma1Ch4Cbf)
	{
		Dma1Ch4AppNotify = Dma1Ch4Cbf;
		return OK;
	}
	return NOT_OK;
}
STD_ERROR DMA_setDma1Ch5Cbf(Dma1Ch5Cbf_t Dma1Ch5Cbf)
{
	if(Dma1Ch5Cbf)
	{
		Dma1Ch5AppNotify = Dma1Ch5Cbf;
		return OK;
	}
	return NOT_OK;
}
STD_ERROR DMA_setDma1Ch6Cbf(Dma1Ch6Cbf_t Dma1Ch6Cbf)
{
	if(Dma1Ch6Cbf)
	{
		Dma1Ch6AppNotify = Dma1Ch6Cbf;
		return OK;
	}
	return NOT_OK;
}
STD_ERROR DMA_setDma1Ch7Cbf(Dma1Ch7Cbf_t Dma1Ch7Cbf)
{
	if(Dma1Ch7Cbf)
	{
		Dma1Ch7AppNotify = Dma1Ch7Cbf;
		return OK;
	}
	return NOT_OK;
}

STD_ERROR DMA_setDma2Ch1Cbf(Dma2Ch1Cbf_t Dma2Ch1Cbf)
{
	if(Dma2Ch1Cbf)
	{
		Dma2Ch1AppNotify = Dma2Ch1Cbf;
		return OK;
	}
	return NOT_OK;
}
STD_ERROR DMA_setDma2Ch2Cbf(Dma2Ch2Cbf_t Dma2Ch2Cbf)
{
	if(Dma2Ch2Cbf)
	{
		Dma2Ch2AppNotify = Dma2Ch2Cbf;
		return OK;
	}
	return NOT_OK;
}
STD_ERROR DMA_setDma2Ch3Cbf(Dma2Ch3Cbf_t Dma2Ch3Cbf)
{
	if(Dma2Ch3Cbf)
	{
		Dma2Ch3AppNotify = Dma2Ch3Cbf;
		return OK;
	}
	return NOT_OK;
}
STD_ERROR DMA_setDma2Ch4Cbf(Dma2Ch4Cbf_t Dma2Ch4Cbf)
{
	if(Dma2Ch4Cbf)
	{
		Dma2Ch4AppNotify = Dma2Ch4Cbf;
		return OK;
	}
	return NOT_OK;
}

void DMA1_Channel1_IRQHandler(void)
{
	if(Dma1Ch1AppNotify)
		Dma1Ch1AppNotify();
}

void DMA1_Channel2_IRQHandler(void)
{
	if(Dma1Ch2AppNotify)
		Dma1Ch2AppNotify();
}

void DMA1_Channel3_IRQHandler(void)
{
	if(Dma1Ch3AppNotify)
		Dma1Ch3AppNotify();
}
void DMA1_Channel4_IRQHandler(void)
{
	if(Dma1Ch4AppNotify)
		Dma1Ch4AppNotify();
}

void DMA1_Channel5_IRQHandler(void)
{
	GPIO_voidWrite(PORTC , GPIO_PIN13 , SET);
	if(Dma1Ch5AppNotify)
		Dma1Ch5AppNotify();
}

void DMA1_Channel6_IRQHandler(void)
{
	if(Dma1Ch6AppNotify)
		Dma1Ch6AppNotify();
}

void DMA1_Channel7_IRQHandler(void)
{
	if(Dma1Ch7AppNotify)
		Dma1Ch7AppNotify();
}

void DMA2_Channel1_IRQHandler(void)
{
	if(Dma2Ch1AppNotify)
		Dma1Ch1AppNotify();
}

void DMA2_Channel2_IRQHandler(void)
{
	if(Dma2Ch2AppNotify)
		Dma1Ch2AppNotify();
}

void DMA2_Channel3_IRQHandler(void)
{
	if(Dma2Ch3AppNotify)
		Dma1C31AppNotify();
}

void DMA2_Channel4_5_IRQHandler(void)
{
	if(Dma2Ch4AppNotify)
		Dma1Ch4AppNotify();
}






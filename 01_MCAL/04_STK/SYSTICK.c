/*
 * SYSTICK_program.c
 *
 *  Created on: Mar 10, 2020
 *      Author: Electronica CARE
 */

#include "STD_TYPES.h"

#include "SYSTICK_cfg.h"
#include "SYSTICK.h"
#include "NVIC.h"
#include "RCC.h"

CALLBACKFUN SysTick_CallBack;

STD_ERROR SYSTICK_Init(void)
{
	u8 MemReg;
	if(!(CHECKINIT()))
		return NOT_OK;

	/* Set up its prescaller */
	if(STK_INIT_PRESCALLER == AHB)
		SYSTICKx->CTRL |= STK_INIT_PRESCALLER;
	else
		SYSTICKx->CTRL &= ~(STK_INIT_PRESCALLER);

	/* Enable or disable SYSTICK Exception according to cfg file */
	SYSTICKx->CTRL |= STK_EXCEPTION;

	if(STK_EXCEPTION == ENABLE)
	{
		/* Set up the Exception */
		SCB_GetVectorTableBaseOffset(&MemReg);
		if(MemReg == VTOR_SRAM)
		{
			/* If vector table is at SRAM so we must set the address of SYSTICK handler at SRAM also */
			/* 15 is the SYSTICK exception number */
			*((volatile u32 *)(SCBx->VTOR+(15<<2))) = (u32)SysTick_Handler;
		}
	}
	return OK;
}

STD_ERROR SYSTICK_SetPrescaller(s32 Copy_u32Prescaler)
{
	if(! (CHECKPRESCALER(Copy_u32Prescaler)) )
		return NOT_OK;

	switch(Copy_u32Prescaler)
	{
	case AHB:
		SYSTICKx->CTRL |= AHB;
		break;
	case AHB_8:
		SYSTICKx->CTRL &= (AHB_8);
		break;
	}
	return OK;
}

STD_ERROR SYSTICK_Start(void)
{
	/* Counter Enable */
	SYSTICKx->CTRL |= SYSTICK_ENABLE_MASK;

	if( (SYSTICKx->CTRL& (SYSTICK_ENABLE_MASK)) == SYSTICK_ENABLE_MASK)
		return OK;
	else
		return NOT_OK;

}

STD_ERROR SYSTICK_Stop(void)
{
	STD_ERROR loc_err = OK;

	/* Counter Disable */
	SYSTICKx->CTRL &= (~SYSTICK_ENABLE_MASK);

	return loc_err;

}

STD_ERROR SYSTICK_SetTime(u32 TimeMs)
{
	STD_ERROR loc_err = OK;
	u32 Reload = 0;
	u32 Freq;
	/*********************************************/
	/* Get System Clock 						*/
	if(STK_CLOCKSOURCE == EXTERNAL)
	{
		if(SYSTICKx->CALIB & EXTERNAL != SYSTICKx->CALIB)
			return NOT_OK;
		/* External clock must be at least two times slower than the internal clock which is 8 MHZ */
		else if(STK_CLOCKSOURCE_EXTERNAL_MHZ > 2*8)
			return NOT_OK;
		else
			Freq = STK_CLOCKSOURCE_EXTERNAL_MHZ;
	}
	else if(STK_CLOCKSOURCE == INTERNAL)
		RCC_GetSysClock(&Freq);
	else
		return NOT_OK;
	/************************************************/
	/* Calculate Reload according to system clock and desired time */
	if( (SYSTICKx->CTRL & AHB) == AHB)
	{
		/* if the prescaler is equal to AHB */
		/* Time with Micro sec and freqency with MHZ */
		Reload = TimeMs * Freq;
	}
	else
	{
		/* if the prescaler is equal to AHB/8 */
		/* Time with Micro sec and freqency with MHZ */
		Reload = (TimeMs * Freq)/8;
	}

	/* The RELOAD value can be any value in the range 0x00000001-0x00FFFFFF */
	if((Reload>0x00FFFFFF)||(Reload<0x00000001))
		loc_err = NOT_OK;

	SYSTICKx->LOAD = Reload;
	 return loc_err;
}


STD_ERROR SYSTICK_SetCallBack( void (*Copy_PtrCbf)(void))
{
	STD_ERROR loc_err = NOT_OK;
	/* Timer must be stopped to set the callback function
	 * if it is enabled return Error */
	if( (SYSTICKx->CTRL & SYSTICK_ENABLE_MASK) == SYSTICK_ENABLE_MASK )
		return loc_err;

	if(Copy_PtrCbf)
	{
		SysTick_CallBack = Copy_PtrCbf;
		loc_err =OK;
	}
	return loc_err;
}

STD_ERROR SysTick_Handler(void)
{
	STD_ERROR loc_err = NOT_OK;
	if(SysTick_CallBack)
	{
		SysTick_CallBack();
		loc_err = OK;
	}
	return loc_err;
}







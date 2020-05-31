#include "STD_TYPES.h"
#include "RCC_interface.h"

/* Set clock ON or OFF
 * Arguments:
 * clock -> HSI, HSE, PLL
 * state -> ON , OFF */
STD_ERROR RCC_SetClock(u32 clock,STD_STATE state)
{
	STD_ERROR loc_err;
	u16 check = 0;
	loc_err = NOT_OK;
	if(state == ON)
	{
		switch(clock)
		{
		case HSI:
			RCC->CR |= clock;
			while(check<=BUSYWAIT)
			{
				if((RCC->CR & HSIRDY )== HSIRDY)
				{
					loc_err = OK;
					break;
				}
				check++;
			}
			break;
		case HSE:
			RCC->CR |= clock;
			while(check<=BUSYWAIT)
			{
				if((RCC->CR & HSERDY) == HSERDY)
				{
					loc_err = OK;
					break;
				}
				check++;
			}
			break;
		case PLL:
			RCC->CR |= clock;
			while(check<=BUSYWAIT)
			{
				if((RCC->CR & PLLRDY) == PLLRDY)
				{
					loc_err = OK;
					break;
				}
				check++;
			}
			break;
		}
	}
	else if (state == OFF)
	{
		switch(clock)
		{
		case HSI:
			RCC->CR &= ~clock;
			while(check<=BUSYWAIT)
			{
				if((RCC->CR | HSIRDY) != RCC->CR)
				{
					loc_err = OK;
					break;
				}
				check++;
			}
			break;
		case HSE:
			RCC->CR &= ~clock;
			while(check<=BUSYWAIT)
			{
				if((RCC->CR | HSERDY) != RCC->CR)
				{
					loc_err = OK;
					break;
				}
				check++;
			}
			break;
		case PLL:
			RCC->CR &= ~clock;
			while(check<=BUSYWAIT)
			{
				if((RCC->CR | PLLRDY) != RCC->CR)
				{
					loc_err = OK;
					break;
				}
				check++;
			}
			break;
		}
	}
	return loc_err;
}

/* before choosing any system clock you should turn it one first
 * Arguments:
 * clock : SW_HSI, SW_HSE, SW_PLL */
STD_ERROR RCC_SelectSysClock(u32 clock)
{
	u32 RCC_temp = RCC->CFGR;
	STD_ERROR loc_err;
	loc_err = NOT_OK;;

	switch(clock)
	{
	case SW_HSI:
		if( ( (RCC->CR & HSI) == HSI ) && ( (RCC->CR & HSIRDY) == HSIRDY ) )
			{
			RCC_temp &= SW_CLEAR;
			RCC->CFGR = RCC_temp;
			loc_err = OK;
			}
		break;

	case SW_HSE:
		if( ( (RCC->CR & HSE) == HSE ) && ( (RCC->CR & HSERDY) == HSERDY ) )
		{
			RCC_temp &= SW_CLEAR;
			RCC_temp |= SW_HSE;
			RCC->CFGR = RCC_temp;
			loc_err = OK;
		}
		break;

	case SW_PLL:
		if( ( (RCC->CR & PLL) == PLL ) && ( (RCC->CR & PLLRDY) == PLLRDY ) )
		{
			RCC_temp &= SW_CLEAR;
			RCC_temp |= SW_PLL;
			RCC->CFGR = RCC_temp;
			loc_err = OK;
		}
		break;
	}
	return loc_err;
}

/******************************************************************/

STD_ERROR RCC_ConfigurePLLClock(u32 source_clock, u32 PLL_MUL_XX )
{
	STD_ERROR loc_err = NOT_OK;
	switch (source_clock)
	{
	case HSI:
		/* PLL must be OFF to enable configuration on it */
		if((RCC->CR & PLL) != PLL)
		{
			RCC->CFGR &= ~PLLSRC_MASK;

			RCC->CFGR &= PLL_MUL_CLEAR;
			RCC->CFGR |= PLL_MUL_XX;
			loc_err = OK;
		}
		break;
	case HSE:
		/* PLL must be OFF to enable configuration on it */
		if((RCC->CR & PLL) != PLL)
		{
			RCC->CFGR |= PLLSRC_MASK;
			RCC->CFGR &= ~PLLXTPRE_MASK;

			RCC->CFGR &= PLL_MUL_CLEAR;
			RCC->CFGR |= PLL_MUL_XX;
			loc_err = OK;
		}
		break;
	case HSE_2:
		/* PLL must be OFF to enable configuration on it */
		if((RCC->CR & PLL) != PLL)
		{
			RCC->CFGR |= PLLSRC_MASK;
			RCC->CFGR |= PLLXTPRE_MASK;

			RCC->CFGR &= PLL_MUL_CLEAR;
			RCC->CFGR |= PLL_MUL_XX;
			loc_err = OK;
		}
		break;
	}

	return loc_err;
}

/******************************************************************/

STD_ERROR RCC_SetPort(u32 Port,STD_STATE state)
{
	STD_ERROR loc_err;
	loc_err = OK;
	switch(Port)
	{
	case RCC_GPIOA:
		if(state == ON)
		{
			RCC->APB2ENR |= Port;
		}
		else if(state==OFF)
		{
			RCC->APB2ENR &= ~Port;
		}
		else
			loc_err = NOT_OK;
		break;
	case RCC_GPIOB:
		if(state == ON)
		{
			RCC->APB2ENR |= Port;
		}
		else if(state==OFF)
		{
			RCC->APB2ENR &= ~Port;
		}
		else
			loc_err = NOT_OK;
		break;

	case RCC_GPIOC:
		if(state == ON)
		{
			RCC->APB2ENR |= Port;
		}
		else if(state==OFF)
		{
			RCC->APB2ENR &= ~Port;
		}
		break;

	case RCC_GPIOD:
		if(state == ON)
		{
			RCC->APB2ENR |= Port;
		}
		else if(state==OFF)
		{
			RCC->APB2ENR &= ~Port;
		}
		else
			loc_err = NOT_OK;
		break;

	case RCC_GPIOE:
		if(state == ON)
		{
			RCC->APB2ENR |= Port;
		}
		else if(state==OFF)
		{
			RCC->APB2ENR &= ~Port;
		}
		else
			loc_err = NOT_OK;
		break;

	case RCC_GPIOF:
		if(state == ON)
		{
			RCC->APB2ENR |= Port;
		}
		else if(state==OFF)
		{
			RCC->APB2ENR &= ~Port;
		}
		else
			loc_err = NOT_OK;
		break;

	case RCC_GPIOG:
		if(state == ON)
		{
			RCC->APB2ENR |= Port;
		}
		else if(state==OFF)
		{
			RCC->APB2ENR &= ~Port;
		}
		else
			loc_err = NOT_OK;
		break;
	}

	return loc_err;
}

STD_ERROR RCC_EnablePeri(u32 Peri)
{
	if(Peri == RCC_USART1)
	{
		RCC->APB2ENR |= RCC_USART1;			/* USART1 Peri on APB2 bus */
		return OK;
	}
	else if((Peri == RCC_DMA1)|(Peri == RCC_DMA2))
	{
		RCC->AHBENR |= Peri;				/* for DMA1 & DMA2 Peri on AHB bus */
		return OK;
	}
	return NOT_OK;
}

/* lsa hatt3ml trag3 MHZ mosh HSI wala HSE wala PLL */
STD_ERROR RCC_GetSysClock(u32* SysClk)
{
		STD_ERROR loc_err = OK;
		u32 Clk;
		u32 Mul;

		Clk = RCC->CFGR & CHECK_SWS;
		switch(Clk)
		{
		case SWS_HSE:
			*SysClk = HSE_8MHZ;
			break;
		case SWS_HSI:
			*SysClk = HSI_8MHZ;
			break;
		case SWS_PLL:
			if(( (RCC->CFGR & PLLSRC_MASK) == PLLSRC_MASK) && ( (RCC->CFGR & PLLXTPRE_MASK) == PLLXTPRE_MASK))
			{
				/* HSE oscillator clock selected as PLL input clock */
				/* HSE clock divided by 2 */
				Mul = (RCC->CFGR & (~PLL_MUL_CLEAR))>>PLL_SHIFT_VALUE;
				Mul+=PLL_MUL_VALUE;
				*SysClk = (HSE_8MHZ/PLL_MUL_VALUE) * Mul;
			}
			else if(( (RCC->CFGR & PLLSRC_MASK) == PLLSRC_MASK) && ( (RCC->CFGR & PLLXTPRE_MASK) != PLLXTPRE_MASK))
			{
				/* HSE oscillator clock selected as PLL input clock */
				/* HSE clock not divided */
				Mul = (RCC->CFGR & (~PLL_MUL_CLEAR))>>PLL_SHIFT_VALUE;
				Mul+=PLL_MUL_VALUE;
				*SysClk = HSE_8MHZ * Mul;
			}
			else
			{
				/* HSI oscillator clock / 2 selected as PLL input clock */
				Mul = (RCC->CFGR & (~PLL_MUL_CLEAR))>>PLL_SHIFT_VALUE;
				Mul+=PLL_MUL_VALUE;
				*SysClk = ( HSI_8MHZ / PLL_MUL_VALUE) * Mul;
			}
			break;
		}

		return loc_err;
}


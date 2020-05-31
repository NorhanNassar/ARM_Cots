/************************************************/
/*  Author : Norhan Nassar    			     	*/
/*  Version : V03          						*/
/*  Date : 10 March 2020    					*/
/************************************************/

#ifndef RCC_INTERFACE_H
#define RCC_INTERFACE_H

/* RCC_CR Register */
/* Enable clock */
#define HSION_MASK		0x1
#define HSEON_MASK		0x10000
#define PLLON_MASK		0x1000000

#define HSE 			HSEON_MASK
#define HSI 			HSION_MASK
#define PLL 			PLLON_MASK

#define HSEBYP_MASK		0x40000
#define CSS				0x80000

#define HSEBYP			HSEBYP_MASK

/* Clock Ready */
#define HSIRDY			0x2
#define HSERDY			0x20000
#define PLLRDY			0x2000000

/* RCC_CFGR Register */
/* System Clock */
#define SW_HSE			0x1
#define SW_PLL			0x2
#define SW_HSI			SW_CLEAR
#define SW_CLEAR 		0xFFFFFFFC

#define CHECK_SWS		0xC

#define SWS_HSI			0x0
#define SWS_HSE			0x4
#define SWS_PLL			0x8

/* PLL CFGR */
#define 		PLL_MUL_CLEAR		0xFFC3FFFF
#define			PLL_MUL_2			0x00
#define			PLL_MUL_3			0x40000
#define			PLL_MUL_4			0x80000
#define			PLL_MUL_5			0xC0000
#define			PLL_MUL_6 			0x100000
#define			PLL_MUL_7			0x140000
#define			PLL_MUL_8 			0x180000
#define			PLL_MUL_9 			0x1C0000
#define			PLL_MUL_10	 		0x200000
#define			PLL_MUL_11	 		0x240000
#define			PLL_MUL_12	 		0x220000
#define			PLL_MUL_13	 		0x2C0000
#define			PLL_MUL_14	 		0x300000
#define			PLL_MUL_15	 		0x380000

#define PLLSRC_MASK			0x10000

#define PLLXTPRE_MASK		0x20000
#define HSE_2				PLLXTPRE_MASK

/* PRE */
#define 		USBPRE				0x400000
#define 		ADCPRE_CLEAR		0x11131111
#define			ADCPRE_2			ADCPRE_CLEAR
#define			ADCPRE_4			0x4000
#define			ADCPRE_6			0x8000
#define			ADCPRE_8			0xC000

#define 		PPRE2_APB2_2				0x0020
#define 		PPRE2_APB2_4				0x0028
#define 		PPRE2_APB2_8				0x0030
#define 		PPRE2_APB2_16				0x0031

#define 		PPRE1_APB1_2				0x004
#define 		PPRE1_APB1_4				0x005
#define 		PPRE1_APB1_8				0x006
#define 		PPRE1_APB1_16				0x007

#define 		HPRE_AHB_2					0x08
#define 		HPRE_AHB_4					0x09
#define 		HPRE_AHB_8					0x0A
#define 		HPRE_AHB_16					0x0B
#define 		HPRE_AHB_64					0x0C
#define 		HPRE_AHB_128				0x0D
#define 		HPRE_AHB_256				0x0E
#define 		HPRE_AHB_512				0x0F

/* RCC_APB2ENR */
#define 		RCC_GPIOA				0x4
#define 		RCC_GPIOB				0x8
#define 		RCC_GPIOC				0x10
#define 		RCC_GPIOD				0x20
#define 		RCC_GPIOE				0x40
#define 		RCC_GPIOF				0x80
#define 		RCC_GPIOG				0x100

/* To get system clock */
#define 		HSE_8MHZ				8
#define			HSI_8MHZ				8
#define 		PLL_MUL_VALUE			2
#define 		PLL_SHIFT_VALUE			18

/* Busy wait to check within it if clock is set correctly or not */
/* according to its RDY bit in CR Register 						*/
#define BUSYWAIT						2000

/* Registers Addresses */
#define RCC_BASE_ADD      				(void*)0x40021000

#define RCC_USART1						0x4000
#define RCC_DMA1						0x8001
#define RCC_DMA2						0x8002

typedef struct
{
	/* FOR PROCESSOR CLOCK */
	u32 CR;
	u32 CFGR;
	u32 CIR;
	u32 APB2RSTR;
	u32 APB1RSTR;
	/* TO ENABLE AND DISABLE EACH PREIPHERAL */
	u32 AHBENR;
	u32 APB2ENR;
	u32 BDCR;
	u32 CSR;
} RCC_t;

#define RCC		((RCC_t volatile * const)RCC_BASE_ADD)

/*****************************************************************************************/
/* Description: This API shall set clock ON or OFF										*/
/* Arguments:																		   */
/* clock -> HSI, HSE, PLL															  */
/* state -> ON , OFF                                                                 */
/************************************************************************************/
extern STD_ERROR RCC_SetClock(u32 clock,STD_STATE state);

/*****************************************************************************************/
/* Description: This API shall select system clock and this clock must be enabled first */
/* Arguments:																		   */
/* clock : SW_HSI, SW_HSE, SW_PLL 													  */
/*************************************************************************************/
extern STD_ERROR RCC_SelectSysClock(u32 clock);

/**********************************************************************/
/* Description: This API shall Configure PLL Clock before turn it on */
/* Arguments:														*/
/* source_clock -> HSI, HSE, HSE_2								   */
/* PLL_MUL_XX -> PLL_MUL_2 to PLL_MUL_15                          */
/*****************************************************************/
extern STD_ERROR RCC_ConfigurePLLClock(u32 source_clock, u32 PLL_MUL_ );

/*******************************************************************************************/
/* Description: This API shall Configure PLL Clock before turn it on                      */
/* Description: This API shall enable clock on certain Port							     */
/* Arguments:													   				        */
/* Port -> RCC_GPIOA, RCC_GPIOB, RCC_GPIOC, RCC_GPIOD, RCC_GPIOE, RCC_GPIOF, RCC_GPIOG */
/* State -> ON, OFF 																  */
/*************************************************************************************/
extern STD_ERROR RCC_SetPort(u32 Port,STD_STATE state);

extern STD_ERROR RCC_EnablePeri(u32 Peri);

/***********************************************************************************/
/* Description: This API shall Return System Clock							      */
/* Arguments: Pointer to get SysClk in it 								       	 */
/********************************************************************************/
extern STD_ERROR RCC_GetSysClock(u32* SysClk);

#endif

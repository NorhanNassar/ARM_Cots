/*
 * USART.c
 *
 *  Created on: Mar 28, 2020
 *      Author: Norhan Nassar
 */

#include "STD_TYPES.h"
#include "GPIO.h"
#include "RCC.h"
#include "NVIC.h"
#include "DMA_cfg.h"
#include "DMA.h"
#include "LIN_cfg.h"
#include "LIN.h"
#include "USART.h"
#include "USART_cfg.h"
#include "Sched.h"

volatile txCbf_t appTxNotify;
volatile rxCbf_t appRxNotify;
volatile breakCbf_t appBreakNotify;

typedef enum
{
	ideal,
	busy
} bufferState_t;
typedef struct
{
	volatile u8* ptrToDataBuffer;
	u32 Position;
	u32 Size;
	bufferState_t State;
}dataBuffer_t;

typedef u8 DMAstate_t;
#define DMA_ideal	0
#define DMA_busy	1

#define NULL (void*)0

/*------------------------------------------------Macros to Check Input Parameters------------------------------------------------*/
#define CHECKDATABITS(NUMBER)	((NUMBER== (DATA_BITS_EIGHT) )|(NUMBER== (DATA_BITS_NINE) ))
#define CHECKPARITY(PARITY)		( (PARITY== (NO_PARITY))|(PARITY== (EVEN_PARITY))|(PARITY== (ODD_PARITY)) )
#define CHECKNUMSTOP(NUMBER)	( (NUMBER== ONE_STOP_BIT)|(NUMBER== TWO_STOP_BIT)|(NUMBER== ONE_HALF_STOP_BIT)|(NUMBER== HALF_STOP_BIT))
#define CHECKCLKMODE(CMODE)		( (CMODE== ASYNCHRONOUS)|(CMODE== SYNCHRONOUS) )
#define CHECKERRINT(ERINT)		( (ERINT== DISABLE_ERROR_INT)|(ERINT== ENABLE_ERROR_INT))
#define CHECKDUPLEXT(TYPE)		( (TYPE== FULL_DUPLEX)|(TYPE== HALF_DUPLEX) )
#define CHECKUSAGEMODE(UMODE)	( (UMODE== INTERRUPT_MODE)|(UMODE== DMA_MODE)|(UMODE== POLLING_MODE) )
#define CHECKRXMODE(RMODE)		( (RMODE== RX_PIN_INPUT_FLOATING)|(RMODE== RX_PIN_INPUT_PULLUP) )
#define CHECKTXMODE(TMODE)		( (TMODE== TX_SPEED_2MHZ)|(TMODE== TX_SPEED_10MHZ)|(TMODE== TX_SPEED_50MHZ))
#define CHECKPARAM(BITS,PARITY,STOP,CLK,ERR,DUPLEX,USAGE,RX,TX) (CHECKDATABITS(BITS)&CHECKPARITY(PARITY)&CHECKNUMSTOP(STOP)&CHECKCLKMODE(CLK)&CHECKERRINT(ERR)&CHECKDUPLEXT(DUPLEX)&CHECKUSAGEMODE(USAGE)&CHECKRXMODE(RX)&CHECKTXMODE(TX))
/*--------------------------------------------------------------------------------------------------------------------------------*/

static volatile dataBuffer_t txBuffer={0};
static volatile dataBuffer_t rxBuffer={0};

volatile DMAstate_t DMASendState = DMA_ideal;
volatile DMAstate_t DMAReceiveState = DMA_ideal;

extern volatile USART_Init_Cfg_t USART_Init_Cfg;

static STD_ERROR USART_SendByInter(volatile u8* bufferData, u32 bytesCount);
static STD_ERROR USART_SendByDma(volatile u8* bufferData, u32 bytesCount);

static STD_ERROR USART_RecieveByInter(volatile u8* bufferData, u32 bytesCount);
static STD_ERROR USART_RecieveByDma(volatile u8* bufferData, u32 bytesCount);
//static STD_ERROR USART_SendByTask(u8* bufferData, u32 bytesCount);

STD_ERROR USART_Init()
{
	u32 tempReg;
	/* Check that arguments at cfg file if there are correct first */
	if(CHECKPARAM(USART_Init_Cfg.NumberOfDataBits,USART_Init_Cfg.ParityBit,USART_Init_Cfg.NumberOfStopBits,\
			USART_Init_Cfg.ClockMode,USART_Init_Cfg.ErrorInterrupt,USART_Init_Cfg.TypeOfDuplex,USART_Init_Cfg.UartUsageMode,\
			USART_Init_Cfg.RxPinModeCfg,USART_Init_Cfg.TXPinSpeedCfg))
	{
		/* Enable clock on Port A and USART1 */
		RCC_SetPort(RCC_GPIOA,ON);						/* Enable Clock on port A  for USART */
		RCC_EnablePeri(RCC_USART1);						/* Enable Clock on USART1 Peripheral */

		if(USART_Init_Cfg.UartUsageMode==INTERRUPT_MODE)
			NVIC_EnableInterrupt(USART1);
		else if(USART_Init_Cfg.UartUsageMode==DMA_MODE)
		{
			DMA_Init();
			NVIC_EnableInterrupt(DMA1_CHANNEL4);
			NVIC_EnableInterrupt(DMA1_CHANNEL5);
			USART1x->CR3 |= (DMA_T_EN | DMA_R_EN);
		}

		/* Configure GPIO Pins for USART1	*/
		GPIO_Conf_Pin_t Uart_Pin_TX = {GPIO_PIN9,MODE_OP_AF_PP,USART_Init_Cfg.TXPinSpeedCfg,PORTA};
		GPIO_Conf_Pin_t Uart_Pin_RX = {GPIO_PIN10,USART_Init_Cfg.RxPinModeCfg,IP_NOSPEED,PORTA};
		GPIO_u8Write(PORTA,GPIO_PIN10,1);

		GPIO_voidConfigure(&Uart_Pin_TX);
		GPIO_voidConfigure(&Uart_Pin_RX);

		/* Configure CR1 Register */
		tempReg = USART1x->CR1;

		tempReg |= USART_Init_Cfg.Enable_Interrupts;
		tempReg &= DISABLE_TXE_INT;						/* to enable it only when we use it for interrupt mode and when there is data to send */

		if(USART_Init_Cfg.NumberOfDataBits == DATA_BITS_EIGHT)
			tempReg &= DATA_BITS_EIGHT;
		else
			tempReg |= DATA_BITS_NINE;

		tempReg &= USART_Init_Cfg.Disable_Interrupts;

		switch(USART_Init_Cfg.ParityBit)
		{
		case NO_PARITY:
			tempReg &= NO_PARITY;
			break;
		case EVEN_PARITY:
			tempReg |= PCE_PARITY_CONTROL;
			tempReg &= ~(PS_PARITY_SELECTION);							/* 0 -> Even parity	*/
			break;
		case ODD_PARITY:
			tempReg |= ( PCE_PARITY_CONTROL | PS_PARITY_SELECTION );	/* 1-> Odd Parity */
			break;
		}

		tempReg |= (TE_EN | RE_EN);
		USART1x->CR1 = tempReg;

		/* Configure CR2 Register */
		tempReg = USART1x->CR2;
		tempReg &= ~(STOP_BITS_CLEAR);
		tempReg |= USART_Init_Cfg.NumberOfStopBits;

		if(USART_Init_Cfg.ClockMode == ASYNCHRONOUS)
			tempReg &= USART_Init_Cfg.ClockMode;
		else
			tempReg |= USART_Init_Cfg.ClockMode;

		USART1x->CR2 = tempReg;

		/* Configure CR3 Register */
		USART1x->CR3 &= USART_Init_Cfg.ErrorInterrupt;
		if(USART_Init_Cfg.TypeOfDuplex == HALF_DUPLEX)
			USART1x->CR3 |= HALF_DUPLEX;
		else
			USART1x->CR3 &= FULL_DUPLEX;

		USART1x->BRR = 0x00000341;

		/* Enable USART1 */
		USART1x->CR1 |= UE_ENABLE;

		return OK;
	}

	return NOT_OK;
}

STD_ERROR USART_Configure(USART_cfg_t* USART_Cfg)
{
	u32 tempReg;

	USART1x->CR1 &= (~UE_ENABLE);			/* Disable USART1 first before reconfiguration */

	/* Configure CR1 Register */
	tempReg = USART1x->CR1;

	if(USART_Cfg->NumberOfDataBits == DATA_BITS_EIGHT)
		tempReg &= DATA_BITS_EIGHT;
	else
		tempReg |= DATA_BITS_NINE;

	switch(USART_Cfg->ParityBit)
	{
	case NO_PARITY:
		tempReg &= NO_PARITY;
		break;
	case EVEN_PARITY:
		tempReg |= PCE_PARITY_CONTROL;
		tempReg &= ~(PS_PARITY_SELECTION);							/* 0 -> Even parity	*/
		break;
	case ODD_PARITY:
		tempReg |= ( PCE_PARITY_CONTROL | PS_PARITY_SELECTION );	/* 1 -> Odd Parity */
		break;
	}
	USART1x->CR1 = tempReg;

	/* Configure CR2 Register */
	tempReg = USART1x->CR2;
	tempReg &= ~(STOP_BITS_CLEAR);
	tempReg |= USART_Cfg->NumberOfStopBits;
	USART1x->CR2 = tempReg;

	/* Configure Buad rate */
	/*----------------------*/

	USART1x->CR1 |= UE_ENABLE;			/* Enable USART1 again after reconfiguration */
	return OK;
}

STD_ERROR USART_Send(volatile u8* bufferData, u32 bytesCount)
{
	switch(USART_Init_Cfg.UartUsageMode)
	{
	case INTERRUPT_MODE:
		return (USART_SendByInter(bufferData, bytesCount));
		break;
	case DMA_MODE:
		return (USART_SendByDma(bufferData, bytesCount));
		break;
	case POLLING_MODE:
		break;
	}
	return OK;
}
STD_ERROR USART_SendByInter(volatile u8* bufferData, u32 bytesCount)
{
	u32 i;
	if((txBuffer.State == ideal)&&(bufferData)&&(bytesCount>0))
	{
		txBuffer.ptrToDataBuffer = bufferData;
		txBuffer.Size = bytesCount;
		txBuffer.Position = 0;
		txBuffer.State = busy;

		for(i=0;i<bytesCount;i++)										/* to save all data that will be transmitted */
			txBuffer.ptrToDataBuffer[i] = bufferData[i];


		USART1x->DR = txBuffer.ptrToDataBuffer[txBuffer.Position];		/* to make it enter USART handler first time */
		USART1x->CR1 |= ENABLE_TC_INT;
		txBuffer.Position++;

		return OK;
	}
	return NOT_OK;
}

STD_ERROR USART_SendByDma(volatile u8* bufferData, u32 bytesCount)
{
	if(DMASendState == DMA_ideal)
	{
		DMA_cfg_t DMA_cfg =
		{
				(u32)DMA1,
				4,
				(u32)&USART1x->DR,
				(u32)bufferData,
				PERI_SIZE_8_BITS,
				MEM_SIZE_8_BITS,
				0,
				1,
				bytesCount,
				DIR_MEM2PRE,
				0,
				LOW,
				0,
				USART_Init_Cfg.EnableDmaInt,
				USART_Init_Cfg.DisableDmaInt
		};

		if(DMA_Configure(&DMA_cfg))
			return NOT_OK;
		DMASendState = DMA_busy;

		if(appTxNotify)
			DMA_setDma1Ch4Cbf(appTxNotify);

		return OK;
	}
	else
		return NOT_OK;
}

STD_ERROR USART_Recieve(volatile u8* bufferData, u32 bytesCount)
{
	switch(USART_Init_Cfg.UartUsageMode)
	{
	case INTERRUPT_MODE:
		return (USART_RecieveByInter(bufferData, bytesCount));
		break;
	case DMA_MODE:
		return (USART_RecieveByDma(bufferData, bytesCount));
		break;
	case POLLING_MODE:
		break;
	}
	return OK;
}

STD_ERROR USART_RecieveByInter(volatile u8* bufferData, u32 bytesCount)
{
	if((rxBuffer.State == ideal)&&(bufferData)&&(bytesCount>0))
	{
		rxBuffer.ptrToDataBuffer = bufferData;
		rxBuffer.Size = bytesCount;
		rxBuffer.Position = 0;
		rxBuffer.State=busy;

		return OK;
	}
	return NOT_OK;
}

STD_ERROR USART_RecieveByDma(volatile u8* bufferData, u32 bytesCount)
{
	if(DMAReceiveState == DMA_ideal)
	{
		DMA_cfg_t DMA_cfg =
		{
				(u32)DMA1,
				5,
				(u32)&USART1x->DR,
				(u32)bufferData,
				PERI_SIZE_8_BITS,
				MEM_SIZE_8_BITS,
				0,
				1,
				bytesCount,
				DIR_PRE2MEM,
				0,
				LOW,
				0,
				USART_Init_Cfg.EnableDmaInt,
				USART_Init_Cfg.DisableDmaInt
		};

		if(DMA_Configure(&DMA_cfg))
			return NOT_OK;
		DMAReceiveState = DMA_busy;

		if(appRxNotify)
			DMA_setDma1Ch5Cbf(appRxNotify);

		return OK;
	}
	else
		return NOT_OK;
}

void USART1_IRQHandler (void)
{
	u32 tempToClear;
	/* for LIN Communication protocol */
	if((USART1x->SR & LBD_FLAG ) == LBD_FLAG)
	{
		USART1x->SR &= ~(LBD_FLAG);                 /* to clear the flag      */

		if(appBreakNotify)
			appBreakNotify();

	}
	if((USART1x->SR & RXNE_FLAG ) == RXNE_FLAG)
	{
		if(rxBuffer.State == busy)
		{
			rxBuffer.ptrToDataBuffer[rxBuffer.Position]=USART1x->DR;    /* first step to clear the flag */
			rxBuffer.Position++;

			if(rxBuffer.Position == rxBuffer.Size)
			{
				rxBuffer.Position =0;
				rxBuffer.Size =0;
				rxBuffer.ptrToDataBuffer =NULL;
				rxBuffer.State = ideal;

				if(appRxNotify)
					appRxNotify();
			}
		}
		else
			tempToClear = USART1x->DR;                  /* to clear the flag also */

	}

	if((USART1x->SR & ((u32)TC_FLAG) )==TC_FLAG)
	{
		USART1x->SR &= ~TC_FLAG;
		if(txBuffer.Position<txBuffer.Size && txBuffer.State == busy)
		{
			/* There is a data to be sent */
			USART1x->DR = txBuffer.ptrToDataBuffer[txBuffer.Position];
			txBuffer.Position++;
		}
		else
		{
			/* Transmission end */
			txBuffer.Position =0;
			txBuffer.Size =0;
			txBuffer.ptrToDataBuffer =NULL;
			txBuffer.State = ideal;
			USART1x->CR1 &= DISABLE_TC_INT;
			if(appTxNotify)
				appTxNotify();
		}
	}



}

STD_ERROR USART_setTxCbf (volatile txCbf_t txCbf)
{
	if(txCbf)
	{
		appTxNotify = txCbf;
		return OK;
	}

	return NOT_OK;
}

STD_ERROR USART_setRxCbf (volatile rxCbf_t rxCbf)
{
	if(rxCbf)
	{
		appRxNotify = rxCbf;
		return OK;
	}

	return NOT_OK;
}

STD_ERROR USART_setBreakCbf (volatile breakCbf_t breakCbf)
{
	if(breakCbf)
	{
		appBreakNotify = breakCbf;
		return OK;
	}

	return NOT_OK;
}

STD_ERROR USART_LIN_Init()
{
	/* Init your UART Communication to be used for LIN Communication 												 */
	USART1x->CR3 &= ~ (HD_SEL);									/* These bits must be cleared 						 */
	USART1x->CR3 &= ~ (SC_EN | IR_EN);

	USART1x->CR1 &= DATA_BITS_EIGHT;
	USART1x->CR2 &= ~(STOP_BITS_CLEAR);
	USART1x->CR2 &= ~ (CLK_EN);					/* some bits must be cleared for LIN communication					 */
	USART1x->CR2 |= LIN_EN;						/* for Enable LIN communication 									 */
	USART1x->CR2 |= LBDIE;						/* for Enable Break detection interrupt								 */
	return OK;
}



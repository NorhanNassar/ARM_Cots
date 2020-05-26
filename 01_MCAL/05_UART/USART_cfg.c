/*
 * USART_cfg.c
 *
 *  Created on: Mar 30, 2020
 *      Author: Norhan Nassar
 */

#include "STD_TYPES.h"
#include "GPIO.h"
#include "DMA.h"
#include "USART.h"

extern USART_Init_Cfg_t USART_Init_Cfg = {
		9600,
		DATA_BITS_EIGHT,
		NO_PARITY,
		(NO_INT_ENABLED),
		(DISABLE_IDLE_INT & DISABLE_TC_INT & DISABLE_PARITY_INT & DISABLE_TXE_INT & DISABLE_RXNE_INT),
		ONE_STOP_BIT,
		ASYNCHRONOUS,
		DISABLE_ERROR_INT,
		FULL_DUPLEX,
		DMA_MODE,
		RX_PIN_INPUT_PULLUP,
		TX_SPEED_50MHZ,
		TRANSFER_COMPLETE_INT_EN,
		TRANSFER_COMPLETE_INT_EN & HALF_TRANSFER_INT_EN
};
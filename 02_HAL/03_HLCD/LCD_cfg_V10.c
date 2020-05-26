/*
 * LCD_cfg_V10.c
 *
 *  Created on: Apr 12, 2020
 *      Author: Norhan Nassar
 */
#include "STD_TYPES.h"
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "LCD_cfg_V10.h"
#include "LCD_V10.h"

Lcd_Init_Cfg_t Lcd_Init_Cfg =
{
		TWO_LINES_MODE,
		DOT_5_7,
		DISPLAY_ON,
		CURSOR_OFF,
		BLINK_OFF,
		DEFAULT_INCREMENT_MODE,
		DEFAULT_DISPLAY_SHIFT
};

/* If it is 4 bit mode so map Data pins only from D4 to D7
 * it means that 4th element will be D4, 5th -> D5, 6th -> D6, 7th-> D7
 * and configure LCDPINS in cfg.h to be just 7 pins 					*/

/* For 8bit Mode 														*/
lcdMap_t lcdPins[LCDPINS]=
{
		[RS] = {PORTA,		GPIO_PIN0,		SPEED_2MHZ},				/* 1st element to configure RS Pin  */
		[RW] = {PORTA,		GPIO_PIN1,		SPEED_2MHZ},				/* 2nd element to configure RW Pin  */
		[E]  = {PORTA,		GPIO_PIN2,		SPEED_2MHZ},				/* 3d element to configure E Pin    */

		[D0] = {PORTA,		GPIO_PIN3,		SPEED_2MHZ},				/* 4th element to configure D0 Pin  */
		[D1] = {PORTA,		GPIO_PIN4,		SPEED_2MHZ},				/* 5th element to configure D1 Pin  */
		[D2] = {PORTA,		GPIO_PIN5,		SPEED_2MHZ},				/* 6th element to configure D2 Pin  */
		[D3] = {PORTA,		GPIO_PIN6,		SPEED_2MHZ},				/* 7th element to configure D3 Pin  */
		[D4] = {PORTA,		GPIO_PIN7,		SPEED_2MHZ},				/* 8th element to configure D4 Pin  */
		[D5] = {PORTA,		GPIO_PIN8,		SPEED_2MHZ},				/* 9th element to configure D5 Pin  */
		[D6] = {PORTA,		GPIO_PIN9,		SPEED_2MHZ},				/* 10th element to configure D6 Pin */
		[D7] = {PORTA,		GPIO_PIN10,		SPEED_2MHZ},				/* 11th element to configure D7 Pin */
};

u32 lcdPorts[LCDPORTSNUM] =
{
		RCC_GPIOA
};



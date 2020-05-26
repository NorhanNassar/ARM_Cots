/*
 * LCD_V10.h
 *
 *  Created on: Apr 12, 2020
 *      Author: Norhan Nassar
 */

#ifndef LCD_V10_H_
#define LCD_V10_H_

#if LCDMODE == EIGHT_BIT_MODE
typedef enum
{
	RS,
	RW,
	E,
	D0,
	D1,
	D2,
	D3,
	D4,
	D5,
	D6,
	D7
}lcdPins_t;

#else
typedef enum
{
	RS,
	RW,
	E,
	D4,
	D5,
	D6,
	D7
}lcdPins_t;
#endif


typedef struct
{
	u32 Port;
	u32 Pin;
	u32 Speed;
}lcdMap_t;

typedef struct
{
									/* To configure options for function set state at init process 			*/
	u8 numberOfLines;				/* Options: ONE_LINE_MODE, TWO_LINES_MODE 					   			*/
	u8 numberOfDots;				/* Options: DOT_5_7, DOT_5_10 			 					   			*/
									/* To configure options for display state at init process	   			*/
	u8 displayControl;				/* Options: DISPLAY_ON, DISPLAY_OFF							   			*/
									/* Note: if you choose DISPLAY_OFF nothing will appear on lcd 			*/
	u8 cursorControl;				/* Options: CURSOR_ON, CURSOR_OFF							   			*/
	u8 blinkControl;				/* Options: BLINK_ON, BLINK_OFF							   	   			*/
									/* to configure options for entry mode state at init process   			*/
	u8 incrementMode;				/* Options: INCREMENT_ENABLE, INCREMENT_DISABLE,DEFAULT_INCREMENT_MODE  */
	u8 displayShiftMode;			/* Options: DISPLAY_SHIFT_ENABLE, DISPLAY_SHIFT_DISABLE,
												DEFAULT_DISPLAY_SHIFT_MODE  								*/

}Lcd_Init_Cfg_t;

/*--------------------Configurations for Function Set State at init Process------------------------*/
/*---------------------------------8Bit Mode or 4Bit Mode -----------------------------------------*/
#define FOUR_BIT_MODE				0
#define EIGHT_BIT_MODE				1
/*--------------------------------Number of Dots Macros--------------------------------------------*/
/*---------------------------------Number of lines Macros------------------------------------------*/
#define ONE_LINE_MODE				0b00110000
#define TWO_LINES_MODE				0b00111000
/*--------------------------------Number of Dots Macros--------------------------------------------*/
#define DOT_5_7						0b00110000
#define DOT_5_10					0b00110100
/*-------------------------Configurations for Display State at init Process------------------------*/
/*-----------------------------------Display Modes Macros------------------------------------------*/
#define DISPLAY_OFF					0b00001000
#define DISPLAY_ON					0b00001100
/*------------------------------------Cursor Modes Macros------------------------------------------*/
#define CURSOR_OFF					0b00001000
#define CURSOR_ON					0b00001010
/*------------------------------------Blink Modes Macros------------------------------------------*/
#define BLINK_OFF					0b00001000
#define BLINK_ON					0b00001001
/*-------------------------Configurations for Entry Mode State at init Process---------------------*/
/*---------------------------------Increment Modes Macros------------------------------------------*/
#define INCREMENT_ENABLE			0b00000110
#define INCREMENT_DISABLE			0b00000100
#define DEFAULT_INCREMENT_MODE		INCREMENT_ENABLE
/*-------------------------------Display Shift Modes Macros----------------------------------------*/
#define DISPLAY_SHIFT_ENABLE		0b00000101
#define DISPLAY_SHIFT_DISABLE		0b00000100
#define DEFAULT_DISPLAY_SHIFT		DISPLAY_SHIFT_DISABLE
/*-------------------------------------------------------------------------------------------------*/

/* Choose which line when use sePosition function */
#define FIRST_LINE					0b10000000
#define SECOND_LINE					0b11000000

/*-----------------------------------------typedef-------------------------------*/
typedef void (*notify_t) (void);

extern STD_ERROR LCD_Init (void);
extern STD_ERROR lcd_writeStringAsy(const char string[],u8 len);
extern STD_ERROR lcd_ClearAsy(void);
extern STD_ERROR lcd_setPositionAsy(u8 xPos, u8 yPos);
extern STD_ERROR lcd_setupCursorAsy(u8 cursorState);
extern STD_ERROR lcd_setupBlinkAsy(u8 blinkState);
extern STD_ERROR lcd_setPositionDoneNotify(notify_t appCbf);
extern STD_ERROR lcd_setWriteDoneNotify(notify_t appCbf);
extern STD_ERROR lcd_setClearDoneNotify(notify_t appCbf);


#endif /* LCD_V10_H_ */

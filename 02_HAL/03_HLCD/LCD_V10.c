/*
 * LCD_V10.c
 *
 *  Created on: Apr 12, 2020
 *      Author: Norhan Nassar
 */
#include "STD_TYPES.h"
#include "Sched.h"
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "LCD_cfg_V10.h"
#include "LCD_V10.h"

/*----------------------------------------defines--------------------------------*/
#define LCD_PERIODICITY								2
#define CLEAR_SCREEN								0b00000001
#define MOVE_CURSOR_AT_FIRST_LINE					0b10000000
#define MOVE_CURSOR_AT_SECOND_LINE 					0b11000000

typedef enum
{
	idle_proc,
	write_proc,
	clear_proc,
	setPosition_proc,
	setup_proc
}lcdProcess_t;

typedef enum
{
	functionSetState,
	displayONOFFState,
	displayClearState,
	entryModeState
}initState_t;

typedef enum
{
	setFirstPart,
	setSecondPart,
	setfunctionSetStatePart,
}initStateParts_t;

/*-------------------------------------------static variables-------------------------------------------------*/
notify_t appWriteDoneNotify;
notify_t appSetPositionDoneNotify;
notify_t appClearDoneNotify;

static lcdProcess_t curProcess;							 /* static variable for current process running       */

static u8 functionSetCfg;								 /* static variables for init process			      */
static u8 displayOnOffCfg;
static u8 enteryModeCfg;

static u8 initCompleted;								/* flag to indicate that init process is completed    */

static char userWord[255];							    /* static variables for write operation 		 	  */
static u8 userLen;

static u8 setXPos;										/* static variables for set position operation 		  */
static u8 setYPos;

static u8 cursorSetupState;								/* static variable to setup the cursor state */
static u8 blinkSetupState;

extern Lcd_Init_Cfg_t Lcd_Init_Cfg;
extern lcdMap_t lcdPins[LCDPINS];
extern lcdPorts[LCDPORTSNUM];
/*-------------------------------------------------static functions-------------------------------------------*/
static STD_ERROR writeCommand(u8 command);									/* to write command on data pins  */
static STD_ERROR writeData(u8 data);										/* to write data on data pins 	  */
static void initProcess8BitMode(void);
static void initProcess4BitMode(void);
static void writeProcess8Bit(void);											/* to init process for 8 bit mode */
static void writeProcess4Bit(void);											/* to init process for 4 bit mode */
static void writeClear8Bit(void);
static void writeClear4Bit(void);
static void setPosition8Bit(void);
static void setPosition8Bit(void);
static void setupProcess8Bit(void);

/* Description: to init Lcd and configure its pins 															  */
STD_ERROR LCD_Init (void)
{
	STD_ERROR loc_err = OK;
	u8 i;
	GPIO_Conf_Pin_t configPin;

	for(i=0;i<LCDPORTSNUM;i++)			/* initialize clock for lcd ports first */
		RCC_SetPort(lcdPorts[i],ON);

	for(i=0;i<LCDPINS;i++)				/* number of lcd pins is 11 for 8 bit mode and 7 for 4 bit mode      */
	{
		configPin.Port = lcdPins[i].Port;
		configPin.Pin = lcdPins[i].Pin;
		configPin.Mode_Configure = MODE_OP_GP_PP;
		configPin.Mode_Speed = lcdPins[i].Speed;
		loc_err |= GPIO_voidConfigure(&configPin);
	}

	GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,RESET);			/* initialize enable pin to 0 */

	functionSetCfg = Lcd_Init_Cfg.numberOfLines | Lcd_Init_Cfg.numberOfDots;
	displayOnOffCfg = Lcd_Init_Cfg.displayControl | Lcd_Init_Cfg.cursorControl | Lcd_Init_Cfg.blinkControl;
	enteryModeCfg = Lcd_Init_Cfg.incrementMode | Lcd_Init_Cfg.displayShiftMode;
	cursorSetupState = Lcd_Init_Cfg.cursorControl;				/* initialize cursor setup state to init state at cfg struct */
	blinkSetupState = Lcd_Init_Cfg.blinkControl;

	return loc_err;
}
/**************************************************************************************************************/
/* Description: to write String on Lcd, it takes array of charters and length								  */
STD_ERROR lcd_writeStringAsy(const char string[],u8 len)
{
	u8 i;
	if(curProcess == idle_proc)
	{
		curProcess = write_proc;
		userLen = len;
		for(i=0;i<len;i++)
		{
			userWord[i] = string[i];
		}
		return OK;
	}
	else
		return BUSY;
}
/***************************************************************************************************************/
STD_ERROR lcd_ClearAsy(void)
{
	if(curProcess==idle_proc)
	{
		curProcess = clear_proc;
		return OK;
	}
	else
		return BUSY;
}
/**************************************************************************************************************/
STD_ERROR lcd_setPositionAsy(u8 xPos, u8 yPos)
{
	if( (yPos!=FIRST_LINE)&&(yPos!=SECOND_LINE) )
		return NOT_OK;
	if(curProcess==idle_proc)
	{
		curProcess = setPosition_proc;
		setXPos = xPos;
		setYPos = yPos;
		return OK;
	}
	else
		return BUSY;
}
/**************************************************************************************************************/
STD_ERROR lcd_setupCursorAsy(u8 cursorState)
{
	if((cursorState!=CURSOR_ON)&&(cursorState!=CURSOR_OFF))
		return NOT_OK;
	if(curProcess==idle_proc)
	{
		cursorSetupState = cursorState;
		curProcess = setup_proc;
		return OK;
	}
	else
		return BUSY;
}
/**************************************************************************************************************/
STD_ERROR lcd_setupBlinkAsy(u8 blinkState)
{
	if((blinkState!=BLINK_ON)&&(blinkState!=BLINK_OFF))
		return NOT_OK;
	if(curProcess==idle_proc)
	{
		blinkSetupState = blinkState;
		curProcess = setup_proc;
		return OK;
	}
	else
		return BUSY;
}
/**************************************************************************************************************/
/* Description: to notify app that set position process is done on Lcd										  */
STD_ERROR lcd_setPositionDoneNotify(notify_t appCbf)
{
	if(appCbf)
	{
		appSetPositionDoneNotify = appCbf ;
		return OK;
	}

	return NOT_OK;
}
/**************************************************************************************************************/
/* Description: to notify app that write process is done on Lcd												  */
STD_ERROR lcd_setWriteDoneNotify(notify_t appCbf)
{
	if(appCbf)
	{
		appWriteDoneNotify = appCbf ;
		return OK;
	}

	return NOT_OK;
}
/**************************************************************************************************************/
/* Description: to notify app that set clear process is done on Lcd											  */
STD_ERROR lcd_setClearDoneNotify(notify_t appCbf)
{
	if(appCbf)
	{
		appClearDoneNotify = appCbf ;
		return OK;
	}

	return NOT_OK;
}
/**************************************************************************************************************/
/* Description: Lcd Runnable task																			  */
void lcd_task(void)
{
	if(initCompleted)
	{
		switch(curProcess)
		{
		case idle_proc:
			break;
		case write_proc:
#if LCDMODE == EIGHT_BIT_MODE
			writeProcess8Bit();
#elif LCDMODE == FOUR_BIT_MODE
			writeProcess4Bit();
#endif
			break;
		case clear_proc:
#if LCDMODE == EIGHT_BIT_MODE
			writeClear8Bit();
#elif LCDMODE == FOUR_BIT_MODE
			writeClear4Bit();
#endif
			break;
		case setPosition_proc:
#if LCDMODE == EIGHT_BIT_MODE
			setPosition8Bit();
#elif LCDMODE == FOUR_BIT_MODE
			setPosition4Bit();
#endif
			break;
		case setup_proc:
			setupProcess8Bit();
			break;
		}
	}
	else
	{
#if LCDMODE == EIGHT_BIT_MODE
		initProcess8BitMode();
#elif LCDMODE == FOUR_BIT_MODE
		initProcess4BitMode();
#endif

	}
}
/**************************************************************************************************************/
/* Description: static function for init Lcd with 8 bit Mode 												  */
void initProcess8BitMode(void)
{
	static u8 initCounter;													/* to indicate that 40 msec is passed */
	static initState_t currentState;

	u8 enablePinState;

	if(initCounter >= (40/LCD_PERIODICITY) )								/* after 40 m sec */
	{
		GPIO_u8Read(lcdPins[E].Port,lcdPins[E].Pin,&enablePinState);
		if(enablePinState)
		{
			GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,RESET);				/* Set Enable Pin to 0 */
			currentState++;
			if(currentState > entryModeState)			/* if you reach last state initComplete flag will be 1 */
				initCompleted = 1;
			return;
		}
		else
		{
			GPIO_u8Write(PORTC,GPIO_PIN13,RESET);
			switch(currentState)
			{
			case functionSetState:
				writeCommand(functionSetCfg);
				GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,SET);				/* Set Enable Pin to 1 */
				break;
			case displayONOFFState:
				writeCommand(displayOnOffCfg);
				GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,SET);				/* Set Enable Pin to 1 */
				break;
			case displayClearState:
				writeCommand(CLEAR_SCREEN);
				GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,SET);				/* Set Enable Pin to 1 */
				break;
			case entryModeState:
				writeCommand(enteryModeCfg);
				GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,SET);				/* Set Enable Pin to 1 */
				break;
			}
		}
	}
	else
		initCounter++;
}
/**************************************************************************************************************/
/* Description: static function for init Lcd with 4 bit Mode 												  */
void initProcess4BitMode(void)
{
	static u8 initCounter;											/* to indicate that 40 msec is passed */
	static initState_t currentState;
	static initStateParts_t statePartscounter;
	u8 init4BitMode = 0b00100010;									/* to init lcd that it's in 4bit mode */
	u8 initDisplayClear = CLEAR_SCREEN;

	u8 enablePinState;

	if(initCounter >= (40/LCD_PERIODICITY) )								/* after 40 m sec 	  */
	{
		GPIO_u8Read(lcdPins[E].Port,lcdPins[E].Pin,&enablePinState);
		if(enablePinState)
		{
			statePartscounter++;
			GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,RESET);					/* Set Enable Pin to 0 */
			if( (currentState==functionSetState)&&(statePartscounter>setfunctionSetStatePart) )
			{
				statePartscounter=0;
				currentState++;
			}
			else if((currentState!=functionSetState)&&(statePartscounter>setSecondPart))
			{
				statePartscounter=0;
				currentState++;
			}

			if(currentState > entryModeState)			/* if you reach last state initComplete flag will be 1 */
				initCompleted = 1;

			return;
		}
		switch(currentState)
		{
		case functionSetState:
			switch(statePartscounter)
			{
			case setFirstPart:
				writeCommand(init4BitMode);
				init4BitMode = init4BitMode >> 4;
				break;
			case setSecondPart:
				writeCommand(init4BitMode);
				break;
			case setfunctionSetStatePart:
				writeCommand(functionSetCfg);
				break;
			}
			GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,SET);				/* Set Enable Pin to 1 */
			break;
			case displayONOFFState:
				switch(statePartscounter)
				{
				case setFirstPart:
					writeCommand(displayOnOffCfg);
					displayOnOffCfg = displayOnOffCfg >> 4;
					break;
				case setSecondPart:
					writeCommand(displayOnOffCfg);
					break;
				}
				GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,SET);				/* Set Enable Pin to 1 */
				break;
				case displayClearState:
					switch(statePartscounter)
					{
					case setFirstPart:
						writeCommand(CLEAR_SCREEN);
						initDisplayClear = initDisplayClear >> 4;
						break;
					case setSecondPart:
						writeCommand(initDisplayClear);
						break;
					}
					GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,SET);				/* Set Enable Pin to 1 */
					break;
					case entryModeState:
						switch(statePartscounter)
						{
						case setFirstPart:
							writeCommand(enteryModeCfg);
							enteryModeCfg = enteryModeCfg >> 4;
							break;
						case setSecondPart:
							writeCommand(enteryModeCfg);
							break;
						}
						GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,SET);				/* Set Enable Pin to 1 */
						break;
		}
	}
	else
		initCounter++;
}
/**************************************************************************************************************/
/* Description: static function for write string process on Lcd with 8 bit Mode 							  */
void writeProcess8Bit(void)
{
	u8 enablePinState;
	static u8 curChar;
	GPIO_u8Read(lcdPins[E].Port,lcdPins[E].Pin,&enablePinState);
	if(enablePinState)
	{
		GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,RESET);				/* Set Enable Pin to 0 */
	}
	else
	{
		if(curChar==userLen)
		{
			curChar = 0;
			curProcess = idle_proc;
			if(appWriteDoneNotify)
				appWriteDoneNotify();
		}
		else
		{
			writeData(userWord[curChar]);
			curChar++;
			GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,SET);			/* Set Enable Pin to 1 */
		}
	}
}
/**************************************************************************************************************/
/* Description: static function for write string process on Lcd with 4 bit Mode 							  */
void writeProcess4Bit(void)
{
	u8 enablePinState;
	static u8 curChar;
	static initStateParts_t counter;
	GPIO_u8Read(lcdPins[E].Port,lcdPins[E].Pin,&enablePinState);
	if(enablePinState)
	{
		GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,RESET);				/* Set Enable Pin to 0 */
	}
	else
	{
		if(curChar==userLen)
		{
			curChar = 0;
			curProcess = idle_proc;
			if(appWriteDoneNotify)
				appWriteDoneNotify();
		}
		else
		{
			switch(counter)
			{
			case setFirstPart:
				writeData(userWord[curChar]);
				userWord[curChar] = userWord[curChar] >> 4;
				break;
			case setSecondPart:
				writeData(userWord[curChar]);
				curChar++;
				break;
			}
			GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,SET);			/* Set Enable Pin to 1 */
		}
	}
}
/**************************************************************************************************************/
/* Description: static function for clear process on Lcd with 8 bit Mode 							  		  */
void writeClear8Bit(void)
{
	u8 enablePinState;
	GPIO_u8Read(lcdPins[E].Port,lcdPins[E].Pin,&enablePinState);
	if(enablePinState)
	{
		GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,RESET);				/* Set Enable Pin to 0 */
		curProcess = idle_proc;
	}
	else
	{
		writeCommand(CLEAR_SCREEN);
		GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,SET);				/* Set Enable Pin to 1 */
	}

}
/**************************************************************************************************************/
/* Description: static function for clear process on Lcd with 4 bit Mode 							 	      */
void writeClear4Bit(void)
{
	u8 initDisplayClear = CLEAR_SCREEN;
	u8 enablePinState;
	static initStateParts_t statePartscounter;
	GPIO_u8Read(lcdPins[E].Port,lcdPins[E].Pin,&enablePinState);
	if(enablePinState)
	{
		GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,RESET);				/* Set Enable Pin to 0 */
		curProcess = idle_proc;
	}
	else
	{
		switch(statePartscounter)
		{
		case setFirstPart:
			writeCommand(initDisplayClear);
			initDisplayClear = initDisplayClear >> 4;
			break;
		case setSecondPart:
			writeCommand(initDisplayClear);
			break;
		}
		GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,SET);				/* Set Enable Pin to 1 */
	}
}
/**************************************************************************************************************/
/* Description: static function for set position process on Lcd with 8 bit Mode 							  */
void setPosition8Bit(void)
{
	u8 enablePinState;
	GPIO_u8Read(lcdPins[E].Port,lcdPins[E].Pin,&enablePinState);
	if(enablePinState)
	{
		GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,RESET);				/* Set Enable Pin to 0 */
		curProcess = idle_proc;
	}
	else
	{
		writeCommand(setYPos+setXPos);
		GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,SET);				/* Set Enable Pin to 1 */
	}

}
/**************************************************************************************************************/
/* Description: static function for set position process on Lcd with 4 bit Mode 							  */
void setPosition4Bit(void)
{
	u8 enablePinState;
	u8 command = setXPos+setYPos;
	static initStateParts_t statePartscounter;
	GPIO_u8Read(lcdPins[E].Port,lcdPins[E].Pin,&enablePinState);
	if(enablePinState)
	{
		GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,RESET);				/* Set Enable Pin to 0 */
		curProcess = idle_proc;
	}
	else
	{
		writeCommand(setYPos+setXPos);
		switch(statePartscounter)
		{
		case setFirstPart:
			writeCommand(command);
			command = command >> 4;
			break;
		case setSecondPart:
			writeCommand(command);
			break;
		}
		GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,SET);				/* Set Enable Pin to 1 */
	}

}
/**************************************************************************************************************/
/* Description: static function for setup Lcd cursor or blink Mode with 8 bit Mode 							  */
void setupProcess8Bit(void)
{
	u8 enablePinState;
	GPIO_u8Read(lcdPins[E].Port,lcdPins[E].Pin,&enablePinState);
	if(enablePinState)
	{
		GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,RESET);				/* Set Enable Pin to 0 */
		curProcess = idle_proc;
	}
	else
	{
		writeCommand(Lcd_Init_Cfg.displayControl|blinkSetupState|cursorSetupState);
		GPIO_u8Write(lcdPins[E].Port,lcdPins[E].Pin,SET);				/* Set Enable Pin to 1 */
	}

}
/**************************************************************************************************************/
/* Description: static function for write command on Lcd, it works for 8 bit and 4 bit Modes 				  */
STD_ERROR writeCommand(u8 command)
{
	STD_ERROR loc_err = OK;
	u8 i;
	loc_err |= GPIO_u8Write(lcdPins[RS].Port,lcdPins[RS].Pin,RESET);		/* Set RS = 0 	  */
	loc_err |= GPIO_u8Write(lcdPins[RW].Port,lcdPins[RW].Pin,RESET);		/* Set RW = 0 	  */

	for(i=3;i<LCDPINS;i++)					/* First three elements in array for RS, RW and E pins */
	{
																					/* at GPIO Driver 		*/
		loc_err |= GPIO_u8Write(lcdPins[i].Port,lcdPins[i].Pin, !(command&1) );		/* to SET pin write 0   */
																					/* to RESET pin write 1 */
		command = command>>1;
	}
	return loc_err;

}
/**************************************************************************************************************/
/* Description: static function for write data process on Lcd, it works for 8 bit and 4 bit Modes 			  */
STD_ERROR writeData(u8 data)
{
	STD_ERROR loc_err = OK;
	u8 i;
	loc_err |= GPIO_u8Write(lcdPins[RS].Port,lcdPins[RS].Pin,SET);				/* Set RS = 1 	  */
	loc_err |= GPIO_u8Write(lcdPins[RW].Port,lcdPins[RW].Pin,RESET);			/* Set RW = 0 	  */

	for(i=3;i<LCDPINS;i++)					/* First three elements in array for RS, RW and E pins */
	{
																				/* at GPIO Driver 		*/
		loc_err |= GPIO_u8Write(lcdPins[i].Port,lcdPins[i].Pin, !(data&1) );	/* to SET pin write 0   */
																				/* to RESET pin write 1 */
		data = data>>1;
	}
	return loc_err;

}

task_t const LcdTask = {&lcd_task,3};

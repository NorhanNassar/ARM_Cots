#ifndef SYSTICK_H
#define SYSTICK_H

#define SYSTICK_BASE_ADDRESS     (void*)0xE000E010


typedef struct{
	u32 CTRL;
	u32 LOAD;
	u32 VAL;
	u32 CALIB;
} SYSTICK_t;

#define SYSTICKx		((SYSTICK_t volatile * const)SYSTICK_BASE_ADDRESS)

/* For enable or disable SysTick exception */

#define SYSTICK_EXCEPTION_MASK				0x02
#define SYSTICK_ENABLE_MASK					0x01
#define SYSTICK_PRESCALLER_MASK				0x04
#define SYSTICK_COUNTERFLAG_MASK			0x10000

#define	AHB_8					~(SYSTICK_PRESCALLER_MASK)
#define AHB						SYSTICK_PRESCALLER_MASK

#define	ENABLE					SYSTICK_EXCEPTION_MASK
#define DISABLE					~(SYSTICK_EXCEPTION_MASK)

#define VTOR_CODE					0
#define VTOR_SRAM					1

/* to check bit number 31 at STK_CALIB to detect if external clock is available at this chip or not */
#define EXTERNAL					0x7FFFFFFF
/* just mask to indicate that at cfg we choose internal clock */
#define INTERNAL					0x80000000

#define CHECKPRESCALER(PRESCALER)			((PRESCALER==AHB)||(PRESCALER==(AHB_8)))
#define CHECKINIT()							(((STK_EXCEPTION==ENABLE)||(STK_EXCEPTION==DISABLE))&&((STK_INIT_PRESCALLER==AHB)||(STK_INIT_PRESCALLER==AHB_8)))

/* This function to init th systick timer according to the systick cfg file
 * - Systick exception enabled or disabled
 * - Init Prescaler as AHB or AHB/8 */
extern STD_ERROR SYSTICK_Init(void);
/* To enable SYSTICK */
extern STD_ERROR SYSTICK_Start(void);
/* Set dessired time to count in micro sec */
extern STD_ERROR SYSTICK_SetTime(u32 TimeMs);
/* disable Systick */
extern STD_ERROR SYSTICK_Stop(void);
/* Timer must be stopped to set the callback function
 * if it is enabled return Error */
extern STD_ERROR SYSTICK_SetCallBack(void(*Copy_PtrCbf)(void));
/* To change Prescaler
 * Parameters :
 * ClockSrc -> AHB or AHB_8 */
extern STD_ERROR SYSTICK_SetPrescaller(s32 ClockSrc);
/* System Exception function */
extern STD_ERROR SysTick_Handler(void);

#endif


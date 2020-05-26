/* To enable or disable SysTick exception */
#define			STK_EXCEPTION						ENABLE

/* Choose which PRESCALLER to init your  SYSTICK with */
#define 		STK_INIT_PRESCALLER					AHB_8

/* Choose Clock source for SYSTICK
 * options : INTERNAL -> for internal clock
 * Or EXTERNAL -> for external clock
 * If the chip does not support external clock init sunction wil return NOT_OK */
#define 		STK_CLOCKSOURCE						INTERNAL

/* If you choose clock source to be an external clock, Write down the value of it
 * If you Choose clock source to be internal so write INTERNAL */
#define 		STK_CLOCKSOURCE_EXTERNAL_MHZ		INTERNAL

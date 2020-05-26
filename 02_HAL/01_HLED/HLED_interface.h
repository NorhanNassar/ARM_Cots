/************************************************/
/*  Author : Norhan Nassar    			     	*/
/*  Version : V02          						*/
/************************************************/

/* it defines leds State 									*/
/* LED_ACTIVE_HIGH -> Write 1 on GPIO Pin to turn led on 	*/
/* LED_ACTIVE_LOW  -> Write 0 on GPIO Pin to turn led on 	*/
#ifndef HLED_INTERFACE_H
#define HLED_INTERFACE_H

typedef 	u8 	LED_STATE;
#define		LED_ACTIVE_HIGH					SET
#define		LED_ACTIVE_LOW					RESET

/* To turn Led ON or OFF */
#define  	LED_STATE_ON					SET
#define  	LED_STATE_OFF					RESET

#define 	CLEAR							0x00000000

/* Struct o each Led 																		*/
/* It contains: Led Number, and which pin connected to and it is active high or active low 	*/
typedef struct
{
	u16 Led_Num;
	u16 Led_Pin;
	u8 LED_STATE;
}Led_Pin_t;

/* Struct to each Port which connect to it some leds										*/
/* It contains: Leds_Port -> Port which leds connected to									*/
/* Leds_Port_Num -> Number of leds at each port  											*/
/* Leds_Port_Pins -> Pointer to Led_Pin_t struct which leds mapped to each pin at it 		*/
typedef struct
{
	u32 Leds_Port;
	u8  Leds_Port_Num;
	Led_Pin_t* Leds_Port_Pins;
}Leds_Port_t;

/******************************************************************************************/
/* Description: This API shall initialize all leds at configuration file to be general    */
/* output push pull with 2MHZ speed for each port 										  */
/******************************************************************************************/
extern STD_ERORR Led_init(void);

/******************************************************************************************/
/* Description: This API shall turn led ON or OFF according to argument state 		      */
/* Each led has its state to be ON -> Active High or Active Low						      */
/* Arguments:																		      */
/* Copy_u8LedNum : Led Number to turn it ON or OFF 										  */
/* Copy_u8state  : LED_STATE_ON, LED_STATE_OFF 											  */
/******************************************************************************************/
extern STD_ERORR Led_SetPinState(u8 Copy_u8LedNum,u8 Copy_u8State);

#endif

/*
 * LIN.c
 *
 *  Created on: May 3, 2020
 *      Author: Norhan Nassar
 */

#include "STD_TYPES.h"
#include "USART.h"
#include "LIN_cfg.h"
#include "LIN.h"

typedef struct
{
	u8 Msg_ID;
	u8 Time_to_Send;								/* Time to send given number of bytes 						     */
	Slave_ID_t Sender;								/* who is the sender for this msg 								 */
	Slave_ID_t Receivers[MAX_NUM_OF_RECEIVERS];		/* Array of receivers for each msg 								 */
}Msg_Info_t;

static u8 masterTaskPeriodicity;
static Msg_Info_t Messeges_info[NUM_OF_MSGS];

extern Msg_cfg_t Messeges_cfg[NUM_OF_MSGS];
extern Slave_ID_t Slave_ID;
extern Node_Type_t	Node_Type;

STD_ERROR LIN_Init(void)
{
	u8 MsgIdx;
	if(!USART_Init())
		return NOT_OK;
	/* Init your UART Communication to be used for LIN Communication 												 */
	if( (USART1x->CR3 & HD_SEL != HALF_DUPLEX)||(USART1x->CR2 & ONE_STOP_BIT != ONE_STOP_BIT)||(USART1x->CR1 & DATA_BITS_EIGHT != DATA_BITS_EIGHT))
		return NOT_OK;							/* UART must be configured to Hulf_dublex & 1 Stop bit & 8 bit data	 */

	USART1x->CR2 &= ~ (CLK_EN);					/* some bits must be cleared for LIN communication					 */
	USART1x->CR3 &= ~ (SC_EN | IR_EN);
	USART1x->CR2 |= LIN_EN;						/* for Enable LIN communication 									 */
	USART1x->CR2 |= LBDIE;						/* for Enable Break detection interrupt								 */
	/* LBDL die ele bet5tar hwa 10 bits wala 11 mosh ha7otha hena ya3ny hasbha by default w ma3rafsh da sa7 wala la2 */
	/* Keda mosh ba2y 8er ele bt send el break w die el mafrod lma agy ab3at el header 								 */
	/*****************************************************************************************************************/
	/* ba3d keda nshof ba2a hwa master wala la2 3shan lw master yb2a n7toloh ba2y el ma3lomat 						 */
	if(Node_Type == MASTER)
	{
		for(MsgIdx=0; MsgIdx<NUM_OF_MSGS; MsgIdx++)
		{
			Messeges_info[MsgIdx].Msg_ID = Messeges_cfg[MsgIdx].Msg_ID;
			Messeges_info[MsgIdx].Sender = Messeges_cfg[MsgIdx].Sender;
			Messeges_info[MsgIdx].Receivers = Messeges_cfg[MsgIdx].Receivers;
			Messeges_info.Time_to_Send = ( (Messeges_cfg[MsgIdx].No_of_bytes * 8) / USART_Init_Cfg.BaudRate ) * 1000 ;		/* 8: to convert byte to bits 	   */
																															/* 1000: to convert it to milli    */
		}
	}

}

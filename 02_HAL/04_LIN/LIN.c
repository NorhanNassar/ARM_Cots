/*
 * LIN.c
 *
 *  Created on: May 3, 2020
 *      Author: Norhan Nassar
 */

#include "STD_TYPES.h"
#include "Sched_cfg.h"
#include "Sched.h"
#include "USART.h"
#include "LIN_cfg.h"
#include "LIN.h"

typedef struct
{
	u8 Msg_ID;
	u8 Num_Of_Bytes;								/* Number of bytes to send at each message 						  */
	u16 Ticks_to_Send;								/* Time to send given number of bytes 						      */
	Slave_ID_t Sender;								/* who is the sender for this msg 								  */
	Slave_ID_t Receivers[MAX_NUM_OF_RECEIVERS];		/* Array of receivers for each msg 								  */
	u8 Msg_PID;										/* PID Message to send at header 								  */
	u8 Num_Of_Receivers;							/* Number of Receivers for each Message 						  */
	u8 Msg_buffer[9];								/* Maximum 8 data bytes and 1 byte for CRC						  */
	STD_ERROR Msg_Error_State;						/* if CRC is correct-> message is OK, if not -> message is NOT_OK */
}Msg_Info_t;

static volatile Msg_Info_t Messages_info[NUM_OF_MSGS];

static volatile u8 SendMsgIdx;
static u8 LIN_Header_Recieve[2];							/* SynchByte & PID 		  */
static u8 LIN_Header[2];
static volatile u8 RecMsgIdx;								/* Received Message Index */
static volatile u8 slaveSend;
static volatile u8 messegeIndex;

extern volatile Msg_cfg_t Messages_cfg[NUM_OF_MSGS];

static void LIN_CalcCRC(void);
static STD_ERROR LIN_CheckCRC(void);
static void LIN_Check(void);
static void LIN_SlaveTask(void);
static void Master_Task(void);
static void Slave_Task(void);

STD_ERROR LIN_Init(void)
{
	u8 MsgIdx;
	u8 recIdx;
	u8 oddParity, evenParity;					/* Parity bits at PID byte for each message 						 */

	if(USART_Init())
		return NOT_OK;

	USART_LIN_Init();							/* Init your UART Communication to be used for LIN Communication 	 */

	if(NODETYPE == MASTER)
	{
		for(MsgIdx=0; MsgIdx<NUM_OF_MSGS; MsgIdx++)
		{
			/* P0 (even parity) is the result of logic “XOR” between ID0, ID1, ID2 and ID4							*/
			/* P1 (odd parity) is the inverted result of logic “XOR” between ID1, ID3, ID4 and ID5.					*/
			evenParity = ((Messages_info[MsgIdx].Msg_ID >> 0) ^ (Messages_info[MsgIdx].Msg_ID >> 1)) & \
					((Messages_info[MsgIdx].Msg_ID >> 2) ^ (Messages_info[MsgIdx].Msg_ID >> 4));
			oddParity = (!((Messages_info[MsgIdx].Msg_ID >> 1) ^ (Messages_info[MsgIdx].Msg_ID >> 3)) & \
					(!((Messages_info[MsgIdx].Msg_ID >> 4) ^ (Messages_info[MsgIdx].Msg_ID >> 5)) ));

			Messages_info[MsgIdx].Msg_ID = Messages_cfg[MsgIdx].Msg_ID;
			Messages_info[MsgIdx].Num_Of_Bytes = Messages_cfg[MsgIdx].Num_Of_Bytes;
			Messages_info[MsgIdx].Num_Of_Receivers = Messages_cfg[MsgIdx].Num_Of_Receivers;
			Messages_info[MsgIdx].Sender = Messages_cfg[MsgIdx].Sender;
			for(recIdx=0; recIdx<Messages_info[MsgIdx].Num_Of_Receivers; recIdx++)
				Messages_info[MsgIdx].Receivers[recIdx] = Messages_cfg[MsgIdx].Receivers[recIdx];
			Messages_info[MsgIdx].Ticks_to_Send = Messages_cfg[MsgIdx].Time_Slot / MASTER_TASK_PERIODICITY;

			Messages_info[MsgIdx].Msg_PID = Messages_info[MsgIdx].Msg_ID & (~(3<<6));	/* to make sure that bits P0 & P1 location in PID is cleared first  */
			Messages_info[MsgIdx].Msg_PID |= (evenParity<<6) | (oddParity<<7);			/* Set even and odd parity bits at the PID message byte 			*/
			Messages_info[MsgIdx].Msg_Error_State = NOT_OK;
		}
	}
	return OK;
}

STD_ERROR LIN_Send(volatile u8* msgBufferData, u8 bytesCount,u8 messageID)
{
	u8 msgIndex;
	u8 bytesIndex;
	for(msgIndex=0; msgIndex<NUM_OF_MSGS; msgIndex++)
	{
		if((Messages_info[msgIndex].Msg_ID == messageID) && (Messages_info[msgIndex].Num_Of_Bytes == bytesCount) )
		{
			for(bytesIndex=0; bytesIndex<bytesCount; bytesIndex++) /* set message data without CRC */
			{
				Messages_info[msgIndex].Msg_buffer[bytesIndex] = msgBufferData[bytesIndex];
			}
			return OK;
		}
	}
	return NOT_OK;
}

STD_ERROR LIN_Recieve(volatile u8* msgBufferData, u8 bytesCount,u8 messageID)
{
	u8 msgIndex;
	u8 bytesIndex;
	for(msgIndex=0; msgIndex<NUM_OF_MSGS; msgIndex++)
	{
		if( (Messages_info[msgIndex].Msg_ID == messageID) && (Messages_info[msgIndex].Num_Of_Bytes == bytesCount) && \
				(Messages_info[msgIndex].Msg_Error_State == OK) )
		{

			for(bytesIndex=0; bytesIndex<bytesCount; bytesIndex++) /* to set message data without CRC */
			{
				msgBufferData[bytesIndex] = Messages_info[msgIndex].Msg_buffer[bytesIndex];
			}
			return OK;
		}
	}
	return NOT_OK;
}

void LIN_CalcCRC(void)
{
	u8 byteIndx;
	u8 CRC = Messages_info[SendMsgIdx].Msg_buffer[0];
	for(byteIndx=1; byteIndx<Messages_info[SendMsgIdx].Num_Of_Bytes; byteIndx++)				/* last byte for CRC 		*/
	{
		CRC = CRC ^ Messages_info[SendMsgIdx].Msg_buffer[byteIndx];								/* CRC = !(D0 ^ D1 ^ D2 ..)	*/
	}
	Messages_info[SendMsgIdx].Msg_buffer[byteIndx] = ~(CRC);									/* last byte for CRC 		*/
}

STD_ERROR LIN_CheckCRC(void)
{
	u8 byteIndx;
	s8 CRC = Messages_info[RecMsgIdx].Msg_buffer[0];
	for(byteIndx=1; byteIndx<Messages_info[RecMsgIdx].Num_Of_Bytes; byteIndx++)					/* last byte for CRC 		*/
	{
		CRC = CRC ^ Messages_info[RecMsgIdx].Msg_buffer[byteIndx];								/* CRC = !(D0 ^ D1 ^ D2 ..)	*/
	}
	if(Messages_info[RecMsgIdx].Msg_buffer[byteIndx] == (~(CRC)) )								/* last byte for CRC 		*/
		return OK;
	else
		return NOT_OK;
}

void LIN_Check(void)
{
	if( LIN_CheckCRC() == OK )
		Messages_info[RecMsgIdx].Msg_Error_State = OK;
	else
		Messages_info[RecMsgIdx].Msg_Error_State = NOT_OK;
}

void LIN_ReceiveBreak(void)
{
	USART_setRxCbf(&LIN_SlaveTask);
	USART_Recieve(LIN_Header_Recieve,2);

	LIN_Header[0] = 0x55;                   				/* 2- send synch byte -> 0x55   */
	LIN_Header[1] = Messages_info[messegeIndex].Msg_PID;    /* 3- send PID for the message  */

	USART_Send(LIN_Header,2);
}

void LIN_SlaveTask(void)				/* Call back function to call after Receive Header */
{
	u8 msgIdx;
	u8 ReceiverIdx;
	if(LIN_Header_Recieve[0] == 0x55)
	{
		for(msgIdx=0; msgIdx<NUM_OF_MSGS; msgIdx++)
		{
			if(Messages_info[msgIdx].Msg_PID == LIN_Header_Recieve[1])
			{
				if(Messages_info[msgIdx].Sender == SLAVE_ID)
				{
					SendMsgIdx = msgIdx;
					slaveSend = 1;
				}
				else
				{
					for(ReceiverIdx=0; ReceiverIdx<Messages_info[msgIdx].Num_Of_Receivers; ReceiverIdx++)
					{
						if(Messages_info[msgIdx].Receivers[ReceiverIdx] == SLAVE_ID)
						{
							USART_setRxCbf(&LIN_Check);
							USART_Recieve(Messages_info[msgIdx].Msg_buffer,(Messages_info[msgIdx].Num_Of_Bytes+1));	/* +1 for CRC byte */
							RecMsgIdx = msgIdx;
						}
					}
				}
			}
		}
	}
}

void Slave_Task(void)
{
	if(slaveSend)
	{
		slaveSend = 0;
		LIN_CalcCRC();
		USART_Send(Messages_info[SendMsgIdx].Msg_buffer,(Messages_info[SendMsgIdx].Num_Of_Bytes+1));		/* +1 for CRC byte */
	}
}

void Master_Task(void)
{
	static u16 timeCounter;
	static u8 msgCounter;
	if(NODETYPE == MASTER)
	{
		if(timeCounter == 0)
		{
			/* its time to send its header */
			messegeIndex = msgCounter;			  /* messegeIndex-> Message index to send its PID */
			USART_setBreakCbf(&LIN_ReceiveBreak);
			USART1x->CR1 |= SBK_SEND_BREAK;       /* 1- send break          */
		}
		if(Messages_info[msgCounter].Ticks_to_Send == timeCounter)
		{
			msgCounter++;
			if(msgCounter == NUM_OF_MSGS)
			{
				msgCounter=0;
			}
			timeCounter = 0;
		}
		else
		{
			timeCounter ++;
		}
	}
}

const task_t masterTask = {&Master_Task, MASTER_TASK_PERIODICITY};
const task_t slaveTask = {&Slave_Task, 2};

/*
 * LIN.h
 *
 *  Created on: May 3, 2020
 *      Author: Norhan Nassar
 */

#ifndef LIN_H_
#define LIN_H_


typedef u8 Slave_ID_t;						/* each Slave ha its ID 							 */
typedef u8 Node_Type_t;						/* to configure each Node if it is a slave or Master */

#define MASTER					0
#define SLAVE					1

typedef struct
{
	u8 Msg_ID;
	u8 Num_Of_Bytes;											/* Number of bytes to send at each message 						 */
	u16 Time_Slot;												/* Time slot for each Message								     */
	Slave_ID_t Sender;											/* who is the sender for this Message 							 */
	Slave_ID_t Receivers[MAX_NUM_OF_RECEIVERS];					/* Array of receivers foe each Message 							 */
	u8 Num_Of_Receivers;										/* Number of Receivers for each Message 						 */
}Msg_cfg_t;

extern STD_ERROR LIN_Init(void);
STD_ERROR LIN_Send(volatile u8* msgBufferData, u8 bytesCount,u8 messageID);
STD_ERROR LIN_Recieve(volatile u8* msgBufferData, u8 bytesCount,u8 messageID);


#endif /* LIN_H_ */

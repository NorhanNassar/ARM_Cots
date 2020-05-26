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
	u8 No_of_bytes;							/* Number of bytes for each msg to calculate its time to send    */
	Slave_ID_t Sender;						/* who is the sender for this msg 								 */
	Slave_ID_t Receivers[MAX_NUM_OF_RECEIVERS];					/* Array of receivers foe each msg 								 */
}Msg_cfg_t;

extern STD_ERROR LIN_Init(void);


#endif /* LIN_H_ */

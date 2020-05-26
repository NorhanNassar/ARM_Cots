/*
 * LIN_cfg.c
 *
 *  Created on: May 3, 2020
 *      Author: Norhan Nassar
 */
#include "STD_TYPES.h"
#include "LIN_cfg.h"
#include "LIN.h"

extern Msg_cfg_t Messeges_cfg[NUM_OF_MSGS]=
{
		/* Msg_ID,		Num_of_bytes,			Sender_slave_id,		Array_of_Slaves_Recievers_IDs	*/
		{ 0x01, 		5,						SLAVE_ID_0,				{SLAVE_ID_1, SLAVE_ID_2}				},
		{ 0x02,			6,						SLAVE_ID_1,				{SLAVE_ID_2,SLAVE_ID_3,SLAVE_ID_4}		}
};

/* hwa momken yb2a #define bs ana 7abet kol el conf ll Node teb2a hena */
extern Slave_ID_t Slave_ID = SLAVE_ID_0;				/* certain slave id 		*/

/* nafs el kalam */
extern Node_Type_t	Node_Type = MASTER;					/* Node Type
 	 	 	 	 	 	 	 	 	 	 	 	 * options: MASTER, SLAVE	*/





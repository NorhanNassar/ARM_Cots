/*
 * LIN_cfg.c
 *
 *  Created on: May 3, 2020
 *      Author: Norhan Nassar
 */
#include "STD_TYPES.h"
#include "LIN_cfg.h"
#include "LIN.h"

volatile Msg_cfg_t Messages_cfg[NUM_OF_MSGS]=
{
		/* Msg_ID,		Number of bytes,		Time slot in msec,		Sender_slave_id,		Array_of_Slaves_Receivers_IDs,			Number of Receivers for this message 	*/
		{ 0x01, 		3,						100,						SLAVE_ID_0,				{SLAVE_ID_1, SLAVE_ID_2},		 		2											},
		{ 0x02,			4,						100,						SLAVE_ID_1,				{SLAVE_ID_2,SLAVE_ID_3,SLAVE_ID_4},		3											}
};


/* lazm a configure el uart 3ala 9600 baud rate 3shan el calculations ma3mola 3ala keda */
/* Number of bytes da men 8er el CRC da 3add el bytes bs */
/* bs el 7essabat ma3mola 3ala kol wa7ed zyada byte 3shan el CRC EL time slot ya3ny */
/* No of bytes byb2a men dmnhom el CRC 3shan byt7sebloh wa2t bardo 3shan die byte */


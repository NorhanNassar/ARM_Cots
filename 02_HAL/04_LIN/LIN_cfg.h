/*
 * LIN_cfg.h
 *
 *  Created on: May 3, 2020
 *      Author: Norhan Nassar
 */

#ifndef LIN_CFG_H_
#define LIN_CFG_H_

#define NUM_OF_MSGS					2				/* configure it to the number of messages in your Network 							*/
#define MAX_NUM_OF_RECEIVERS		3				/* maximum number of receivers for all messages				  						*/
#define MASTER_TASK_PERIODICITY		2				/* master task periodicity it is determined according to time slot for each message */
#define SLAVE_TASK_PERIODICITY		1
#define SLAVE_ID					SLAVE_ID_0
#define NODETYPE					MASTER			/* Node type: MASTER or SLAVE 														*/

/* add Slave IDs if your Network has more 	*/
#define SLAVE_ID_0					0x01
#define SLAVE_ID_1					0x02
#define SLAVE_ID_2					0x03
#define SLAVE_ID_3					0x04
#define SLAVE_ID_4					0x05

#endif /* LIN_CFG_H_ */

/*
 * HSwitch.h
 *
 *  Created on: Mar 26, 2020
 *      Author: Norhan Nassar
 */

#ifndef HSWITCH_H_
#define HSWITCH_H_

typedef 	u8 	SWITCH_STATE;
#define 	FLOATING				MODE_IP_FLOATING
#define		PULL_UP					MODE_IP_PUPD
#define		PULL_DOWN				0

#define 	SWITCH_PRESSED			1
#define		SWITCH_RELEASED			0


/* Structure to each switch 																*/
/* It contains: Switch Number, Switch Pin, Switch Port
 * and Switch state -> PULL_UP, PULL_DOWN, FLOATING											*/
typedef struct
{
	u32 Switch_Port;								/* PORTA to PORTG					    */
	u16 Switch_Pin;									/* GPIO_PIN0 to GPIO_PIN15 			    */
	u16 Switch_Num;									/* Switch_0 to Switch_15 or more
													 * if you add some in cfg.h file		*/
	u8 SWITCH_STATE;								/* PULL_UP, PULL_DOWN or FLAOTING	    */
}Switch_cfg_t;

/*------------------------------------------------------------------------------------------*/
/* Description: This API shall initialize all Switches at configuration file to be Input    */
/* Pull up, pull down or floating for each port 										    */
/*------------------------------------------------------------------------------------------*/
extern STD_ERROR HSwitch_init(void);

/*------------------------------------------------------------------------------------------*/
/* Description: This API shall check if a certain switch is pressed or not				    */
/* Copy_PtrState -> pointer to switch state												    */
/*------------------------------------------------------------------------------------------*/
extern STD_ERROR HSwitch_getSwitchState(u8 Copy_u8SwitchNum,u8* Copy_PtrSwitchState);


#endif /* HSWITCH_H_ */

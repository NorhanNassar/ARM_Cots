/*
 * HSwitch.c
 *
 *  Created on: Mar 26, 2020
 *      Author: Norhan Nassar
 */

#include "STD_TYPES.h"
#include "Sched.h"
#include "GPIO_interface.h"
#include "HSwitch_cfg.h"
#include "HSwitch.h"

/*------------------------------------------------------------------------------------------*/
/* Task Runnable every given periodicity												    */
/* to check if Switches are pressed or not 												    */
/*------------------------------------------------------------------------------------------*/
static void HSwitch_Runnable(void);


extern Switch_cfg_t Switches[SWITCHES_NUMBER];
static u8 switchState[SWITCHES_NUMBER];					/* it contains each switch state 	*/

STD_ERROR HSwitch_init()
{
	STD_ERROR loc_err = OK;
	u8 i;
	GPIO_Conf_Pin_t switchMap;

	for(i=0;i<SWITCHES_NUMBER;i++)
	{
		switchMap.Port = Switches[i].Switch_Port;
		switchMap.Pin = Switches[i].Switch_Pin;
		if(Switches[i].SWITCH_STATE == PULL_UP)
		{
			switchMap.Mode_Configure = Switches[i].SWITCH_STATE;
			switchMap.Mode_Speed = IP_NOSPEED;
			GPIO_voidConfigure(&switchMap);
			GPIO_u8Write(switchMap.Port,switchMap.Pin,SET);
		}
		else if(Switches[i].SWITCH_STATE == PULL_DOWN)
		{
			switchMap.Mode_Configure = MODE_IP_PUPD;
			switchMap.Mode_Speed = IP_NOSPEED;
			GPIO_voidConfigure(&switchMap);
			GPIO_u8Write(switchMap.Port,switchMap.Pin,RESET);
		}
		else
		{
			switchMap.Mode_Configure = Switches[i].SWITCH_STATE;
			switchMap.Mode_Speed = IP_NOSPEED;
			GPIO_voidConfigure(&switchMap);
		}
	}
	return loc_err;
}

void HSwitch_Runnable(void)
{
	static u8 currentState;
	static u8 prevState[SWITCHES_NUMBER]={0};
	static u8 counter[SWITCHES_NUMBER]={0};
	u8 i;
	u8 Data;

	for(i=0;i<SWITCHES_NUMBER;i++)
	{
		GPIO_u8Read(Switches[i].Switch_Port,Switches[i].Switch_Pin,&currentState);
		if(currentState==prevState[i])
			counter[i]++;
		else
			counter[i]=0;

		if(counter[i] == 5)
		{
			counter[i] =0;
			switchState[i] = currentState;

		}
		prevState[i]=currentState;
	}
}

STD_ERROR HSwitch_getSwitchState(u8 Copy_u8SwitchNum,u8* Copy_PtrSwitchState)
{
	if(Switches[Copy_u8SwitchNum].SWITCH_STATE == PULL_UP)
		*Copy_PtrSwitchState = !switchState[Copy_u8SwitchNum];			/* if it is pull_up so it will return
																		 * 0 if pin is read as 1 and 1 if it is read as 0 */
	else
		*Copy_PtrSwitchState = switchState[Copy_u8SwitchNum];

	return OK;
}


const task_t HswitchTask = {HSwitch_Runnable,5};			/* HSwitch task runnable and periodicity */










/*
 * HSwitch_cfg.c
 *
 *  Created on: Mar 26, 2020
 *      Author: Norhan Nassar
 */

#include "STD_TYPES.h"
#include "GPIO_interface.h"
#include "HSwitch.h"
#include "HSwitch_cfg.h"

Switch_cfg_t Switches[SWITCHES_NUMBER]=
{
		{PORTA,GPIO_PIN0,SWITCH_0,PULL_UP},
		{PORTA,GPIO_PIN1,SWITCH_1,PULL_UP},
		{PORTA,GPIO_PIN2,SWITCH_2,PULL_UP},
		{PORTA,GPIO_PIN3,SWITCH_3,PULL_UP}
};
















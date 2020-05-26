/*
 * OS_cfg.c
 *
 *  Created on: Apr 7, 2020
 *      Author: Norhan Nassar
 */
#include "STD_TYPES.h"
#include "Sched_cfg.h"
#include "Sched.h"


extern const task_t HswitchTask;
extern const task_t Task;

const taskInfo_t tasks[MAXTASKNUM]=					/* tasks Order according to its priority	*/
{
		/* 	task Runnable,   firstDelayTicks	*/
		{	&HswitchTask,   	 0				},
		{	&Task,				 0				}
};





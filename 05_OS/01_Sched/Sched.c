/*
 * OS.c
 *
 *  Created on: Mar 24, 2020
 *      Author: Norhan Nassar
 */

#include "STD_TYPES.h"
#include "SYSTICK_interface.h"
#include "Sched_cfg.h"
#include "Sched.h"

typedef u8 taskState_t;
#define NULL ((void *)0)

#define SUSPENDED			0
#define READY				1
#define SLEEP				2
#define IDEAL				3

typedef struct
{
	taskInfo_t AppTask;
	u8 ticksToExecute;
	u8 countIfPause;
	taskState_t taskState;
} taskMoreInfo_t ;


extern const taskInfo_t tasks[MAXTASKNUM];

static u8 schedStartFlag = 0;
static taskMoreInfo_t Systasks[MAXTASKNUM];

/*---------------------------------------------------------------------------------------------------------*/
/* Call back function for systick handler, to increment the flag that a tick has arrived  				   */
/*---------------------------------------------------------------------------------------------------------*/
static void Sched_SystickCbf(void);

/*--------------------------------------Schedule Creat tasks-----------------------------------------------*/
static STD_ERROR Sched_CreatTasks();

/*-----------------------------------------Schedule Runnable-----------------------------------------------*/
static void Sched_Runnable(void);

/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------*/
/* Call back function for systick handler, to increment the flag that a tick has arrived */
/*---------------------------------------------------------------------------------------*/
void Sched_SystickCbf(void)
{
	schedStartFlag++;	 /* to handle a lot of one if an interrupt cut one while running */
}

/*---------------------------------------------------------------------------------------*/
STD_ERROR Sched_init(void)	/* at cfg file, Tick time must be configured in milli sec	 */
{
	if(SYSTICK_Init())
		return NOT_OK;
	if(SYSTICK_SetTime(TICKTIMEMS*1000))	/* TICKTIMEMS*1000 to convert it to micro 	 */
		return NOT_OK;

	SYSTICK_SetCallBack(&Sched_SystickCbf);

	return (Sched_CreatTasks());
}

/*-------------------------------------------------------------------------------------------------*/
STD_ERROR Sched_Start(void)
{
	if(SYSTICK_Start())					/* Start Systick to start count */
		return NOT_OK;					/* if systick returns NOT_OK =1 so Sched_start returns NOT_OK */
	while(1)
	{
		while(!schedStartFlag);			/* when tick is arrived schedStartFlag will not be zero */
		if(schedStartFlag>0)
			schedStartFlag--;			/* here is a runnable for one flag is going to be executed */
		Sched_Runnable();				/* then Sched will be executed */
	}
	return OK;
}

/*-------------------------------------------------------------------------------------------------*/
/* task struct has:
 * task Periodicity -> it must be in micro sec
 * firstDelayTicks -> first delay then task start to count
 * Runnable -> task function to execute
 * taskPriority -> it will return error if it exceeds the array of tasks limit
 * and return error -> if TICKTIMEUS in cfg file is less than or equal 0 */
/*-------------------------------------------------------------------------------------------------*/
STD_ERROR Sched_CreatTasks()
{
	u8 i;
	if(TICKTIMEMS<=0)
		return NOT_OK;
	for(i=0;i<MAXTASKNUM;i++)
	{
		Systasks[i].AppTask = tasks[i];
		Systasks[i].ticksToExecute = tasks[i].task->Periodicity/TICKTIMEMS;
		Systasks[i].taskState = READY;
	}
	return OK;
}

/*------------------------------------------------------------------------------------*/
/* taskRunnable -> address of the task call back function that you want to suspend it */
/*------------------------------------------------------------------------------------*/
STD_ERROR Sched_TaskSuspend(taskRunnable_t taskRunnable)
{
	u8 i;
	for(i=0;i<MAXTASKNUM;i++)
	{
		if(Systasks[i].AppTask.task->Runnable == taskRunnable)
		{
			Systasks[i].taskState = SUSPENDED;
			return OK;
		}
	}
	return NOT_OK;
}


/*-----------------------------------------------------------------------------------*/
/* Period -> must be in milli sec
 * and it must be larger than task periodicity time
 * taskRunnable -> address of the task call back function that you want to pause it   */
/*------------------------------------------------------------------------------------*/
STD_ERROR Sched_TaskSleep(taskRunnable_t taskRunnable, u32 Period)
{
	STD_ERROR loc_err = NOT_OK;
									/* task must be ready first to make it suspended   */
									/* and Period must be larger than task Periodicity */
	u8 i;
	for(i=0;i<MAXTASKNUM;i++)
	{
		if((Systasks[i].AppTask.task->Runnable == taskRunnable)
				&&(Systasks[i].taskState == READY)&&(Period>Systasks[i].AppTask.task->Periodicity))
		{
			Systasks[i].taskState = SLEEP;
			Systasks[i].countIfPause = Period / Systasks[i].AppTask.task->Periodicity;
			loc_err = OK;
		}
	}

	return loc_err;
}

/*-------------------------------------------------------------------------------*/
void Sched_Runnable(void)
{
	u8 i;
	for(i=0;i<MAXTASKNUM;i++)
	{
		if(Systasks[i].ticksToExecute == 0)
		{
			switch(Systasks[i].taskState)
			{
			case READY:
				if(Systasks[i].AppTask.task->Runnable == NULL)
					break;
				if(Systasks[i].AppTask.firstDelayTicks==0)
					Systasks[i].AppTask.task->Runnable();
				break;
			case SLEEP:
				Systasks[i].countIfPause--;
				if(Systasks[i].countIfPause == 0)
					Systasks[i].taskState = READY;
				break;
			}

			Systasks[i].ticksToExecute = (Systasks[i].AppTask.task->Periodicity / TICKTIMEMS) - 1 ;
		}
		else
			Systasks[i].ticksToExecute--;
	}
}





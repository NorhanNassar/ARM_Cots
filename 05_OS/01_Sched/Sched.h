/*
 * Sched2.h
 *
 *  Created on: Mar 24, 2020
 *      Author: Norhan Nassar
 */

#ifndef SCHED_H_
#define SCHED_H_


typedef void (*taskRunnable_t)(void);

typedef struct							/* struct for user, for each task module */
{
	taskRunnable_t Runnable;			/* Pointer to Task Function 			*/
	u32 Periodicity;					/* time in milli sec 					*/
} task_t;

typedef struct							/* struct for Architect to set Periority and first delay for each task */
{
	const task_t* task;
	u8 firstDelayTicks;					/* first delay in Ticks 	*/
} taskInfo_t;


/*----------------------------------------------------------------------------------------*/
/* at cfg file, Tick time must be configured in milli sec								  */
/*----------------------------------------------------------------------------------------*/
extern STD_ERROR Sched_init(void);

/*----------------------------------------------------------------------------------------*/
extern STD_ERROR Sched_Start(void);

/*----------------------------------------------------------------------------------------*/
/* taskRunnable -> address of the task call back function that you want to suspend it     */
/*----------------------------------------------------------------------------------------*/
extern STD_ERROR Sched_TaskSuspend(taskRunnable_t tasktosus);

/*----------------------------------------------------------------------------------------*/
/* Period -> must be in milli sec
 * and it must be larger than task periodicity time
 * taskRunnable -> address of the task call back function that you want to pause it       */
/*----------------------------------------------------------------------------------------*/
extern STD_ERROR Sched_TaskPause(taskRunnable_t taskRunnable, u32 Period);


#endif /* SCHED2_H_ */

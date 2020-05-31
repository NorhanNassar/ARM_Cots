#include "STD_TYPES.h"

#include "NVIC.h"


/* To get Active flag for system exceptions */
static STD_ERROR SCB_GetActiveFlag(u32 Copy_u32InterruptNum,u8* Copy_PtrActiveStatus)
{
	switch(Copy_u32InterruptNum)
	{
	case MEMMANAGE:
		*Copy_PtrActiveStatus = (SCBx->SHCSR) & 1;
		break;
	case BUSFAULT:
		*Copy_PtrActiveStatus = ((SCBx->SHCSR)>>1) & 1;
		break;
	case USAGEFAULT:
		*Copy_PtrActiveStatus = ((SCBx->SHCSR)>>3) & 1;
		break;
	case SVCALL:
		*Copy_PtrActiveStatus = ((SCBx->SHCSR)>>7) & 1;
		break;
	case DEBUGMONITOR:
		*Copy_PtrActiveStatus = ((SCBx->SHCSR)>>8) & 1;
		break;
	case PENDSV:
		*Copy_PtrActiveStatus = ((SCBx->SHCSR)>>10) & 1;
		break;
	case SYSTICK:
		*Copy_PtrActiveStatus = ((SCBx->SHCSR)>>11) & 1;
		break;
	}
	return OK;

}

/* To get Pending flag for system exceptions at ICSR Register */
static STD_ERROR SCB_ICSR_GetPendingFlag(u32 Copy_u32InterruptNum,u8* Copy_PtrPendingStatus)
{
	/* If this Interrupt is NMI or PENDSV or SYSTICK it will be set at SCB->ICSR Register not ISPR */
	switch(Copy_u32InterruptNum)
	{
	case SYSTICK:
		*Copy_PtrPendingStatus = (SCBx->ICSR >> 26) &1;
		break;
	case NMI:
		*Copy_PtrPendingStatus = (SCBx->ICSR >> 31) &1;
		break;
	case PENDSV:
		*Copy_PtrPendingStatus = (SCBx->ICSR >> 28) &1;
		break;
	}
	return OK;
}

/* To get Pending flag for system exceptions at SHCSR Register */
static STD_ERROR SCB_SHCSR_GetPendingFlag(u32 Copy_u32InterruptNum,u8* Copy_PtrPendingStatus)
{
	/* If this interrupt is USAGEFAULT, BUSFAULT, MEMMANAGE or SVCALL */
	switch(Copy_u32InterruptNum)
	{
	case SVCALL:
		*Copy_PtrPendingStatus = (SCBx->ICSR >> 15) &1;
		break;
	case MEMMANAGE:
		*Copy_PtrPendingStatus = (SCBx->ICSR >> 13) &1;
		break;
	case BUSFAULT:
		*Copy_PtrPendingStatus = (SCBx->ICSR >> 14) &1;
		break;
	case USAGEFAULT:
		*Copy_PtrPendingStatus = (SCBx->ICSR >> 12) &1;
		break;
	}
	return OK;

}
/*******************************************************************/

STD_ERROR NVIC_EnableInterrupt(u32 Copy_u32InterruptNum)
{
	STD_ERROR loc_err = NOT_OK;
	if(CHECKENABLESYSEXC(Copy_u32InterruptNum))
	{
		/* if this interrupt is MEMMANAGE or USAGEFAULT or BUSFAULT */

		/* Clear Pending by software before enable this interrupt */
		NVIC_ClearPending(Copy_u32InterruptNum);

		SCBx->SHCSR |= Copy_u32InterruptNum;
		loc_err = OK;
	}
	else if(Copy_u32InterruptNum<=MAXINTERRUPTNUM)
	{
		/* Clear Pending by software before enable this interrupt */
		NVIC_ClearPending(Copy_u32InterruptNum);

		NVICx->ISER[Copy_u32InterruptNum/32] |= 1<< ( Copy_u32InterruptNum % 32 );
		loc_err = OK;
	}
	return loc_err;
}

STD_ERROR NVIC_DisableInterrupt(u32 Copy_u32InterruptNum)
{
	STD_ERROR loc_err = NOT_OK;
	if(CHECKENABLESYSEXC(Copy_u32InterruptNum))
	{
		/* if this interrupt is MEMMANAGE or USAGEFAULT or BUSFAULT */
		SCBx->SHCSR &= (~Copy_u32InterruptNum);
		loc_err = OK;
	}
	else if(Copy_u32InterruptNum<=MAXINTERRUPTNUM)
	{
		NVICx->ICER[Copy_u32InterruptNum/32] |= 1<< ( Copy_u32InterruptNum % 32 );
		loc_err = OK;
	}
	return loc_err;
}


STD_ERROR NVIC_SetPending(u32 Copy_u32InterruptNum)
{
	STD_ERROR loc_err = NOT_OK;
	if(CHECKPENDATSCBICSR(Copy_u32InterruptNum))
	{
		/* If this interrupt is PENDSV or SYSTICK return error because writing 1 at these bits make unpredictable effects */
		if((Copy_u32InterruptNum == PENDSV) || (Copy_u32InterruptNum == SYSTICK))
			return NOT_OK;

		/* If this Interrupt is NMI it will be set at SCB->ICSR Register not ISPR */
		SCBx->ICSR |= Copy_u32InterruptNum;
		loc_err = OK;
	}
	else if(CHECKPENDATSCBSHCSR(Copy_u32InterruptNum))
	{
		if(Copy_u32InterruptNum == SVCALL)
		{
			SCBx->SHCSR |= SVCALL;
		}
		else if(Copy_u32InterruptNum == USAGEFAULT)
		{
			/* To mask USAGEFAULT pending bit, it is far 6 bits away to its enable bit */
			SCBx->SHCSR |= (USAGEFAULT>>6);
		}
		else
		{
			/* To mask MEMMANAGE or BUSFAULT pending bit, they are far 3 bits away to there enable bits */
			SCBx->SHCSR |= (Copy_u32InterruptNum>>3);
		}

		loc_err = OK;
	}
	else if(Copy_u32InterruptNum <= MAXINTERRUPTNUM)
	{
		/* 32 is the register size */
		NVICx->ISPR[Copy_u32InterruptNum/32] |= 1<< (Copy_u32InterruptNum % 32 );
		loc_err = OK;
	}

	return loc_err;

}

STD_ERROR NVIC_GetPendingFlag(u32 Copy_u32InterruptNum,u8* Copy_PtrPendingStatus)
{
	STD_ERROR loc_err = NOT_OK;
	if(CHECKPENDATSCBICSR(Copy_u32InterruptNum))
	{
		/* If this Interrupt is NMI or PENDSV or SYSTICK it will be set at SCB->ICSR Register not ISPR */
		loc_err = SCB_ICSR_GetPendingFlag(Copy_u32InterruptNum,Copy_PtrPendingStatus);
	}
	else if(CHECKPENDATSCBSHCSR(Copy_u32InterruptNum))
	{
		/* If this interrupt is UAGEFAULT, BUSFAULT, MEMMANAGE or SVCALL */
		loc_err = SCB_SHCSR_GetPendingFlag(Copy_u32InterruptNum,Copy_PtrPendingStatus);
	}
	else if(Copy_u32InterruptNum <= MAXINTERRUPTNUM)
	{
		/* 32 is the register size */
		*Copy_PtrPendingStatus=(NVICx->ISPR[Copy_u32InterruptNum/32]>>(Copy_u32InterruptNum % 32 ))&1;
		loc_err = OK;
	}

	return loc_err;

}

STD_ERROR NVIC_ClearPending(u32 Copy_u32InterruptNum)
{
	STD_ERROR loc_err = NOT_OK;
	if(CHECKPENDATSCBICSR(Copy_u32InterruptNum))
	{
		/* If this Interrupt is PENDSV or SYSTICK it will be cleared at SCB Register not ICPR */

		/* If this Interrupt is NMI it hasn't bit for clear pending by software */
		if(Copy_u32InterruptNum == NMI)
			return loc_err;
		else
		{
			SCBx->ICSR |= Copy_u32InterruptNum>>1;
			loc_err = OK;
		}
	}
	else if(CHECKPENDATSCBSHCSR(Copy_u32InterruptNum))
	{
		if(Copy_u32InterruptNum == SVCALL)
		{
			SCBx->SHCSR &= (~SVCALL);
		}
		else if(Copy_u32InterruptNum == USAGEFAULT)
		{
			/* To mask USAGEFAULT pending bit, it is far 6 bits away to its enable bit */
			SCBx->SHCSR &= (~(USAGEFAULT>>6));
		}
		else
		{
			/* To mask MEMMANAGE or BUSFAULT pending bit, they are far 3 bits away to there enable bits */
			SCBx->SHCSR &= (~(Copy_u32InterruptNum>>3));
		}

		loc_err = OK;
	}
	else if(Copy_u32InterruptNum <= MAXINTERRUPTNUM)
	{
		NVICx->ICPR[Copy_u32InterruptNum/32] |= 1<< (Copy_u32InterruptNum % 32 );
		loc_err = OK;
	}

	return loc_err;

}

STD_ERROR NVIC_GetActiveFlag(u32 Copy_u32InterruptNum,u8* Copy_PtrActiveStatus)
{
	STD_ERROR loc_err = NOT_OK;
	if( ( CHECKACTIVEATSCBSHCSR(Copy_u32InterruptNum) ) && (Copy_PtrActiveStatus) )
	{
		loc_err = SCB_GetActiveFlag(Copy_u32InterruptNum,Copy_PtrActiveStatus);
	}
	else if( (Copy_u32InterruptNum<=MAXINTERRUPTNUM) && (Copy_PtrActiveStatus) )
	{
		*Copy_PtrActiveStatus = (NVICx->IABR[Copy_u32InterruptNum/32]>>(Copy_u32InterruptNum%32))&1;
		loc_err = OK;
	}
	return loc_err;
}

STD_ERROR NVIC_SetPriorityGrouping(u8 Copy_u8PriorityGroup)
{
	STD_ERROR loc_err = NOT_OK;

	/* Priority group is only 3 bits, so you must check that it isn't exceed its limit -> 7 */
	if(Copy_u8PriorityGroup<=7)
	{
		/* The number needed for shift to be in correct region to set the priority group is 7
		 *  and it equals to the maximum number for priority group that it could be */
		SCBx->AIRCR = SCB_AIRCR_VECTKEY_MASK | (Copy_u8PriorityGroup<<8);
		loc_err = OK;
	}

	return loc_err;
}

STD_ERROR SCB_GetPriorityGroup(u8* Copy_PtrPriGroup)
{
	*Copy_PtrPriGroup = (SCBx->AIRCR>>8)&(0b111);
	return OK;
}

STD_ERROR NVIC_SetPriority(u32 Copy_u32InterruptNum,u8 Copy_u8SubPriority, u8 Copy_u8Preemp)
{
	u8 PriGroup;
	/* number of bits for priority is changed for each chip
	 * for STM it equals to 4 bits */
	u8 PriBits = 4;
	/* for STM it is 4 bit Priority so maximum 16 programmable priority levels */
	if( (Copy_u32InterruptNum>=MAXINTERRUPTNUM) || ( (Copy_u8SubPriority+Copy_u8Preemp)>16) )
	{
		return NOT_OK;
	}
	else
	{
		SCB_GetPriorityGroup(&PriGroup);
		if(PriGroup < PriBits)
		{
			/* If the priority group is not large enough to number of bits for priority so all priority bits
			 * will be for preemption and subgroup must be 0 because it will not handle subgroups */
			if(Copy_u8SubPriority!=0)
				return NOT_OK;
			/* Shift it according to how many priority bits each chip has so 8(for the byte)-number of bits
			 * for STM it is 4 bit Priority -> 8-4*/
			NVICx->IPR[Copy_u32InterruptNum] = Copy_u8Preemp<<(8-PriBits);
		}
		else
		{
			/* Shift it according to how many priority bits each chip has so 8(for the byte)-number of bits
			 * for STM it is 4 bit Priority -> 8-4*/
			NVICx->IPR[Copy_u32InterruptNum] = Copy_u8SubPriority<<(8-PriBits);
			/* Shift to set preemption according to how many bits each chip has for priority and
			 * how many bits for priority group you set */
			NVICx->IPR[Copy_u32InterruptNum] |= Copy_u8Preemp<<(8-PriBits+(PriGroup-(PriBits-1)));
		}

		return OK;
	}

}


STD_ERROR NVIC_DisableGlobalInterrupt()
{
	STD_ERROR loc_err = OK;
	asm("MOV R0, #1");
	asm("MSR PRIMASK, R0");
	return loc_err;
}

STD_ERROR NVIC_EnableGlobalInterrupt()
{
	STD_ERROR loc_err = OK;
	asm("MOV R0, #0");
	asm("MSR PRIMASK, R0");
	return loc_err;
}

STD_ERROR NVIC_DisGloInterrAndHardFault()
{
	STD_ERROR loc_err = OK;
	asm("MOV R0, #1");
	asm("MSR FAULTMASK, R0");
	return loc_err;
}

STD_ERROR NNVIC_EnaGloInterrAndHardFault()
{
	STD_ERROR loc_err = OK;
	asm("MOV R0, #0");
	asm("MSR FAULTMASK, R0");
	return loc_err;
}

STD_ERROR NVIC_DisInterrsWithPriority(u8 Copy_u8InterruptNum)
{
	STD_ERROR loc_err;
	loc_err = NOT_OK;
	if (Copy_u8InterruptNum <= MAXINTERRUPTNUM)
	{
		/* The BASEPRI/ BASEPRI_MAX register cannot be set in the user state */
		asm("MOV R0, %0" : : "r" (Copy_u8InterruptNum) : "memory");
		asm("MSR BASEPRI, R0");
		loc_err = OK;
	}

	return loc_err;
}

STD_ERROR NVIC_EnaInterrsWithPriority()
{
	STD_ERROR loc_err;
	loc_err = OK;
	asm("MOV R0, #0");
	asm("MSR BASEPRI, R0");
	return loc_err;
}

STD_ERROR SCB_GetPendingInterruptNum(u8* Copy_PtrInterruptNum)
{
	STD_ERROR loc_err = OK;
	/* This pending bit at bit offset 12 */

	*Copy_PtrInterruptNum= (SCBx->ICSR&GETPENDING)>>12;
	if(! *Copy_PtrInterruptNum)
	{
		/* No pending exceptions */
		loc_err = NOT_OK;
	}

	return(loc_err);
}

STD_ERROR SCB_GetActiveInterruptNum(u8* Copy_PtrInterruptNum)
{
	if(!(SCBx->ICSR&GETACTIVE))
	{
		/* No Active exceptions */
		return(NOT_OK);
	}
	else
	{
		*Copy_PtrInterruptNum = (SCBx->ICSR&GETACTIVE);
		return(OK);
	}
}

STD_ERROR SCB_SetVectorTableBaseOffset(u32 Copy_u32BaseOffset,u32 Copy_u8MemoryRegion)
{
	STD_ERROR loc_err = OK;
	if(Copy_u8MemoryRegion == Code)
	{
		SCBx->VTOR |= Code;
		/* VTOROFFSETBIT is located after bit number 8 */
		SCBx->VTOR |= (Copy_u32BaseOffset<<8);
	}
	else if(Copy_u8MemoryRegion == SRAM)
	{
		SCBx->VTOR &= SRAM;
		SCBx->VTOR |= (Copy_u32BaseOffset<<8);
	}
	else
		loc_err= NOT_OK;

	return loc_err;
}

STD_ERROR SCB_GetVectorTableBaseOffset(u8* Copy_PtrMemoryRegion)
{
	/* Bit 29 determines whether the vector table is in the code or SRAM memory region */
	*Copy_PtrMemoryRegion = (SCBx->VTOR>>29) & 1;

	return OK;
}

STD_ERROR SCB_EnableTrapDivByZero()
{
	STD_ERROR loc_err = OK;

	SCBx->CCR |= CCR_DIV_BY_ZERO_TRAP;

	return loc_err;
}

STD_ERROR SCB_DisableTrapDivByZero()
{
	STD_ERROR loc_err = OK;

	SCBx->CCR &= (~CCR_DIV_BY_ZERO_TRAP);

	return loc_err;
}

STD_ERROR NVIC_SoftwareInterrupt(u32 Copy_u8InterruptNum)
{
	STD_ERROR loc_err = NOT_OK;
	if(Copy_u8InterruptNum<=MAXINTERRUPTNUM)
	{
		SCBx->CCR |= CCR_USERSETMPEND;
		NVICx->STIR = Copy_u8InterruptNum;

		if(NVICx->STIR == Copy_u8InterruptNum)
			loc_err = OK;
	}
	return loc_err;
}

STD_ERROR SCB_SoftwareReset(void)
{
	STD_ERROR loc_err = NOT_OK;

	/* Priority group is only 3 bits, so you must check that it isn't exceed its limit -> 7 */

	/* The number needed for shift to be in correct region to set the priority group is 7
	 *  and it equals to the maximum number for priority group that it could be */
	SCBx->AIRCR = SCB_AIRCR_VECTKEY_MASK | SYS_RESET;
	loc_err = OK;

	return loc_err;
}



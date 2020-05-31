#define WWDG 			0
#define PVD				1
#define TAMPER			2
#define RTC				3
#define	FLASH			4
#define RCC				5
#define EXTI0			6
#define EXTI1			7
#define EXTI2			8
#define EXTI3			9
#define EXTI4			10
#define DMA1_CHANNEL1	11
#define DMA1_CHANNEL2	12
#define DMA1_CHANNEL3	13
#define DMA1_CHANNEL4	14
#define DMA1_CHANNEL5	15
#define DMA1_CHANNEL6	16
#define DMA1_CHANNEL7	17
#define ADC1_2 			18
#define CAN1_TX			19
#define CAN1_RX0		20
#define CAN1_RX1		21
#define CAN1_SCE		22
#define EXTI9_5			23
#define TIM1_BRK		24
#define TIM1_UP			25
#define TIM1_TRG_COM	26
#define TIM1_CC			27
#define TIM2			28
#define TIM3			29
#define TIM4			30
#define I2C1_EV			31
#define I2C1_ER			32
#define I2C2_EV			33
#define I2C2_ER			34
#define SPI1			35
#define SPI2			36
#define USART1			37
#define USART2			38
#define USART3			39
#define EXTI5_10		40
#define RTCALARM		41
#define OTG_FS_WKUP		42
#define TIM5			50
#define SPI3			51
#define UART4			52
#define UART5			53
#define TIM6			54
#define TIM7			55
#define DMA2_CHANNEL1	56
#define DMA2_CHANNEL2	57
#define DMA2_CHANNEL3	58
#define DMA2_CHANNEL4	59
#define DMA2_CHANNEL5	60
#define ETH				61
#define ETH_WKUP		62
#define CAN2_TX			63
#define CAN2_RX0		64
#define CAN2_RX1		65
#define CAN2_SCE		66
#define OTG_FS			67
/*********************************************************/
/* Base address for NVIC Registers */
#define NVIC_BASE_ADDRESS					0xE000E100

/* Base address for SCB Registers */
#define SCB_BASE_ADDRESS					0xE000ED00

/* Number of external Interrupts, and it changes for each chip for M3
 * Maximum number at M3 is 240 interrupt
 * at STM, it equals 67 interrupt 													      */
#define MAXINTERRUPTNUM						67

/*********************************************************/
#define NMI								0x80000000
#define PENDSV							0x10000000
#define SYSTICK							0x04000000
#define MEMMANAGE						0x00010000
#define BUSFAULT						0x00020000
#define USAGEFAULT						0x00040000
#define SVCALL							0x00008000
#define DEBUGMONITOR					0x00000100

#define CHECKENABLESYSEXC(INTERRUPTNUM) ( (INTERRUPTNUM == MEMMANAGE) || (INTERRUPTNUM == BUSFAULT) || (INTERRUPTNUM == USAGEFAULT) )
#define CHECKPENDATSCBICSR(INTERRUPTNUM) 	( (INTERRUPTNUM == NMI ) || (INTERRUPTNUM == PENDSV ) || (INTERRUPTNUM == SYSTICK) )
#define CHECKPENDATSCBSHCSR(INTERRUPTNUM) 	( (INTERRUPTNUM == SVCALL ) || (INTERRUPTNUM == USAGEFAULT ) || (INTERRUPTNUM == BUSFAULT) || (INTERRUPTNUM == MEMMANAGE) )
#define CHECKACTIVEATSCBSHCSR(INTERRUPTNUM) 	( (CHECKPENDATSCBSHCSR(INTERRUPTNUM))||(INTERRUPTNUM==DEBUGMONITOR)||(INTERRUPTNUM==SYSTICK)||(INTERRUPTNUM==PENDSV))
/**************************************************/
/* To change base address of vector table and it will be at Code or SRAM */
#define Code							0x20000000
#define SRAM							0x1FFFFFFF
/*************************************************/
/* Mask to get the external interrupt number pending at time */
#define GETPENDING						0x2FF000
/* Mask to get the external interrupt number active at time */
#define GETACTIVE						0x1FF
/**************************************************/
/* You must write this Register Key at SCB_AIRCR to be able to write at it
 * the Interrupt priority grouping field */
#define SCB_AIRCR_VECTKEY_MASK			0x05FA0000
#define SYS_RESET						0x04
/**************************************************/
/* Mask to enable DIV by zero interrupt */
#define CCR_DIV_BY_ZERO_TRAP			0x00000010
/* Mask must be set at SCB_SCR to enable the unprivileged software access to the STIR */
#define CCR_USERSETMPEND				0x00000002

/******************************************************************************************/
typedef struct NVIC_{
	/* Interrupt set-enable registers 													  */
	/* Number of Registers according to number of interrupts supported for each chip
	 * Maximum number of registers at M3 for 240 interrupts is 7
	 * for STM maximum number of interrupts is 84 so number of registers is 3															  */
	u32 ISER[3];
	/* Array for Reserved block to fill the gap between ISER Registers and ICER Registers
	 * for STM this gap equal to 116 decimal 											  */
	u8 Reserved_ISER[116];

	/* Interrupt clear-enable registers 												  */
	u32 ICER[3];
	/* Array for Reserved block to fill the gap between ICER Registers and ISPR Registers */
	u8 Reserved_ICER[116];

	/* Interrupt set-pending registers 													  */
	u32 ISPR[3];
	/* Array for Reserved block to fill the gap between ISPR Registers and ICPR Registers */
	u8 Reserved_ISPR[116];

	/* Interrupt clear-pending registers 												  */
	u32 ICPR[3];
	/* Array for Reserved block to fill the gap between ICPR Registers and IABR Registers */
	u8 Reserved_ICPR[116];

	/* Interrupt active bit registers 													  */
	u32 IABR[3];
	/* Array for reserved location for the gap between last register at NVIC before Priority
	 * Registers(IABR) and base address for these priority registers(IPR)
	 * at STM it equals to 244 decimal													 */
	u8 Reserved_IABR[244];

	/* Interrupt priority registers
	 * It is byte accessible, byte for each interrupt so it is u8 	    				 */
	u8 IPR[MAXINTERRUPTNUM];
	/* Array for reserved location between last priority register IPR and STIR Register
	 * at STM it equals to 2780 decimal													 */
	u8 Reserved_IPR[2780];

	/* Software trigger interrupt register */
	u32 STIR;

} NVIC_t;

#define NVICx	((NVIC_t volatile * const)NVIC_BASE_ADDRESS)

typedef struct{
	/* The CPUID register contains the processor part number, version, and implementation information */
	u32 CPUID;
	/* Interrupt control and state register at System Control Block Registers
	 * It is used to:
	 * - Set-pending bit for the Non-Maskable Interrupt (NMI) exception
	 * - Set-pending and clear-pending bits for the PendSV and SysTick exceptions
	 * - The exception number of the exception being processed
	 * - Whether there are preempted active exceptions
	 * - The exception number of the highest priority pending exception
	 * - Whether any interrupts are pending.														*/
	u32 ICSR;
	/* To change the vector table offset register address, whether it will be
	 * at Code or SRAM memory Region																*/
	u32 VTOR;
	/* It provides priority grouping control for the exception model, endian status for data
	 * accesses, and reset control of the system. 													*/
	u32 AIRCR;
	/* The SCR controls features of entry to and exit from low power state 							*/
	u32 SCR;
	/* The CCR controls entry to Thread mode and enables:
	 * -The handlers for NMI, hard fault and faults escalated by FAULTMASK to ignore bus faults
	 * -Trapping of divide by zero and unaligned accesses
	 * -Access to the STIR by unprivileged software													*/
	u32 CCR;
	/* The SHPR1-SHPR3 registers set the priority level, 0 to 15 of the exception handlers that
	 * have configurable priority, and it is byte accessible										*/
	u32 SHPR[3];
	/* The SHCSR enables the system handlers, and indicates:
	 * The pending status of the bus fault, memory management fault, and SVC exceptions
	 * The active status of the system handlers														*/
	u32 SHCSR;
	/* The CFSR indicates the cause of a memory management fault, bus fault, or usage fault 		*/
	u32 CFSR;
	/* The HFSR gives information about events that activate the hard fault handler		   			*/
	u32 HFSR;
	/* To fill the gap between two registers							 				   			*/
	u32 SCB_Reserved;
	/* Memory management fault address register 										   			*/
	u32 MMAR;
	/* Bus fault address register														   			*/
	u32 BFAR;
} SCB_t;

#define SCBx 							((SCB_t volatile* const)SCB_BASE_ADDRESS)

/****************************************************************************************************/
/* To enable an Interrupt external or system exceptions by its Number
 * Copy_u8InterruptNumber -> external Interrupt number to be enabled
 * or System Exception to be enabled like: MEMMANAGE, USAGEFAULT, BUSFAULT   */
extern STD_ERROR NVIC_EnableInterrupt(u32 Copy_u32InterruptNum);

/* To disable an Interrupt external or system exceptions by its Number
 * Copy_u8InterruptNumber -> external Interrupt number to be disabled
 * or System Exception to be disabled like: MEMMANAGE, USAGEFAULT, BUSFAULT  */
extern STD_ERROR NVIC_DisableInterrupt(u32 Copy_u32InterruptNum);

/* To set pending of an external interrupt or system exceptions
 * Copy_u8InterruptNumber -> external Interrupt number to be set its pending flag
 * or System Exception to set its pending flag like:
 * NMI, MEMMANAGE, USAGEFAULT, BUSFAULT, SVCALL  			 				*/
extern STD_ERROR NVIC_SetPending(u32 Copy_u32InterruptNum);

/* To clear pending of an external interrupt or system exceptions
 * Copy_u8InterruptNumber -> external Interrupt number to be clear its pending flag
 * or System Exception to clear its pending flag like:
 * MEMMANAGE, USAGEFAULT, BUSFAULT, SVCALL  								*/
extern STD_ERROR NVIC_ClearPending(u32 Copy_u32InterruptNum);

/* To get Pending status for given interrupt number
 * Copy_u32InterruptNum-> external Interrupt number or system exception
 * NMI, PENDSV, SYSTICK, MEMMANAGE, USAGEFAULT, BUSFAULT, SVCALL 			*/
extern STD_ERROR NVIC_GetPendingFlag(u32 Copy_u32InterruptNum,u8* Copy_PtrPendingStatus);

/* To get Active status for given interrupt number
 * Copy_u32InterruptNum-> external Interrupt number or system exception
 * NMI, PENDSV, SYSTICK, MEMMANAGE, USAGEFAULT, BUSFAULT, SVCALL 			*/
extern STD_ERROR NVIC_GetActiveFlag(u32 Copy_u32InterruptNum,u8* Copy_PtrActiveStatus);

/* To set number of Priority Groups for all interrupts 						*/
extern STD_ERROR NVIC_SetPriorityGrouping(u8 Copy_u8PriorityGroup);

/* To get the priority group												*/
extern STD_ERROR SCB_GetPriorityGroup(u8* Copy_PtrPriGroup);

/* To set Priority for given external interrupt
 * Copy_u32InterruptNum -> external interrupt number
 * Copy_u8SubPriority -> sub priority for this interrupt according to the priority group
 * if priority group is smaller than number of bits for priority to this chip so
 * this sub priority must equal to zero
 * Copy_u8Preemp -> Preemption for given external number
 * it will return NOT_OK if the priority exceed maximum priority level for each chip */
extern STD_ERROR NVIC_SetPriority(u32 Copy_u32InterruptNum,u8 Copy_u8SubPriority, u8 Copy_u8Preemp);

/* Set PRIMASK register is used to disable all exceptions except NMI and hard fault */
extern STD_ERROR NVIC_DisableGlobalInterrupt();

/* Clear PRIMASK register allows interrupts again  									*/
extern STD_ERROR NVIC_EnableGlobalInterrupt();

/* Set FAUTMASK register is used to disable all exceptions except NMI only 			*/
extern STD_ERROR NVIC_DisGloInterrAndHardFault();

/* Clear FAUTMASK register allows interrupts again  								*/
extern STD_ERROR NNVIC_EnaGloInterrAndHardFault();

/* To disable interrupts only with priority lower than or equal a certain level
 * Set BASEPRI */
/**********************/
/* m7tageen nshof lw momken ne3mlaha get w inna net2akd lw ma7tsh el raqam yraga3 error */
extern STD_ERROR NVIC_DisInterrsWithPriority(u8 Copy_u8InterruptNum);

/* To cancel the masking of BASEPRI 												*/
extern STD_ERROR NVIC_EnaInterrsWithPriority();

/* to make software interrupt for given Interrupt Number 							*/
extern STD_ERROR NVIC_SoftwareInterrupt(u32 Copy_u8InterruptNum);

/* This Function returns the exception number of the highest priority pending
 * enabled exception
 * It returns error if there is no pending interrupt at this time					*/
extern STD_ERROR SCB_GetPendingInterruptNum(u8* Copy_PtrInterruptNum);

/* This function returns the active exception number
 * It returns error if there is no Active interrupt at this time (Thread mode)		*/
extern STD_ERROR SCB_GetActiveInterruptNum(u8* Copy_PtrInterruptNum);

/* It is to change memory address of vector table
 * Copy_u32BaseOffset -> the offset of the table base from memory address 0x00000000
 * Copy_u8MemoryRegion -> Code or SRAM to choose whether the vector table is in the code
 * or SRAM memory region 																	*/
extern STD_ERROR SCB_SetVectorTableBaseOffset(u32 Copy_u32BaseOffset,u32 Copy_u8MemoryRegion);

/* To detect the vector table is set at Code or SRAM
 * 1 -> SRAM
 * 0 -> CODE																				*/
extern STD_ERROR SCB_GetVectorTableBaseOffset(u8* Copy_PtrMemoryRegion);

/* It enables faulting or halting when the processor executes an SDIV or UDIV instruction with
 * a divisor of 0																			*/
extern STD_ERROR SCB_EnableTrapDivByZero();

/* It disables faulting or halting when the processor executes an SDIV or UDIV instruction with
 * a divisor of 0																			*/
extern STD_ERROR SCB_DisableTrapDivByZero();

/* It makes a software reset for whole system												*/
extern STD_ERROR SCB_SoftwareReset(void);


/* keda elle ba2y hena
 * 1- fie function set priority n3ml 7sab el SCB ll execptions mn 1 to 15 at SHPR 1 to 3
 * 2- nshof BASEPRI mosh bet7at feha el raqm leh w n3ml enha trag3 error lw maktbsh el b raqam */





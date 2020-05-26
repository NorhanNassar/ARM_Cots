/*
 * DMA.h
 *
 *  Created on: Apr 2, 2020
 *      Author: Norhan Nassar
 */

#ifndef DMA_H_
#define DMA_H_

#define DMA_1_BASE_ADDRESS				(void*)0x40020000
#define DMA_2_BASE_ADDRESS				(void*)0x40018400

typedef void(*Dma1Ch1Cbf_t)(void);
typedef void(*Dma1Ch2Cbf_t)(void);
typedef void(*Dma1Ch3Cbf_t)(void);
typedef void(*Dma1Ch4Cbf_t)(void);
typedef void(*Dma1Ch5Cbf_t)(void);
typedef void(*Dma1Ch6Cbf_t)(void);
typedef void(*Dma1Ch7Cbf_t)(void);

typedef void(*Dma2Ch1Cbf_t)(void);
typedef void(*Dma2Ch2Cbf_t )(void);
typedef void(*Dma2Ch3Cbf_t)(void);
typedef void(*Dma2Ch4Cbf_t)(void);

typedef struct
{
	u32 ISR;							/* DMA interrupt status register
	 	 	 	 	 	 	 	 	 	 * for read any flag at DMA			 */
	u32 IFCR;							/* DMA interrupt flag clear register
	 	 	 	 	 	 	 	 	 	 * for clearing any flag			 */
	u32 CH[7][5];
}DMA1_t;

typedef struct
{
	u32 ISR;
	u32 IFCR;
	u32 CH[5][4];
}DMA2_t;

typedef struct
{
	u32 DMAController;					/* DMA1 or DMA2
	 	 	 	 	 	 	 	 	 	 * Options : DMA1, DMA2					 */
	u32 ChannelNum;						/* which channel to configure 		 	 */
	u32 PerAdd;							/* Peripheral Address 		   		 	 */
	u32 MemAdd;							/* Memory Address			   		 	 */

	u32 PerSize;						/* Peripheral Size to transfer
	 	 	 	 	 	 	 	 	 	 * Options: PERI_SIZE_8_BITS, PERI_SIZE_16_BITS,
	 	 	 	 	 	 	 	 	 	 *  PERI_SIZE_32_BITS */
	u32 MemSize;						/* Memory Size to transfer
	 	 	 	 	 	 	 	 	 	 * Options: MEM_SIZE_8_BITS, MEM_SIZE_16_BITS
	 	 	 	 	 	 	 	 	 	 * MEM_SIZE_32_BITS						 */
	u32 PerAutoInc;						/* Peripheral Address auto increment
	 	 	 	 	 	 	 	 	 	 * Options: 1-> enabled
	 	 	 	 	 	 	 	 	 	 * 			0-> disabled 	 */
	u32 MemAutoInc;						/* Memory Address auto increment
										 * Options: 1-> enabled
										 * 			0-> disabled 	 */
	u32 counter;						/* Amount to transfer 				 	 */
	s32 Direction;						/* Options: DIR_MEM2PRE, DIR_PRE2MEM 		 	 */
	u32 MemToMem;						/* if you desire to transfer from memory to memory
	 	 	 	 	 	 	 	 	 	 * Options: 1-> from memory to memory
	 	 	 	 	 	 	 	 	 	 * 			0-> from peripheral to memory or from memory to peripheral */
	u32 Periority;						/* Options: LOW, MEDIUM, HIGH, VERYHIGH		*/
	u32 CircularMode;					/* Options: 1-> enable circular mode
	 	 	 	 	 	 	 	 	 	 * 			0-> disable circular mode */
	u32 EnableInterrupts;				/* Options: TRANSFER_ERROR_INT_EN, TRANSFER_COMPLETE_INT_EN
	 	 	 	 	 	 	 	 	 	 * 			HALF_TRANSFER_INT_EN
	 	 	 	 	 	 	 	 	 	 * and it can take more than one like
	 	 	 	 	 	 	 	 	 	 * TRANSFER_ERROR_INT_EN | TRANSFER_COMPLETE_INT_EN */

	s32 DisableInterrupts;				/* Options: TRANSFER_ERROR_INT_EN, TRANSFER_COMPLETE_INT_EN
	 	 	 	 	 	 	 	 	 	 * 			HALF_TRANSFER_INT_EN
	 	 	 	 	 	 	 	 	 	 * and it can take more than one like
	 	 	 	 	 	 	 	 	 	 * TRANSFER_ERROR_INT_EN & TRANSFER_COMPLETE_INT_EN */

}DMA_cfg_t;

#define CCR		0					/* DMA channel x configuration register 	*/

#define CNDTR	1					/* DMA channel x number of data register
									 * Number of data to be transferred (0 up to 65535)
									 * This register can only be written when the
									 * channel is disabled						*/

#define CPAR	2					/* Peripheral address 						*/

#define CMAR	3					/* Memory address 							*/

#define DMA1_t			((DMA1_t volatile * const)DMA_1_BASE_ADDRESS)
#define DMA2_t			((DMA2_t volatile * const)DMA_2_BASE_ADDRESS)

#define DMA1_INIT		RCC_DMA1
#define DMA2_INIT		RCC_DMA2

/*---------------------------------------------For DMA_ISR Register---------------------------------------------*/
/* all flags here cleared by software writing 1 to the corresponding bit in the DMA_IFCR						*/
#define GIFx			0x01					   /* Channel x global interrupt flag
 	 	 	 	 	 	 	 	 	 	 	 	 	* 0-> No TE, HT or TC event on channel x
										 	 	 	* 1-> A TE, HT or TC event occurred on channel x 			*/

#define TCIFx			0x02						/* Channel x transfer complete flag
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 0-> No transfer complete (TC) event on channel x
													 * 1-> A transfer complete (TC) event occurred on channel x	*/

#define HTIFx			0x04						/* Channel x half transfer flag
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 0-> No half transfer (HT) event on channel x
													 * 1-> A half transfer (HT) event occurred on channel x		*/

#define TEIFx			0x08						/* Channel x transfer error flag
													 * 0-> No transfer error (TE) on channel x
													 * 1-> A transfer error (TE) occurred on channel x 			*/

/*---------------------------------------------For DMA_IFCR Register---------------------------------------------*/
/* all bits here are set and cleared by software 																 */
#define CGIF1			0x01				/* Channel x global interrupt clear
										 * 0-> No effect
										 * 1-> Clears the GIF, TEIF, HTIF and TCIF flags in the DMA_ISR register */

#define CTCIF1			0x02				/* Channel x transfer complete clear
										 * 0-> No effect
										 * 1->  Clears the corresponding TCIF flag in the DMA_ISR register		*/

#define CHTIF1			0x04				/* Channel x half transfer clear
										 * 0-> No effect
										 * 1-> Clears the corresponding HTIF flag in the DMA_ISR register		*/

#define CTEIF1			0x08				/* Channel x transfer error clear
										 * 0-> No effect
										 * 1-> Clears the corresponding TEIF flag in the DMA_ISR register		*/

/*---------------------------------------------For DMA_CCR Registers--------------------------------------------*/
#define EN_CH					0x001					/* Channel enable 										*/
#define TCIE					0x002					/* Transfer complete interrupt enable					*/
#define HTIE					0x004					/* Half transfer interrupt enable 						*/
#define TEIE					0x008					/* Transfer error interrupt enable 						*/

#define DIR						0x010					/* Data transfer direction
														 * 0: Read from peripheral (Peripheral to Memory)
														 * 1: Read from memory (Memory to Peripheral)								*/

#define CIRC					0x020					/* Circular mode 										*/
#define PINC					0x040					/* Peripheral increment mode 							*/
#define MINC					0x080					/* Memory increment mode 								*/
#define PSIZE_CLEAR				0x300					/* Peripheral size
														 * 00: 8-bits
														 * 01: 16-bits
														 * 10: 32-bits
														 * 11: Reserved											*/
#define MSIZE_CLEAR				0xC00					/* Memory size
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 * 00: 8-bits
														 * 01: 16-bits
														 * 10: 32-bits
														 * 11: Reserved							 				*/
#define PIR_CLEAR				0x3000					/* Channel priority level
	 	 	 	 	 	 	 	 	 	 	 	 		 * 00: Low
	 	 	 	 	 	 	 	 	 	 	 	 		 * 01: Medium
	 	 	 	 	 	 	 	 	 	 	 	 		 * 10: High
	 	 	 	 	 	 	 	 	 	 	 	 		 * 11: Very high										*/
#define MEM2MEM					0x4000					/* Memory to memory mode 								*/

/*-----------------------------------------For Configuration----------------------------------------------------*/
#define LOW								0x0000
#define MEDIUM							0x1000
#define HIGH							0x2000
#define VERYHIGH						0x3000

#define PERI_SIZE_8_BITS				0x0000
#define PERI_SIZE_16_BITS				0x0100
#define PERI_SIZE_32_BITS				0x0200

#define MEM_SIZE_8_BITS					0x0000
#define MEM_SIZE_16_BITS				0x0400
#define MEM_SIZE_32_BITS				0x0800


#define DIR_MEM2PRE						 (DIR)
#define DIR_PRE2MEM						~(DIR)

#define TRANSFER_ERROR_INT_EN			 (TEIE)
#define TRANSFER_ERROR_INT_DIS			~(TEIE)

#define TRANSFER_COMPLETE_INT_EN		 (TCIE)
#define TRANSFER_COMPLETE_INT_DIS		~(TCIE)

#define HALF_TRANSFER_INT_EN			 (HTIE)
#define HALF_TRANSFER_INT_DIS			~(HTIE)


STD_ERROR DMA_Init(void);
STD_ERROR DMA_Configure(DMA_cfg_t* DMA_cfg);
STD_ERROR DMA_setDma1Ch1Cbf(Dma1Ch1Cbf_t Dma1Ch1Cbf);
STD_ERROR DMA_setDma1Ch2Cbf(Dma1Ch2Cbf_t Dma1Ch2Cbf);
STD_ERROR DMA_setDma1Ch3Cbf(Dma1Ch3Cbf_t Dma1Ch3Cbf);
STD_ERROR DMA_setDma1Ch4Cbf(Dma1Ch4Cbf_t Dma1Ch4Cbf);
STD_ERROR DMA_setDma1Ch5Cbf(Dma1Ch5Cbf_t Dma1Ch5Cbf);
STD_ERROR DMA_setDma1Ch6Cbf(Dma1Ch6Cbf_t Dma1Ch6Cbf);
STD_ERROR DMA_setDma1Ch7Cbf(Dma1Ch7Cbf_t Dma1Ch7Cbf);

STD_ERROR DMA_setDma2Ch1Cbf(Dma2Ch1Cbf_t Dma2Ch1Cbf);
STD_ERROR DMA_setDma2Ch2Cbf(Dma2Ch2Cbf_t Dma2Ch2Cbf);
STD_ERROR DMA_setDma2Ch3Cbf(Dma2Ch3Cbf_t Dma2Ch3Cbf);
STD_ERROR DMA_setDma2Ch4Cbf(Dma2Ch4Cbf_t Dma2Ch4Cbf);


#endif /* DMA_H_ */

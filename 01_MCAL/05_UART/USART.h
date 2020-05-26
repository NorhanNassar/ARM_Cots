/*
 * USART.h
 *
 *  Created on: Mar 28, 2020
 *      Author: Norhan Nassar
 */

#ifndef USART_H_
#define USART_H_

typedef struct
{
	/* This Register to read flags' status */
	u32 SR;
	/* This Register to read from or write to the date that will be received or transmitted */
	u32 DR;
	/* this register for baud rate of transmission 			*/
	u32 BRR;
	/* these three registers to control USART */
	u32 CR1;
	u32 CR2;
	u32 CR3;
	/* Guard time and prescaler register */
	u32 GTPR;
} USARTx;

typedef struct
{
	s16 BaudRate; 				/**/
	s16 NumberOfDataBits;		/* Options: DATA_BITS_EIGHT, or DATA_BITS_NINE 				*/
	s16	ParityBit;				/* Options: NO_PARITY, EVEN_PARITY, ODD_PARITY 				*/
	s16 Enable_Interrupts;		/* Options: ENABLE_PARITY_INT, ENABLE_TC_INT, ENABLE_TXE_INT,
											ENABLE_RXNE_INT, ENABLE_IDLE_INT, NO_INT_ENABLED
									Enter here all interrupts you want to enable like:
									ENABLE_PARITY_INT | ENABLE_TC_INT and so on for all
									interrupts you want to be enabled						*/
	s16 Disable_Interrupts;		/* Options: DISABLE_PARITY_INT, DISABLE_TC_INT, DISABLE_TXE_INT,
	 	 	 	 	 	 	 	 	 	 	DISABLE_RXNE_INT, DISABLE_IDLE_INT, NO_INT_DIABLED
	 	 	 	 	 	 	 	   Enter here all interrupts you want to disable like:
								   Disable_PARITY_INT & ENABLE_TC_INT and so on for all
						       	   interrupts you want to be disabled						*/
	s16 NumberOfStopBits;		/* Options:	ONE_STOP_BIT, TWO_STOP_BIT, ONE_HALF_STOP_BIT,
											HALF_STOP_BIT									*/
	s16 ClockMode;				/* Options: ASYNCHRONOUS, SYNCHRONOUS						*/
	s16 ErrorInterrupt;			/* Options: ENABLE_ERROR_INT, DISABLE_ERROR_INT				*/
	s16 TypeOfDuplex;			/* Options: HALF_DUPLEX, FULL_DUPLEX						*/
	s16 UartUsageMode;			/* Options: INTERRUPT_MODE, DMA_MODE, POLLING_MODE			*/
	s16 RxPinModeCfg;			/* Options: RX_PIN_INPUT_FLOATING, RX_PIN_INPUT_PULLUP		*/
	s16 TXPinSpeedCfg;			/* Options: TX_SPEED_10MHZ, TX_SPEED_2MHZ, TX_SPEED_50MHZ	*/
	s16 EnableDmaInt;			/* Options: TRANSFER_COMPLETE_INT_EN,TRANSFER_COMPLETE_INT_EN
								 * HALF_TRANSFER_INT_EN, NO_INT_ENABLED
								 * Enter here all interrupts you want to enable
								 * if you use DMA mode
								 * like: TRANSFER_COMPLETE_INT_EN | TRANSFER_COMPLETE_INT_EN
								 * and so on for all										*/
	s16 DisableDmaInt;			/* Options: TRANSFER_COMPLETE_INT_EN,TRANSFER_COMPLETE_INT_EN
	 	 	 	 	 	 	 	 * HALF_TRANSFER_INT_EN, NO_INT_DISABLED
	 	 	 	 	 	 	 	 * Enter here all interrupts you want to disable
	 	 	 	 	 	 	 	 * if you use DMA mode
	 	 	 	 	 	 	 	 * like: TRANSFER_COMPLETE_INT_EN & TRANSFER_COMPLETE_INT_EN
	 	 	 	 	 	 	 	 * and so on for all										*/
} USART_Init_Cfg_t;

typedef struct					/* user can configure these items by calling a function configure */
{
	s16 BaudRate; 				/**/
	s16 NumberOfDataBits;		/* Options: DATA_BITS_EIGHT, or DATA_BITS_NINE 				*/
	s16	ParityBit;				/* Options: NO_PARITY, EVEN_PARITY, ODD_PARITY 				*/
	s16 NumberOfStopBits;		/* Options:	ONE_STOP_BIT, TWO_STOP_BIT, ONE_HALF_STOP_BIT,
												HALF_STOP_BIT									*/
	/* there will be flow control later */
} USART_cfg_t;

/* There will be a struct for synchronous type later */

typedef void (*txCbf_t)(void);
typedef void (*rxCbf_t)(void);

#define USART_BASE_ADDRESS				(void*)0x40013800
#define USART1x 						((USARTx volatile * const)USART_BASE_ADDRESS)

/*-------------------------------USART1_SR Register---------------------------------*/
#define CTS_FLAG						0x200		/* CTS flag 					*/
#define LBD_FLAG						0x100		/* LIN break detection flag 	*/
#define TXE_FLAG						0x080		/* Transmit data register empty */
#define TC_FLAG							0x040		/* Transmission complete 		*/
#define RXNE_FLAG						0x020		/* Read data register not empty */
#define IDLE_FLAG						0x010		/* IDLE line detected 			*/
#define ORE_FLAG						0x008		/* Overrun error 				*/
#define NE_FLAG							0x004		/* Noise error flag 			*/
#define FE_FLAG							0x002		/* Framing error 				*/
#define PE_FLAG							0x001		/* Parity error 				*/
/*-------------------------------USART1_CR1 Register--------------------------------*/
#define UE_ENABLE						0x2000		/* 0 -> USART disabled, 1 -> USART enabled 	 	*/

#define M_WORD_LENGTH					0x1000		/* it is for word length
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 0 -> 1 Start bit, 8 Data bits, n Stop bit
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 1 -> 1 Start bit, 9 Data bits, n Stop bit 	*/

#define WAKE							0x0800		/* This bit determines the USART wakeup method
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 0 -> Idle Line
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 1 -> Address Mark 							*/

#define PCE_PARITY_CONTROL				0x0400		/* to enable or disable hardware generation of parity bit
 	 	 	 	 	 	 	 	 	 	 	 	 	 * the computed parity is inserted at the MSB position
 	 	 	 	 	 	 	 	 	 	 	 	 	 * (9th bit if M=1; 8th bit if M=0)				*/

#define PS_PARITY_SELECTION				0x0200		/* This bit selects the odd or even parity
 	 	 	 	 	 	 	 	 	 	 	 	 	 * when the parity generation/detection is enabled (PCEbit set)
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 0 -> Even parity
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 1 -> Odd parity								*/

#define PEIE_PARITY_INT_EN				0x0100		/* parity interrupt enable bit
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 0 -> Interrupt is inhibited
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 1 -> A USART interrupt is generated
 	 	 	 	 	 	 	 	 	 	 	 	 	 * whenever PE=1 in the USART_SR register		*/

#define TXEIE_TXE_INT_EN				0x0080		/* TXE interrupt enable bit
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 0 -> Interrupt is inhibited
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 1 -> A USART interrupt is generated
 	 	 	 	 	 	 	 	 	 	 	 	 	 * whenever TXE=1 in the USART_SR register		*/

#define TCIE_TC_INT						0x0040		/* TC interrupt enable bit
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 0 -> Interrupt is inhibited
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 1 -> A USART interrupt is generated
 	 	 	 	 	 	 	 	 	 	 	 	 	 * whenever TC=1 in the USART_SR register		*/

#define RXNEIE_RXNE_INT					0x0020		/* RXNE interrupt enable bit
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 0 -> Interrupt is inhibited
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 1 -> A USART interrupt is generated
 	 	 	 	 	 	 	 	 	 	 	 	 	 * whenever ORE=1 or RXNE=1
 	 	 	 	 	 	 	 	 	 	 	 	 	 * in the USART_SR register						*/

#define IDLEIE_INT						0x0010		/* Idle interrupt enable bit
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 0 -> Interrupt is inhibited
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 1 -> A USART interrupt is generated
 	 	 	 	 	 	 	 	 	 	 	 	 	 * whenever IDLE=1 in the USART_SR register		*/

#define TE_EN							0x0008		/* Transmitter enable bit 						*/

#define RE_EN							0x0004		/* Receiver enable bit 							*/

#define RWU_WAKEUP						0x0002		/* Receiver wake up bit
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 0 -> Receiver in active mode
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 1 -> Receiver in mute mode					*/

#define SBK_SEND_BREAK					0x0001		/* to send a break
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 0 -> No break character is transmitted
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 1 -> Break character will be transmitted		*/
/*-------------------------------------USART1_CR2 Register------------------------------------------*/
#define LIN_EN							0x4000		/* LIN mode enable 								*/

#define STOP_BITS_CLEAR					0x3000		/* These bits are used for programming the stop bits.
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 00 -> 1 Stop bit
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 01 -> 0.5 Stop bit
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 10 -> 2 Stop bits
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 11 -> 1.5 Stop bit 							 */
#define CLK_EN							0x0800		/* This bit allows the user to enable the CK pin */

#define CLK_POL							0x0400		/* This bit allows the user to select the polarity of
 	 	 	 	 	 	 	 	 	 	 	 	 	 * the clock output on the CK pin in synchronous mode
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 0 -> Steady low value on CK pin outside transmission window.
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 1 -> Steady high value on CK pin outside transmission window */

#define CLX_PHA							0x0200		/* This bit allows the user to select the phase of
 	 	 	 	 	 	 	 	 	 	 	 	 	 * the clock output on the CK pin in synchronous mode
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 0 -> The first clock transition is the first data capture edge.
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 1 -> The second clock transition is the first data capture edge. */

#define LAST_BIT_CL						0x0100		/* Last bit clock pulse
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 0 -> The clock pulse of the last data bit is not output to the CK pin
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 1 -> The clock pulse of the last data bit is output to the CK pin	*/

#define LBDIE							0x0040		/* LIN break detection interrupt enable
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 0 -> Interrupt is inhibited
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 1 -> An interrupt is generated whenever LBD=1 in the USART_SR register */

#define LBDL							0x0020		/* lin break detection length
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 0 -> 10 bit break detection
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 1 -> 11 bit break detection 					*/
/*-------------------------------------USART1_CR3 Register------------------------------------------*/
#define CTSIE							0x0400		/* CTS interrupt enable							*/
#define CTSE							0x0200		/* CTS enable 									*/

#define RTSE_FLOW_CONTROL				0x0100		/* RTS enable
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 0-> RTS hardware flow control disabled
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 1-> RTS interrupt enabled,
 	 	 	 	 	 	 	 	 	 	 	 	 	 * data is only requested when there is space in the receive buffer	*/

#define DMA_T_EN						0x0080		/* DMA enable transmitter 						*/
#define DMA_R_EN						0x0040		/* DMA enable receiver 							*/
#define SC_EN							0x0020		/* Smartcard mode enable 						*/
#define NACK							0x0010		/* Smartcard NACK enable 						*/
#define HD_SEL							0x0008		/* Half-duplex selection 						*/
#define IR_LP							0x0004		/* IrDA low-power 								*/
#define IR_EN							0x0002		/* IrDA mode enable 							*/

#define ERROR_INT_EN					0x0001		/* Error interrupt enable
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 0 -> Interrupt is inhibited
 	 	 	 	 	 	 	 	 	 	 	 	 	 * 1 -> An interrupt is generated whenever DMAR=1 in the USART_CR3 register
 	 	 	 	 	 	 	 	 	 	 	 	 	 * and FE=1 or ORE=1 or NE=1 in the USART_SR register					*/

/*-------------------------------------For configuration------------------------------------------*/
#define DATA_BITS_EIGHT					~(M_WORD_LENGTH)
#define DATA_BITS_NINE					M_WORD_LENGTH

#define NO_PARITY						~(PCE_PARITY_CONTROL)
#define EVEN_PARITY						  0
#define ODD_PARITY						  1

#define DISABLE_PARITY_INT				~(PEIE_PARITY_INT_EN)
#define ENABLE_PARITY_INT				 PEIE_PARITY_INT_EN

#define DISABLE_TC_INT					~(TCIE_TC_INT)
#define ENABLE_TC_INT					TCIE_TC_INT

#define DISABLE_TXE_INT					~(TXEIE_TXE_INT_EN)
#define ENABLE_TXE_INT					TXEIE_TXE_INT_EN

#define DISABLE_RXNE_INT				~(RXNEIE_RXNE_INT)
#define ENABLE_RXNE_INT					RXNEIE_RXNE_INT

#define DISABLE_IDLE_INT				~(IDLEIE_INT)
#define ENABLE_IDLE_INT					IDLEIE_INT

#define DISABLE_ERROR_INT				~(ERROR_INT_EN)
#define ENABLE_ERROR_INT				ERROR_INT_EN

#define ASYNCHRONOUS					~(CLK_EN)
#define SYNCHRONOUS						CLK_EN

#define FULL_DUPLEX						~(HD_SEL)
#define HALF_DUPLEX						HD_SEL

#define RX_PIN_INPUT_FLOATING			MODE_IP_FLOATING
#define RX_PIN_INPUT_PULLUP				MODE_IP_PUPD

#define TX_SPEED_2MHZ					SPEED_2MHZ
#define TX_SPEED_10MHZ					SPEED_10MHZ
#define TX_SPEED_50MHZ					SPEED_50MHZ

#define NO_INT_ENABLED					0x00
#define NO_INT_DISABLED					0xFFFFFFFF

#define ONE_STOP_BIT					0x2000
#define TWO_STOP_BIT					0x0000
#define ONE_HALF_STOP_BIT				0x3000
#define HALF_STOP_BIT					0x1000

#define INTERRUPT_MODE					0
#define DMA_MODE						1
#define POLLING_MODE					2

/*--------------------------------------Functions Prototypes--------------------------------*/
extern STD_ERROR USART_Init();
extern STD_ERROR USART_Configure(USART_cfg_t* USART_cfg);
extern STD_ERROR USART_Send(u8* bufferData, u32 bytesCount);
extern STD_ERROR USART_Recieve(u8* bufferData, u32 bytesCount);
extern STD_ERROR USART_setTxCbf (txCbf_t txCbf);
extern STD_ERROR USART_setRxCbf (rxCbf_t rxCbf);

#endif /* USART_H_ */

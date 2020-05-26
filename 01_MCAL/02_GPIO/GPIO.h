/************************************************/
/*  Author  : Norhan Nassar    			     	*/
/*  Version : V02          						*/
/*  Date    : 1 March 2020    					*/
/************************************************/

#ifndef GPIO_INTERFACE_H
#define GPIO_INTERFACE_H

/* Base addresses for each GPIO Registers */
#define GPIOA_BASE_ADDRESS     (void*)0x40010800
#define GPIOB_BASE_ADDRESS     (void*)0x40010C00
#define GPIOC_BASE_ADDRESS     (void*)0x40011000
#define GPIOD_BASE_ADDRESS     (void*)0x40011400
#define GPIOE_BASE_ADDRESS     (void*)0x40011800
#define GPIOF_BASE_ADDRESS     (void*)0x40011C00
#define GPIOG_BASE_ADDRESS     (void*)0x40012000

/* Struct for GPIO registers */
typedef struct{
	u32 CRL;
	u32 CRH;
	u32 IDR;
	u32 ODR;
	u32 BSRR;
	u32 BRR;
	u32 LCKR;
} GPIO_t;

#define GPIOA		((GPIO_t volatile * const)GPIOA_BASE_ADDRESS)
#define GPIOB		((GPIO_t volatile * const)GPIOB_BASE_ADDRESS)
#define GPIOC		((GPIO_t volatile * const)GPIOC_BASE_ADDRESS)
#define GPIOD		((GPIO_t volatile * const)GPIOD_BASE_ADDRESS)
#define GPIOE		((GPIO_t volatile * const)GPIOE_BASE_ADDRESS)
#define GPIOF		((GPIO_t volatile * const)GPIOF_BASE_ADDRESS)
#define GPIOG		((GPIO_t volatile * const)GPIOG_BASE_ADDRESS)

/* Masks for configure inputs and outputs Pins */
#define	MODE_OP_AF_PP			0b1011
#define MODE_OP_AF_OD			0b1111
#define	MODE_OP_GP_PP			0b0011
#define MODE_OP_GP_OD			0b0111

#define MODE_IP_ANALOG			0b0000
#define MODE_IP_FLOATING		0b0100
#define MODE_IP_PUPD			0b1000

#define IP_NOSPEED				0b00
#define SPEED_10MHZ				0b01
#define SPEED_2MHZ				0b10
#define SPEED_50MHZ				0b11

#define	CLEAR_4BITS				0b1111
#define	CLEAR_2BITS				0b11

#define SHIFTNUMBITS			4

/* Masks for each pin */
#define GPIO_PIN0			0x1
#define GPIO_PIN1			0x2
#define GPIO_PIN2			0x4
#define GPIO_PIN3			0x8
#define GPIO_PIN4			0x10
#define GPIO_PIN5			0x20
#define GPIO_PIN6			0x40
#define GPIO_PIN7			0x80
#define GPIO_PIN8			0x100
#define GPIO_PIN9			0x200
#define GPIO_PIN10			0x400
#define GPIO_PIN11			0x800
#define GPIO_PIN12			0x1000
#define GPIO_PIN13			0x2000
#define GPIO_PIN14			0x4000
#define GPIO_PIN15			0x8000

#define CRL_THREASHOLD		8
#define PINS_MASK			16

#define PORTA				0
#define PORTB				1
#define PORTC				2
#define PORTD				3
#define PORTE				4
#define PORTF				5
#define PORTG				6



typedef struct{

	u32 Pin;
	u32 Mode_Configure;
	u32 Mode_Speed;
	u32 Port;
} GPIO_Conf_Pin_t;

/********************************************************************************************************/
/* Description: This API shall Configure each Pin and it could configure more than one bit at a time   	*/
/* Arguments: Address of struct to configure										  			      	*/
/* Pin -> from GPIO_PIN0 to GPIO_PIN15 and it could be GPIO_PIN0 | GPIO_PIN1 and etc till GPIO_PIN15    */
/* Port -> which Port to enable given bit on, from PORTA to PORTG                                       */
/* Mode_Configure -> MODE_OP_AF_PP, MODE_OP_AF_OD, MODE_OP_GP_PP, MODE_OP_GP_OD      			        */
/* Mode_ Speed if mode is output -> SPEED_10MHZ, SPEED_2MHZ, SPEED_50MHZ  						        */
/* Mode_ Speed if mode is input -> IP_NOSPEED  													        */
/********************************************************************************************************/
extern STD_ERROR GPIO_voidConfigure(GPIO_Conf_Pin_t* Copy_structConfPin);

/********************************************************************************************************/
/* Description: This API shall Write on a certain pin at a certain port 1 or 0 						   	*/
/* Arguments: 																		  			      	*/
/* Port -> from PORTA to PORTG                   									                    */
/* Pin -> from GPIO_PIN0 to GPIO_PIN15																    */
/* State -> SET => 1, RESET => 0												     			        */
/********************************************************************************************************/
extern STD_ERROR GPIO_u8Write(u32 Port , u16 Pin , STD_WRITE state);

/********************************************************************************************************/
/* Description: This API shall Read from certain pin at a certain port 1 or 0 						   	*/
/* Arguments: 																		  			      	*/
/* Port -> from PORTA to PORTG                   									                    */
/* Pin -> from GPIO_PIN0 to GPIO_PIN15																    */
/* Pointer to data -> to set data read at it												     	    */
/********************************************************************************************************/
extern STD_ERROR GPIO_u8Read(u32 Port ,u16 Pin,u8* Data);

#endif

#include "STD_TYPES.h"

#include "GPIO.h"

/* GPIO_PORT array of GPIO_t struct type  */
static GPIO_t volatile * const GPIO_PORT [7]={GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG};

/* Function like Macro to check given port and pin */
#define CHECKPORT(PortIn)					(PortIn!=PORTA)&&(PortIn!=PORTB)&&(PortIn!=PORTC)&&(PortIn!=PORTD)&&(PortIn!=PORTE)&&(PortIn!=PORTF)&&(PortIn!=PORTG)
#define CHECKPIN(PinIn)						(PinIn!=GPIO_PIN0)&&(PinIn!=GPIO_PIN1)&&(PinIn!=GPIO_PIN2)&&(PinIn!=GPIO_PIN3)&&(PinIn!=GPIO_PIN4)&&(PinIn!=GPIO_PIN5)&&(PinIn!=GPIO_PIN6)&&(PinIn!=GPIO_PIN7)&&(PinIn!=GPIO_PIN8)&&(PinIn!=GPIO_PIN9)&&(PinIn!=GPIO_PIN10)&&(PinIn!=GPIO_PIN11)&&(PinIn!=GPIO_PIN12)&&(PinIn!=GPIO_PIN13)&&(PinIn!=GPIO_PIN14)&&(PinIn!=GPIO_PIN15)

/* To check Output Mode and Speed */
#define CHECKMODEIP(Mode)               	((Mode==MODE_IP_ANALOG)||(Mode==MODE_IP_FLOATING)||(Mode==MODE_IP_PUPD))
#define CHECKIPMODESPEED(Mode,Speed)		(CHECKMODEIP(Mode)&&(Speed==IP_NOSPEED)))

/* To check Input Mode and NoSpeed */
#define CHECKMODEOP(Mode)					((Mode==MODE_OP_AF_PP)||(Mode==MODE_OP_AF_OD)||(Mode==MODE_OP_GP_PP)||(Mode== MODE_OP_GP_OD))
#define CHECKSPEED(Speed)					((Speed==SPEED_10MHZ)||(Speed==SPEED_2MHZ)||(Speed==SPEED_50MHZ))
#define CHECKOPMODESPEED(Mode,Speed)		(CHECKMODEOP(Mode)&&CHECKSPEED(Speed))

/*********************************************************************************/
STD_ERROR GPIO_voidConfigure(GPIO_Conf_Pin_t* Copy_structConfPin)
{
	STD_ERROR loc_err;
	u8 Pin_Num=0;
	u16 Pin_Temp = Copy_structConfPin->Pin;
	loc_err = NOT_OK;
	while(Pin_Temp!=0)
	{
		if(Pin_Temp&1)
		{
			if(Pin_Num < CRL_THREASHOLD)
			{
				if( (CHECKOPMODESPEED(Copy_structConfPin->Mode_Configure,Copy_structConfPin->Mode_Speed)||(CHECKIPMODESPEED(Copy_structConfPin->Mode_Configure,Copy_structConfPin->Mode_Speed)) )
						{
					GPIO_PORT[Copy_structConfPin->Port]->CRL &= ~(CLEAR_4BITS<<(SHIFTNUMBITS*Pin_Num));
					GPIO_PORT[Copy_structConfPin->Port]->CRL |= Copy_structConfPin->Mode_Configure<<(SHIFTNUMBITS*Pin_Num);
					GPIO_PORT[Copy_structConfPin->Port]->CRL &= ~(CLEAR_2BITS<<(SHIFTNUMBITS*Pin_Num));
					GPIO_PORT[Copy_structConfPin->Port]->CRL |= Copy_structConfPin->Mode_Speed<<(SHIFTNUMBITS*Pin_Num);
					loc_err = OK;
						}
			}
			else
			{
				if( (CHECKOPMODESPEED(Copy_structConfPin->Mode_Configure,Copy_structConfPin->Mode_Speed)||(CHECKIPMODESPEED(Copy_structConfPin->Mode_Configure,Copy_structConfPin->Mode_Speed)) )
						{
					GPIO_PORT[Copy_structConfPin->Port]->CRH &= ~(CLEAR_4BITS<<(SHIFTNUMBITS*(Pin_Num-8)));
					GPIO_PORT[Copy_structConfPin->Port]->CRH |= Copy_structConfPin->Mode_Configure<<(SHIFTNUMBITS*(Pin_Num - CRL_THREASHOLD));
					GPIO_PORT[Copy_structConfPin->Port]->CRH &= ~(CLEAR_2BITS<<(SHIFTNUMBITS*(Pin_Num-8)));
					GPIO_PORT[Copy_structConfPin->Port]->CRH |= Copy_structConfPin->Mode_Speed<<(SHIFTNUMBITS*(Pin_Num - CRL_THREASHOLD));
					loc_err = OK;
						}
			}
		}
		Pin_Temp=Pin_Temp>>1;
		Pin_Num++;
	}
	return loc_err;

}
/*********************************************************************************/
STD_ERROR GPIO_u8Write(u32 Copy_u32Port , u16 Copy_u16Pin , STD_WRITE state)
{
	STD_ERROR loc_err = OK;

	if( (CHECKPORT(Copy_u32Port)) || (CHECKPIN(Copy_u16Pin)) )
	{
		loc_err = NOT_OK;
		return loc_err;
	}

	switch(state)
	{
	case SET:
		GPIO_PORT[Copy_u32Port]->BSRR = Copy_u16Pin;
		break;
	case RESET:
		GPIO_PORT[Copy_u32Port]->BRR = Copy_u16Pin;
		break;
	default:
		loc_err = NOT_OK;
		break;
	}
	return loc_err;
}
/*********************************************************************************/
STD_ERROR GPIO_u8Read(u32 Copy_u32Port , u16 Copy_u16Pin , u8* Data)
{
	STD_ERROR loc_err = OK;
	u16 Pin_Temp = Copy_u16Pin;
	u8 Pin_Num = 0;
	if( (CHECKPORT(Copy_u32Port)) || (CHECKPIN(Copy_u16Pin)) || (!Data) )
	{
		loc_err = NOT_OK;
		return loc_err;
	}

	while(Pin_Temp!=0)
	{
		if(Pin_Temp&1)
			break;
		Pin_Temp = Pin_Temp >> 1;
		Pin_Num ++;
	}

	*Data = (GPIO_PORT[Copy_u32Port]->IDR>>Pin_Num)&1;

	return loc_err;

}
/************************************************************************************/

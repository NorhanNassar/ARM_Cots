#include "STD_TYPES.h"
#include "GPIO_interface.h"
#include "HLED_Configure.h"
#include "HLED_interface.h"

extern Leds_Port_t Leds_Port_Map[];

STD_ERORR Led_init(void)
{
	STD_ERORR loc_err = OK;
	u8 PortCounter,PinCounter;
	GPIO_Conf_Pin_t Led_GPIO_Pin;				/* struct of type GPIO to configure it with this one */
	Led_GPIO_Pin.Pin=CLEAR;						/* Initialize Pin at GPIO struct to 0 */

	for(PortCounter=0;PortCounter<PORTS_NUM;PortCounter++)
	{
		Led_GPIO_Pin.Port = Leds_Port_Map[PortCounter].Leds_Port;
		for(PinCounter=0;PinCounter<Leds_Port_Map[PortCounter].Leds_Port_Num;PinCounter++)
		{
			Led_GPIO_Pin.Pin |= Leds_Port_Map[PortCounter].Leds_Port_Pins[PinCounter].Led_Pin;
		}
		Led_GPIO_Pin.Mode_Configure = MODE_OP_GP_PP;
		Led_GPIO_Pin.Mode_Speed=SPEED_2MHZ;
		GPIO_voidConfigure(&Led_GPIO_Pin);
	}
	return loc_err;
}

STD_ERORR Led_SetPinState(u8 Copy_u8LedNum,u8 Copy_u8State)
{
	STD_ERORR loc_err = OK;
	u8 PortCounter,PinCounter;
	for(PortCounter=0;PortCounter<PORTS_NUM;PortCounter++)
	{
		for(PinCounter=0;PinCounter<Leds_Port_Map[PortCounter].Leds_Port_Num;PinCounter++)
		{
			if(Leds_Port_Map[PortCounter].Leds_Port_Pins[PinCounter].Led_Num==Copy_u8LedNum)
			{
				switch (Leds_Port_Map[PortCounter].Leds_Port_Pins[PinCounter].LED_STATE)
				{
				case LED_ACTIVE_HIGH:
					GPIO_voidWrite(Leds_Port_Map[PortCounter].Leds_Port , Leds_Port_Map[PortCounter].Leds_Port_Pins[PinCounter].Led_Pin , Copy_u8State);
					break;
					break;
				case LED_ACTIVE_LOW:
					GPIO_voidWrite(Leds_Port_Map[PortCounter].Leds_Port , Leds_Port_Map[PortCounter].Leds_Port_Pins[PinCounter].Led_Pin , !Copy_u8State);
					break;
					break;
				}
			}
		}
	}
	return loc_err;
}




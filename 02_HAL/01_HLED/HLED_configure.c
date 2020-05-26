/*
 * HLED_Configure.c
 *
 *  Created on: Mar 12, 2020
 *      Author: Electronica CARE
 */
#include "STD_TYPES.h"
#include "GPIO_interface.h"
#include "HLED_interface.h"
#include "HLED_Configure.h"

/* Configure each Led on your system to a certain Pin at a certain Port and configure its state -> Active High or Active Low */
#define LEDS_PORTC { {LED_0,GPIO_PIN13,LED_ACTIVE_LOW},{LED_1,GPIO_PIN14,LED_ACTIVE_LOW},{LED_2,GPIO_PIN15,LED_ACTIVE_HIGH} };
#define LEDS_PORTB { {LED_3,GPIO_PIN0,LED_ACTIVE_HIGH},{LED_4,GPIO_PIN1,LED_ACTIVE_LOW},{LED_5,GPIO_PIN2,LED_ACTIVE_HIGH} };

extern Led_Pin_t Leds_PORTC[LEDS_PORTC_NUM]=LEDS_PORTC;
Led_Pin_t Leds_PORTB[LEDS_PORTB_NUM]=LEDS_PORTB;
Leds_Port_t Leds_Port_Map[PORTS_NUM] = { {PORTC,LEDS_PORTC_NUM,(Led_Pin_t*)&Leds_PORTC}, {PORTB,LEDS_PORTB_NUM,(Led_Pin_t*)&Leds_PORTB} };







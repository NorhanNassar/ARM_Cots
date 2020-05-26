/*
 * LCD_cfg_V10.h
 *
 *  Created on: Apr 12, 2020
 *      Author: Norhan Nassar
 */

#ifndef LCD_CFG_V10_H_
#define LCD_CFG_V10_H_

#define LCDPINS					11				/* Options: 11 if it is 8 bit Mode
												 * 			Number of pins at lcd (RS, RW, E, from D0 to D7)
												 * 			or 7 if it is 4 bit Mode
												 * 			Number of pins at lcd (RS, RW, E, from D4 to D7)	*/

/* to configure which mode it will work, 8bit or 4bit 					*/
/* Options: FOUR_BIT_MODE, EIGHT_BIT_MODE								*/
#define LCDMODE				EIGHT_BIT_MODE

/* Number of Ports mapped to the lcd */
#define LCDPORTSNUM			1


#endif /* LCD_CFG_V10_H_ */

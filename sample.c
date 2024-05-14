/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "button_EXINT/button.h"
#include "Joystick/joystick.h"
#include "GLCD/GLCD.h" 
#include "game/game.h"

/* for simulator */
#define SIMULATOR 1

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif
extern int but_0;



int main(void)
{

	but_0=0;

  SystemInit();  												/* System Initialization (i.e., PLL)  */
	BUTTON_init();	 											/* BUTTON Initialization              */
  LCD_Initialization();									/* LCD Initialization */
	joystick_init();											/* Joystick Initialization */
	
	// rit for buttons and joystick
	init_RIT(0x004C4B40);									 /* RIT -> 50 ms, RIT set at 100MHZ */
	
	// timer 0 for turn 
	init_timer(0,0x17D7840);               /* 1 sec */

	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);	
	//show start screen
	startscreen();
  while (1)	
  {
		if(but_0 == 0){
			__ASM("wfi");
		}
			else if(but_0 == 1){
				//start_single_human();
				choose_mode();
				but_0=0;
				//enable_timer(0);
				enable_RIT();
			}
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

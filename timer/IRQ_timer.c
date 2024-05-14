/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <stdio.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h"
#include "../Joystick/joystick.h"


/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern Pedina p1;
extern Pedina p2;
extern Walls w;
extern Match m;

void TIMER0_IRQHandler (void){
	if(p1.win == 1 || p2.win ==1){
		LPC_TIM0->IR = 1;			/* clear interrupt flag */
		return;
	}
	else{
	if(p1.turnOn ==1){            // turno del player 1, decremento il contatore time left ogni secondo
		if(p1.time_left != 255) {
			if(m.Player == 1 && p1.time_left == 17){
				NPCmove();
			}
			GUI_Text(95, 297,"P1 Turn", Red, BACKGROUND);
			LCD_DisplayTimer(p1.time_left);
			p1.time_left--;
		}
		else{
			LCD_DisplayMove(5,p1);
			swapTurn(&p1,&p2);					//funzione per cambio turno
		}
	}
	else {
		if(p2.time_left != 255) {			
				// turno del player 2, decremento il contatore time left ogni secondo
			GUI_Text(95, 297,"P2 Turn", Blue, BACKGROUND);
			LCD_DisplayTimer(p2.time_left);
			p2.time_left--;
			
		}
		else{
			LCD_DisplayMove(5,p2);
			swapTurn(&p1,&p2);																//funzione per cambio turno
		}
	}
}

  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/

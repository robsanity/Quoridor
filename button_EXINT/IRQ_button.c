#include "button.h"
#include "lpc17xx.h"
#include "../game/game.h"

extern int but_1;
extern int but_2;
extern int but_0;
extern Pedina p1;
extern Pedina p2;
#define ROWS 13
#define COLS 13

#include <stdio.h>


void EINT0_IRQHandler (void)	  	/* INT0														 */       //bottone per startare il game
{		
	but_0 = 1;
	NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */	
	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */      //bottone per entrare/uscire in/da wallmode
{
  but_1 = 1;
	NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */      //bottone per girare  muro
{
	but_2 = 1;
	NVIC_DisableIRQ(EINT2_IRQn);
	LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
  
	LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
}



/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../GLCD/GLCD.h" 
#include "../game/game.h"
#include "../Joystick/joystick.h"
#include "../button_EXINT/button.h"

extern Pedina p1;
extern Pedina p2;
extern Walls w;
extern Match m;
volatile int but_0=0;
volatile int but_1 = 0;
volatile int but_2 = 0;
volatile int down = 0;
volatile int up = 0;
volatile int left = 0;
volatile int right = 0;
volatile int select;

void RIT_IRQHandler(void) {
	int i;
		disable_RIT();										//spostato il RIT disable e reset a inizio funzione in quanto non funzionava prima
    reset_RIT();	
    if ((LPC_GPIO1->FIOPIN & (1<<29)) == 0) {             
        up++;																//gestione input su, controllo di chi è il turno, controllo se sono in wall mode o movemode, 
				if(m.mode == 1 && m.Board == 0 && m.Player == 0){
					LCD_DrawSquare(60, 120, 180, 180 , Green);
					LCD_DrawSquare(60, 200, 180, 260 , Red);
					left = 0;
          right = 0;
          down = 0;
				} else
				if(m.mode == 1 && (m.Board == 1 || m.Board == 2) && m.Player == 0){
					LCD_DrawSquare(60, 120, 180, 180 , Green);
					LCD_DrawSquare(60, 200, 180, 260 , Red);
					left = 0;
          right = 0;
          down = 0;
				} else
        if (p1.turnOn == 1 && m.Player == 2 && p1.extendedMode == 0) {
            if (p1.wall_mode == 0) {
                if (up >= 1) {
									up = 1;
									for(i=0; i<8; i+=2){				
									if((p1.position[1] - 1 == p1.possibleMoves[i+1] || p1.position[1] - 2 == p1.possibleMoves[i+1]) && p1.position[0] == p1.possibleMoves[i]){
                    movePedina1(0);			//se sono in move controllo sia possibile spostare la pedina in quel punto
                    left = 0;
                    right = 0;
                    down = 0;
									}
                }
							}
            } else if (p1.wall_mode) {
							moveWall(0,&w);				//sposto muro, il resto è uguale per ogni direzione
                
        }
			}else if (p1.turnOn == 1 && m.Player == 2 && p1.extendedMode == 1 && (p1.extOrien == 1)) {
				if (up >= 1) {
									up = 1;
                    movePedina1(9);			
                    left = 0;
                    right = 0;
                    down = 0;
									}
                }
				

        if (p2.turnOn == 1 && p2.extendedMode == 0) {
            if (p2.wall_mode == 0) {
                if (up >= 1) {
									up = 1;
									for(i=0; i<8; i+=2){
									if((p2.position[1] -1 == p2.possibleMoves[i+1] || p2.position[1] -2 == p2.possibleMoves[i+1]) && p2.position[0] == p2.possibleMoves[i]){
                    movePedina2(0);
                    left = 0;
                    right = 0;
                    down = 0;
									}
                }
            }
					}							
						else if (p2.wall_mode) {
										moveWall(0,&w);
                }
            }
				else if (p2.turnOn == 1 && p2.extendedMode == 1 && (p2.extOrien == 1)) {
				if (up >= 1) {
									up = 1;
                    movePedina2(9);			
                    left = 0;
                    right = 0;
                    down = 0;
									}
                }
        }
		

    if ((LPC_GPIO1->FIOPIN & (1<<26)) == 0) {
        down++;
			if(m.mode == 1 && m.Board == 0 && m.Player == 0){
					LCD_DrawSquare(60, 200, 180, 260 , Green);
					LCD_DrawSquare(60, 120, 180, 180 , Red);
					left = 0;
          right = 0;
          up = 0;
				} else 
			if(m.mode == 1 && (m.Board == 1 || m.Board == 2) && m.Player == 0){
					LCD_DrawSquare(60, 120, 180, 180 , Red);
					LCD_DrawSquare(60, 200, 180, 260 , Green);
					left = 0;
          right = 0;
          up = 0;
				} else
        if (p1.turnOn == 1 && m.Player == 2 && p1.extendedMode == 0) {
            if (p1.wall_mode == 0) {
                if (down >= 1) {
									down = 1;
									for(i=0; i<8; i+=2){
									if((p1.position[1] +1 == p1.possibleMoves[i+1] || p1.position[1] +2 == p1.possibleMoves[i+1]) && p1.position[0] == p1.possibleMoves[i]){
                    movePedina1(1);
                    left = 0;
                    right = 0;
                    up = 0;
									}
								}
                }
            } else if (p1.wall_mode) {
                moveWall(1,&w);
            }
        }
				else if (p1.turnOn == 1 && m.Player == 2 && p1.extendedMode == 1 && (p1.extOrien == 1)) {
				if (down >= 1) {
									down = 1;
                    movePedina1(10);			
                    left = 0;
                    right = 0;
                    up = 0;
									}
                }

        if (p2.turnOn == 1 && p2.extendedMode == 0) {
            if (p2.wall_mode == 0) {
                if (down >= 1) {
									down = 1;
									for(i=0; i<8; i+=2){
									if((p2.position[1] +1 == p2.possibleMoves[i+1] || p2.position[1] +2 == p2.possibleMoves[i+1]) && p2.position[0] == p2.possibleMoves[i]){
                    movePedina2(1);
                    left = 0;
                    right = 0;
                    up = 0;
									}
								}
                }
            } else if (p2.wall_mode) {
                moveWall(1,&w);
            }
        }else if (p2.turnOn == 1 && p2.extendedMode == 1 && (p2.extOrien == 1)) {
				if (down >= 1) {
									down = 1;
                    movePedina2(10);			
                    left = 0;
                    right = 0;
                    up = 0;
									}
                }
    }

    if ((LPC_GPIO1->FIOPIN & (1<<27)) == 0) {
        left++;
        if (p1.turnOn == 1 && m.Player == 2 && p1.extendedMode == 0) {
            if (p1.wall_mode == 0) {
                if (left >= 1) {
									left = 1;
									for(i=0; i<8; i+=2){
									if(p1.position[1] == p1.possibleMoves[i+1] && (p1.position[0]-1 == p1.possibleMoves[i] || p1.position[0]-2 == p1.possibleMoves[i])){
                    movePedina1(2);
                    right = 0;
                    up = 0;
									  down = 0;
									}
								}
                }
            } else if (p1.wall_mode) {
                moveWall(2,&w);
            }
					}
						else if (p1.turnOn == 1 && m.Player == 2 && p1.extendedMode == 1 && (p1.extOrien == 0)) {
							if (left >= 1) {
									left = 1;
                    movePedina1(12);			
                    down = 0;
                    right = 0;
                    up = 0;
									}
                }
        

        if (p2.turnOn == 1 && p2.extendedMode == 0) {
            if (p2.wall_mode == 0) {
                if (left >= 1) {
									left = 1;
									for(i=0; i<8; i+=2){
									if(p2.position[1]  == p2.possibleMoves[i+1] && (p2.position[0]-1 == p2.possibleMoves[i] || p2.position[0]-2 == p2.possibleMoves[i])){
                    movePedina2(2);
                    right = 0;
                    up = 0;
										down = 0;
									}
								}
                }
            } else if (p2.wall_mode) {
                moveWall(2,&w);
            }
					}
						else if (p2.turnOn == 1 && p2.extendedMode == 1 && (p2.extOrien == 0)) {
							if (left >= 1) {
									left = 1;
                    movePedina2(12);			
                    down = 0;
                    right = 0;
                    up = 0;
									}
                }
        }

    if ((LPC_GPIO1->FIOPIN & (1<<28)) == 0) {
        right++;
        if (p1.turnOn == 1 && m.Player == 2 && p1.extendedMode == 0) {
            if (p1.wall_mode == 0) {
                if (right >= 1) {
									right = 1;
									for(i=0; i<8; i+=2){
									if(p1.position[1] == p1.possibleMoves[i+1] && (p1.position[0] +1== p1.possibleMoves[i] || p1.position[0] +2== p1.possibleMoves[i])){
                    movePedina1(3);
                    left = 0;
                    up = 0;
										down = 0;
									}
								}
                }
            } else if (p1.wall_mode) {
                moveWall(3,&w);
            }
					}
						else if (p1.turnOn == 1 && m.Player == 2&& p1.extendedMode == 1 && (p1.extOrien == 0)) {
							if (right >= 1) {
									right = 1;
                    movePedina1(11);			
                    down = 0;
                    left = 0;
                    up = 0;
									}
                }

        if (p2.turnOn == 1 && p2.extendedMode == 0) {
            if (p2.wall_mode == 0) {
                if (right >= 1) {
									right = 1;
									for(i=0; i<8; i+=2){
									if(p2.position[1] == p2.possibleMoves[i+1] && (p2.position[0] +1== p2.possibleMoves[i] || p2.position[0] +2== p2.possibleMoves[i])){
                    movePedina2(3);
                    left = 0;
                    up = 0;
										down = 0;
									}
								}
                }
            } else if (p2.wall_mode) {
                moveWall(3,&w);
            }
					}
						else if (p2.turnOn == 1 && p2.extendedMode == 1 && (p2.extOrien == 0)) {
							if (right >= 1) {
									right = 1;
                    movePedina2(11);			
                    down = 0;
                    left = 0;
                    up = 0;
									}
                }
        }
		if ((LPC_GPIO1->FIOPIN & (1<<25)) == 0) {								//pulsante select, scambio turno e confermo muro o mossa
			if(m.mode == 1 && m.Board == 0 && m.Player == 0){
					if(up != 0 && down == 0){
						m.Board = 1;
					
						choose_single_mode();
				}
					else if ( down != 0 && up == 0){
						m.Board = 2;
						
						choose_multiplayer_mode();
					}
			}
			else
			if(m.mode == 1 && (m.Board != 0) && m.Player == 0){
					if(up != 0 && down == 0 && m.Board == 1){
						m.Player = 2;
						m.mode = 0;
						start_single_human();
				}
					else if ( down != 0 && up == 0 && m.Board == 1){
						m.Player = 1;
						m.mode = 0;
						start_single_NPC();
					}
			}	
			else {
			if(p1.wall_mode == 1 || p2.wall_mode == 1){
				if(confirmWall(&w)==0){
					
				swapTurn(&p1,&p2);
				}
			}
			else if(p1.wall_mode == 0 && p2.wall_mode == 0){ 
			if(up == 1){
				if(p2.turnOn == 1 && p2.extendedMode == 0 ){confirmMove(0,&p2);}else if(p1.turnOn == 1 && p1.extendedMode == 0 ){confirmMove(0,&p1);}
				else if(p2.turnOn == 1 && p2.extendedMode == 1){confirmMove(9,&p2);}else if(p1.turnOn == 1 && p1.extendedMode == 1 ){confirmMove(9,&p1);}
				swapTurn(&p1,&p2);
			}
			if(down == 1){
				if(p2.turnOn == 1 && p2.extendedMode == 0 ){confirmMove(1,&p2);}else if(p1.turnOn == 1 && p1.extendedMode == 0 ){confirmMove(1,&p1);}
				else if(p2.turnOn == 1 && p2.extendedMode == 1){confirmMove(10,&p2);}else if(p1.turnOn == 1 && p1.extendedMode == 1 ){confirmMove(10,&p1);}
				swapTurn(&p1,&p2);
			}
			if(left == 1){
				if(p2.turnOn == 1 && p2.extendedMode == 0 ){confirmMove(2,&p2);}else if(p1.turnOn == 1 && p1.extendedMode == 0 ){confirmMove(2,&p1);}
				else if(p2.turnOn == 1 && p2.extendedMode == 1){confirmMove(12,&p2);}else if(p1.turnOn == 1 && p1.extendedMode == 1 ){confirmMove(12,&p1);}
				swapTurn(&p1,&p2);
			}
			if(right == 1){
				if(p2.turnOn == 1 && p2.extendedMode == 0 ){confirmMove(3,&p2);}else if(p1.turnOn == 1 && p1.extendedMode == 0 ){confirmMove(3,&p1);}
				else if(p2.turnOn == 1 && p2.extendedMode == 1){confirmMove(11,&p2);}else if(p1.turnOn == 1 && p1.extendedMode == 1 ){confirmMove(11,&p1);}
				swapTurn(&p1,&p2);
			}
		}
	}
	}
		


		
		if(but_1!=0){											//gestione bottoni per wall mode
			but_1++;
						
			if(p1.turnOn == 1 && but_1 == 2){
		WallMode(&p1);
	}
	else if(p2.turnOn == 1 && but_1 == 2){
		WallMode(&p2);
	}
	else {	/* button released */
		but_1=0;			
		NVIC_EnableIRQ(EINT1_IRQn); 							 /* enable Button interrupts			*/
		LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 1 pin selection */
	}
}
		
		if(but_2!=0){										//gestione bottone per ruotare wall, sempre in base a orientation e posizione,
			but_2++;
			if(p1.wall_mode == 1 && but_2 == 2 && p1.turnOn == 1){
			if(w.orientation == 1){
			LCD_RemoveWall(w.position[0],w.position[1],w.orientation);
			w.orientation = 0;
			LCD_DisplayWall(w.position[0],w.position[1],w.orientation,Red);
			}
			else {
			LCD_RemoveWall(w.position[0],w.position[1],w.orientation);	
			w.orientation = 1;
			LCD_DisplayWall(w.position[0],w.position[1],w.orientation,Red);	
			}
	}
			else if(but_2 == 2 && p1.turnOn == 1 && (p1.extendedMoves[0] !=255 || p1.extendedMoves[2] != 255) && p1.wall_mode == 0 && p1.extendedMode == 0){
				p1.extendedMode = 1;
				calculateP2Moves(p2,&p1);
			}
			else if(but_2 == 2 && p1.turnOn == 1 && (p1.extendedMoves[0] !=255 || p1.extendedMoves[2] != 255) && p1.wall_mode == 0 && p1.extendedMode == 1){
				p1.extendedMode = 0;
				LCD_Colora(1+30*p1.extendedMoves[0]+5*p1.extendedMoves[0], 1+30*p1.extendedMoves[1]+5*p1.extendedMoves[1], 30*(p1.extendedMoves[0]+1)+5*p1.extendedMoves[0]-1, 30*(p1.extendedMoves[1]+1)+5*p1.extendedMoves[1]-1, White);
				LCD_Colora(1+30*p1.extendedMoves[2]+5*p1.extendedMoves[2], 1+30*p1.extendedMoves[3]+5*p1.extendedMoves[3], 30*(p1.extendedMoves[2]+1)+5*p1.extendedMoves[2]-1, 30*(p1.extendedMoves[3]+1)+5*p1.extendedMoves[3]-1, White);
				calculateP2Moves(p2,&p1);
			}
	else if(p2.wall_mode == 1 && but_2 == 2 && p2.turnOn == 1){
		if(w.orientation == 1){
		LCD_RemoveWall(w.position[0],w.position[1],w.orientation);		
		w.orientation = 0;
		LCD_DisplayWall(w.position[0],w.position[1],w.orientation,Red);	
		}else {
		LCD_RemoveWall(w.position[0],w.position[1],w.orientation);
		w.orientation = 1;
		LCD_DisplayWall(w.position[0],w.position[1],w.orientation,Red);
		}
	}
	else if(but_2 == 2 && p2.turnOn == 1 && (p2.extendedMoves[0] !=255 || p2.extendedMoves[2] != 255) && p2.wall_mode == 0 && p2.extendedMode == 0){
				p2.extendedMode = 1;
				calculateP2Moves(p1,&p2);
			}
	else if(but_2 == 2 && p2.turnOn == 1 && (p2.extendedMoves[0] !=255 || p2.extendedMoves[2] != 255) && p2.wall_mode == 0 && p2.extendedMode == 1){
				p2.extendedMode = 0;
				LCD_Colora(1+30*p2.extendedMoves[0]+5*p2.extendedMoves[0], 1+30*p2.extendedMoves[1]+5*p2.extendedMoves[1], 30*(p2.extendedMoves[0]+1)+5*p2.extendedMoves[0]-1, 30*(p2.extendedMoves[1]+1)+5*p2.extendedMoves[1]-1, White);
				LCD_Colora(1+30*p2.extendedMoves[2]+5*p2.extendedMoves[2], 1+30*p2.extendedMoves[3]+5*p2.extendedMoves[3], 30*(p2.extendedMoves[2]+1)+5*p2.extendedMoves[2]-1, 30*(p2.extendedMoves[3]+1)+5*p2.extendedMoves[3]-1, White);
				calculateP2Moves(p1,&p2);
			}
	else {	
		but_2=0;			
		NVIC_EnableIRQ(EINT2_IRQn);
		LPC_PINCON->PINSEL4    |= (1 << 24);     
	}
}

    enable_RIT();
    LPC_RIT->RICTRL |= 0x1;

    return;
}


/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

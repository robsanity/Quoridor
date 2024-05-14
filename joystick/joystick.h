																			 /*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        Prototypes of functions included in the lib_joystick, funct_joystick .c files
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/
#include "../game/game.h"
/* lib_joystick */
void joystick_init(void);
void movePedina1(char choice);
void movePedina2(char choice);
void confirmMove(char direction,struct Pedina *p);
void moveWall(char direction,struct Walls *w);
int confirmWall(struct Walls *w);
void printAllWalls(struct Walls *w);
int confirmBotWall(struct Walls *w);




/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           funct_joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        High level joystick management functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "joystick.h"
#include "../GLCD/GLCD.h"
#include "../game/game.h"
extern Pedina p1;
extern Pedina p2;
extern int up;



void movePedina1(char direction) {																				//in base alla direzione, ci sono i vari casi
	if(direction==0 || direction==1 || direction==2 || direction==3 || direction == 9 || direction ==10||direction == 11 || direction == 12){
		LCD_DisplayMove(direction,p1);
	}
}

void movePedina2(char direction) {
	if(direction==0 || direction==1 || direction==2 || direction==3 || direction == 9 || direction ==10||direction == 11 || direction == 12){
		LCD_DisplayMove(direction,p2);
	}
}

void printAllWalls(struct Walls *w){													//funzione che prende dall'array allwalls presente nella struct walls tutti i muri creati e li printa sulla board
	int i = 0;																									// ci si muove con indici in quanto è stato fatto un array che ha in posizione 1 la x del muro, posizione 2 la y e pos 3 l'orientation
	for (i = 0; i < w->counter; i += 6){												// ci si muove di 6 in questo caso, perchè la print del muro aggiunge l'altro "pezzetto" da sola in base a orientation
		LCD_DisplayWall(w->allWalls[i],w->allWalls[i+1],w->allWalls[i+2],Green);  //(implementato in displaywalls)in quanto in allwalls ogni muro è lungo una cella e sono salvati contigui i 2 pezzetti di un muro
	}
}

void moveWall(char direction,struct Walls *w){					//funzione per spostare i muri in fase di selezione della posizione del muro, usato meccanismo simile a pedina per le direction 
	if(direction == 0 && w->position[1] >=1){	
		LCD_RemoveWall(w->position[0],w->position[1],w->orientation);	
		w->position[1]--;
		}
		if(direction == 1&& w->position[1] <5){
		LCD_RemoveWall(w->position[0],w->position[1],w->orientation);	
		w->position[1]++;
		}
	 	if(direction == 2&& w->position[0] >=1){
		LCD_RemoveWall(w->position[0],w->position[1],w->orientation);	
		w->position[0]--;
		}
		if(direction == 3&& w->position[0] <5){
		LCD_RemoveWall(w->position[0],w->position[1],w->orientation);	
		w->position[0]++;
		}
		printAllWalls(w);
		LCD_DisplayWall(w->position[0],w->position[1],w->orientation,Red);
	}

int confirmWall(struct Walls *w){			//funzione per confermare la posizione di un muro(dopo aver premuto select).ci sono 2 check per vedere se crea una trappola(per entrambe le pedine)
	int check1;													// e un flag validity, che controlla se i muri si overlappano, anche a croce, utilizzando delle relazioni geometriche tra 
																			// le coordinate cartesiane dei muri. Inoltre avendo salvato i muri sulla board 7x7 quelli orientati in orizzontale avranno
	int i = 0;													// 0<=x <= 6 e 1<=y<=6, per quelli verticali al contrario. E' molto ricorrente per tutto ciò che riguarda i muri questa 	
	int validity = 0;										// relazione tra le coordinate e la scacchiera
	for (i = 0; i < w->counter; i += 3) {
	if (w->orientation == 1 && w->allWalls[i + 2] == 1 && (w->position[0] == w->allWalls[i] || w->position[0]+1 == w->allWalls[i]) && w->position[1] == w->allWalls[i + 1]) {
            GUI_Text(20, 250, "Invalid Wall. REPLACE", Red, BACKGROUND);
						validity = 1;
        }
	if (w->orientation == 0 && w->allWalls[i + 2] == 0 && (w->position[1] == w->allWalls[i+1] || w->position[1]+1 == w->allWalls[i+1]) && w->position[0] == w->allWalls[i]) {
            GUI_Text(20, 250, "Invalid Wall. REPLACE", Red, BACKGROUND);
						validity = 1;
        }
			}
	for (i = 0; i < w->counter; i += 6) {
	if (w->orientation == 0 && w->allWalls[i + 2] == 1 && 
		(w->position[0] + w->position[1]- w->allWalls[i+3] - w->allWalls[i+4] +1 == 0)&&
   		w->position[0] == w->allWalls[i] && w->position[1] == w->allWalls[i + 1]) {
            GUI_Text(20, 250, "Invalid Wall. REPLACE", Red, BACKGROUND);
						validity = 1;
        }
			}
	for (i = 0; i < w->counter; i += 6) {
	if (w->orientation == 1 && w->allWalls[i + 2] == 0 && 
		(w->position[0] + w->position[1]- w->allWalls[i+3] - w->allWalls[i+4] +1 == 0)&&
   		w->position[0] == w->allWalls[i] && w->position[1] == w->allWalls[i + 1]) {
            GUI_Text(20, 250, "Invalid Wall. REPLACE", Red, BACKGROUND);
						validity = 1;
        }
			}
	LCD_RemoveWall(w->position[0],w->position[1],w->orientation);    //rimuovo il muro simulato e controllo la validità
  if (validity == 0){																							
	if(w->orientation == 1){
	w->allWalls[w->counter] = w->position[0];
	w->counter++;	
	w->allWalls[w->counter] = w->position[1];
	w->counter++;	
	w->allWalls[w->counter] = w->orientation;	
	w->counter++;
	w->allWalls[w->counter] = w->position[0]+1;
	w->counter++;	
	w->allWalls[w->counter] = w->position[1];
	w->counter++;	
	w->allWalls[w->counter] = w->orientation;	
	w->counter++;
	}
	else {
	w->allWalls[w->counter] = w->position[0];
	w->counter++;	
	w->allWalls[w->counter] = w->position[1];
	w->counter++;	
	w->allWalls[w->counter] = w->orientation;	
	w->counter++;
	w->allWalls[w->counter] = w->position[0];
	w->counter++;	
	w->allWalls[w->counter] = w->position[1]+1;
	w->counter++;	
	w->allWalls[w->counter] = w->orientation;	
	w->counter++;
	}																			//dopo aver controllato il muro rispetto agli altri lko aggiungo momentaneamente al vettore di tutti i muri per controllare	
	populateSchema();
	if(check() != 1 || check2() != 1){																	//se il check non va a buon fine vado a displayare errore e rimuovo dall'array il muro inserito, togliendo il muro displayato
		clearInvalidWall();																						//check1 è ancora simulato e non è verde
		GUI_Text(20,250,"Invalid Wall. REPLACE",Red,BACKGROUND);
		w->counter--;
		w->allWalls[w->counter]=0;
		w->counter--;
		w->allWalls[w->counter]=0;
		w->counter--;
		w->allWalls[w->counter]=0;
		w->counter--;
		w->allWalls[w->counter]=0;
		w->counter--;
		w->allWalls[w->counter]=0;
		w->counter--;
		w->allWalls[w->counter]=0;
		w->position[0]=3;w->position[1]=3;w->orientation=1; 
		LCD_DisplayWall(w->position[0],w->position[1],w->orientation,Red);
		validity = 1;
		printAllWalls(w);																													//e vado a riprintare tutti i muri
		GUI_Text(20, 250, "                     ", White, BACKGROUND);
		return validity;
	}
	else{
	if(p1.turnOn == 1){																//se il check va a buon fine vado a vedere chi ha messo il muro che rimane nell'array e salvo nell'unsigned int la mossa
			p1.wall_remain--;
			LCD_DisplayWalls1(p1.wall_remain);
			if(w->orientation ==1){
			saveLastMove(0, 1 ,w->allWalls[w->counter-1],w->allWalls[w->counter-2],w->allWalls[w->counter-3] +1);
			}
			else{
				saveLastMove(0, 1 ,w->allWalls[w->counter-1],w->allWalls[w->counter-2]+1,w->allWalls[w->counter-3]);
			}
		}
		else {
			p2.wall_remain--;
			p2.lastWall = 1;
			LCD_DisplayWalls2(p2.wall_remain);	
			if(w->allWalls[w->counter-1] == 1){
			saveLastMove(0, 1 ,w->allWalls[w->counter-1],w->allWalls[w->counter-2]+1,w->allWalls[w->counter-3]);
			}
			else{
				saveLastMove(0, 1 ,w->allWalls[w->counter-1],w->allWalls[w->counter-2],w->allWalls[w->counter-3]+1);
			}
		}
	}
}
	else{
		w->position[0]=3;w->position[1]=3;w->orientation=1; 				//quando un muro è invalido poi riposiziono il muro simulato al centro per permettere al giocatore di cambiarne la posizione	
		LCD_DisplayWall(w->position[0],w->position[1],w->orientation,Red);		// e averne una valida
	}
	GUI_Text(20, 250, "                     ", White, BACKGROUND);
 	printAllWalls(w);
	return validity;
}



int confirmBotWall(struct Walls *w){			//funzione per confermare la posizione di un muro(dopo aver premuto select).ci sono 2 check per vedere se crea una trappola(per entrambe le pedine)
	int check1;													// e un flag validity, che controlla se i muri si overlappano, anche a croce, utilizzando delle relazioni geometriche tra 
																			// le coordinate cartesiane dei muri. Inoltre avendo salvato i muri sulla board 7x7 quelli orientati in orizzontale avranno
	int i = 0;													// 0<=x <= 6 e 1<=y<=6, per quelli verticali al contrario. E' molto ricorrente per tutto ciò che riguarda i muri questa 	
	int validity = 0;										// relazione tra le coordinate e la scacchiera
	for (i = 0; i < w->counter; i += 3) {
	if (w->orientation == 1 && w->allWalls[i + 2] == 1 && (w->position[0] == w->allWalls[i] || w->position[0]+1 == w->allWalls[i]) && w->position[1] == w->allWalls[i + 1]) {
           validity = 1;
        }
	if (w->orientation == 0 && w->allWalls[i + 2] == 0 && (w->position[1] == w->allWalls[i+1] || w->position[1]+1 == w->allWalls[i+1]) && w->position[0] == w->allWalls[i]) {
           validity = 1;
        }
			}
	for (i = 0; i < w->counter; i += 6) {
	if (w->orientation == 0 && w->allWalls[i + 2] == 1 && 
		(w->position[0] + w->position[1]- w->allWalls[i+3] - w->allWalls[i+4] +1 == 0)&&
   		w->position[0] == w->allWalls[i] && w->position[1] == w->allWalls[i + 1]) {
           validity = 1;
        }
			}
	for (i = 0; i < w->counter; i += 6) {
	if (w->orientation == 1 && w->allWalls[i + 2] == 0 && 
		(w->position[0] + w->position[1]- w->allWalls[i+3] - w->allWalls[i+4] +1 == 0)&&
   		w->position[0] == w->allWalls[i] && w->position[1] == w->allWalls[i + 1]) {
          	validity = 1;
        }
			}
	if (validity == 0){																							
	if(w->orientation == 1){
	w->allWalls[w->counter] = w->position[0];
	w->counter++;	
	w->allWalls[w->counter] = w->position[1];
	w->counter++;	
	w->allWalls[w->counter] = w->orientation;	
	w->counter++;
	w->allWalls[w->counter] = w->position[0]+1;
	w->counter++;	
	w->allWalls[w->counter] = w->position[1];
	w->counter++;	
	w->allWalls[w->counter] = w->orientation;	
	w->counter++;
	}
	else {
	w->allWalls[w->counter] = w->position[0];
	w->counter++;	
	w->allWalls[w->counter] = w->position[1];
	w->counter++;	
	w->allWalls[w->counter] = w->orientation;	
	w->counter++;
	w->allWalls[w->counter] = w->position[0];
	w->counter++;	
	w->allWalls[w->counter] = w->position[1]+1;
	w->counter++;	
	w->allWalls[w->counter] = w->orientation;	
	w->counter++;
	}																			//dopo aver controllato il muro rispetto agli altri lko aggiungo momentaneamente al vettore di tutti i muri per controllare	
	populateSchema();
	if(check() != 1 || check2() != 1){																	//se il check non va a buon fine vado a displayare errore e rimuovo dall'array il muro inserito, togliendo il muro displayato
		clearInvalidWall();																						//check1 è ancora simulato e non è verde
		w->counter--;
		w->allWalls[w->counter]=0;
		w->counter--;
		w->allWalls[w->counter]=0;
		w->counter--;
		w->allWalls[w->counter]=0;
		w->counter--;
		w->allWalls[w->counter]=0;
		w->counter--;
		w->allWalls[w->counter]=0;
		w->counter--;
		w->allWalls[w->counter]=0;
		w->position[0]=3;w->position[1]=3;w->orientation=1; 
		validity = 1;
		return validity;
	}
	else{
	if(p1.turnOn == 1){																//se il check va a buon fine vado a vedere chi ha messo il muro che rimane nell'array e salvo nell'unsigned int la mossa
			p1.wall_remain--;
			LCD_DisplayWalls1(p1.wall_remain);
			if(w->orientation ==1){
			saveLastMove(0, 1 ,w->allWalls[w->counter-1],w->allWalls[w->counter-2],w->allWalls[w->counter-3] +1);
			}
			else{
				saveLastMove(0, 1 ,w->allWalls[w->counter-1],w->allWalls[w->counter-2]+1,w->allWalls[w->counter-3]);
			}
		}
		else {
			p2.wall_remain--;
			LCD_DisplayWalls2(p2.wall_remain);	
			if(w->allWalls[w->counter-1] == 1){
			saveLastMove(0, 1 ,w->allWalls[w->counter-1],w->allWalls[w->counter-2]+1,w->allWalls[w->counter-3]);
			}
			else{
				saveLastMove(0, 1 ,w->allWalls[w->counter-1],w->allWalls[w->counter-2],w->allWalls[w->counter-3]+1);
			}
		}
	}
}
	else{
		w->position[0]=3;w->position[1]=3;w->orientation=1; 				//quando un muro è invalido poi riposiziono il muro simulato al centro per permettere al giocatore di cambiarne la posizione	
	}
	printAllWalls(w);
	
	return validity;
}


void confirmMove(char direction,struct Pedina *p){        //funzione per la conferma della mossa
	int su[2];
	int giu[2];
	int sx[2];
	int dx[2];
	int ex1[2];
	int ex2[2];
	su[0] = p->possibleMoves[6];
	su[1] = p->possibleMoves[7];
	giu[0]= p->possibleMoves[4];
	giu[1]= p->possibleMoves[5];
	sx[0] = p->possibleMoves[2];
	sx[1] = p->possibleMoves[3];
	dx[0] = p->possibleMoves[0];
	dx[1] = p->possibleMoves[1];
	ex1[0]= p->extendedMoves[0];
	ex1[1]= p->extendedMoves[1];
	ex2[0]= p->extendedMoves[2];
	ex2[1]= p->extendedMoves[3];
	LCD_DisplayMove(4,*p);																//togli i quadrati delle possible moves
	if (direction == 0 && (su[0] != 255 || su[1] != 255)){  //controlla che le possible moves non siano invalide e conferma la direzione
		if(p->name == 1){LCD_Colora(1+30*su[0]+5*su[0], 1+30*su[1]+5*su[1], 30*(su[0]+1)+5*su[0]-1, 30*(su[1]+1)+5*su[1]-1, Red);}
			else{LCD_Colora(1+30*su[0]+5*su[0], 1+30*su[1]+5*su[1], 30*(su[0]+1)+5*su[0]-1, 30*(su[1]+1)+5*su[1]-1, Blue);}
		p->position[0] = su[0];
		p->position[1] = su[1];
	}else if(direction == 1 && (giu[0] != 255 || giu[1] != 255)){
		if(p->name == 1){LCD_Colora(1+30*giu[0]+5*giu[0], 1+30*giu[1]+5*giu[1], 30*(giu[0]+1)+5*giu[0]-1, 30*(giu[1]+1)+5*giu[1]-1, Red);}
			else{LCD_Colora(1+30*giu[0]+5*giu[0], 1+30*giu[1]+5*giu[1], 30*(giu[0]+1)+5*giu[0]-1, 30*(giu[1]+1)+5*giu[1]-1, Blue);}
		p->position[0] = giu[0];
		p->position[1] = giu[1];
	}
	else if(direction == 2 && (sx[0] != 255 || sx[1] != 255)){
		if(p->name == 1){LCD_Colora(1+30*sx[0]+5*sx[0], 1+30*sx[1]+5*sx[1], 30*(sx[0]+1)+5*sx[0]-1, 30*(sx[1]+1)+5*sx[1]-1, Red);}
			else {LCD_Colora(1+30*sx[0]+5*sx[0], 1+30*sx[1]+5*sx[1], 30*(sx[0]+1)+5*sx[0]-1, 30*(sx[1]+1)+5*sx[1]-1, Blue);}
		p->position[0] = sx[0];
		p->position[1] = sx[1];
	}
	else if(direction == 3 && (dx[0] != 255 || dx[1] != 255)){
		if(p->name == 1){LCD_Colora(1+30*dx[0]+5*dx[0], 1+30*dx[1]+5*dx[1], 30*(dx[0]+1)+5*dx[0]-1, 30*(dx[1]+1)+5*dx[1]-1, Red);}
			else{LCD_Colora(1+30*dx[0]+5*dx[0], 1+30*dx[1]+5*dx[1], 30*(dx[0]+1)+5*dx[0]-1, 30*(dx[1]+1)+5*dx[1]-1, Blue);}
		p->position[0] = dx[0];
		p->position[1] = dx[1];
	}
	else if(direction == 9 && (ex2[0] != 255 || ex2[1] != 255)){
		if(p->name == 1){LCD_Colora(1+30*ex2[0]+5*ex2[0], 1+30*ex2[1]+5*ex2[1], 30*(ex2[0]+1)+5*ex2[0]-1, 30*(ex2[1]+1)+5*ex2[1]-1, Red);}
			else{LCD_Colora(1+30*ex2[0]+5*ex2[0], 1+30*ex2[1]+5*ex2[1], 30*(ex2[0]+1)+5*ex2[0]-1, 30*(ex2[1]+1)+5*ex2[1]-1, Blue);}
		p->position[0] = ex2[0];
		p->position[1] = ex2[1];
	}
	else if(direction == 10 && (ex1[0] != 255 || ex1[1] != 255)){
		if(p->name == 1){LCD_Colora(1+30*ex1[0]+5*ex1[0], 1+30*ex1[1]+5*ex1[1], 30*(ex1[0]+1)+5*ex1[0]-1, 30*(ex1[1]+1)+5*ex1[1]-1, Red);}
			else{LCD_Colora(1+30*ex1[0]+5*ex1[0], 1+30*ex1[1]+5*ex1[1], 30*(ex1[0]+1)+5*ex1[0]-1, 30*(ex1[1]+1)+5*ex1[1]-1, Blue);}
		p->position[0] = ex1[0];
		p->position[1] = ex1[1];
	}
	else if(direction == 11 && (ex2[0] != 255 || ex2[1] != 255)){
		if(p->name == 1){LCD_Colora(1+30*ex2[0]+5*ex2[0], 1+30*ex2[1]+5*ex2[1], 30*(ex2[0]+1)+5*ex2[0]-1, 30*(ex2[1]+1)+5*ex2[1]-1, Red);}
			else{LCD_Colora(1+30*ex2[0]+5*ex2[0], 1+30*ex2[1]+5*ex2[1], 30*(ex2[0]+1)+5*ex2[0]-1, 30*(ex2[1]+1)+5*ex2[1]-1, Blue);}
		p->position[0] = ex2[0];
		p->position[1] = ex2[1];
	}
	else if(direction == 12 && (ex1[0] != 255 || ex1[1] != 255)){
		if(p->name == 1){LCD_Colora(1+30*ex1[0]+5*ex1[0], 1+30*ex1[1]+5*ex1[1], 30*(ex1[0]+1)+5*ex1[0]-1, 30*(ex1[1]+1)+5*ex1[1]-1, Red);}
			else{LCD_Colora(1+30*ex1[0]+5*ex1[0], 1+30*ex1[1]+5*ex1[1], 30*(ex1[0]+1)+5*ex1[0]-1, 30*(ex1[1]+1)+5*ex1[1]-1, Blue);}
		p->position[0] = ex1[0];
		p->position[1] = ex1[1];
	}
	else{LCD_DisplayMove(6,*p);} //fatto per handlare se non sono coperti tutti i casi e si sceglie una mossa invalida, viene ristampata la pedina
	saveLastMove(p->name == 1 ? 0 : 1, 0 ,0, p->position[1], p->position[0]);
	if(p->name == 2){p2.lastWall = 0;};
	
}



	
/******************************************************************************
**                            End Of File
******************************************************************************/

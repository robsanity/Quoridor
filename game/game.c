#include <stdio.h>
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"
#include "../RIT/RIT.h"
#include "game.h"
#include "../Joystick/joystick.h"


Pedina p1;
Pedina p2;
Walls w;
uint32_t last_move;
Match m;

#define ROWS 13
#define COLS 13
#define IS_PLACING_BARRIER_MASK 0x10
static int c = 0;
static uint32_t last_move;

static uint8_t Traps[ROWS][COLS] = {																				//matrice 13x13 per la dfs
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

int visited[ROWS][COLS];			//visited matrice per dfs
int visited2[ROWS][COLS];
void choose_mode(){
	m.mode = 1;
	LCD_Clear(Black);
	GUI_Text(80, 50, (uint8_t *)"Select the", Red, Black);
	GUI_Text(80, 75, (uint8_t *)"GAME MODE", Red, Black);
	LCD_DrawSquare(60, 120, 180, 180 , Red);
	LCD_DrawSquare(60, 200, 180, 260 , Red);
	GUI_Text(75, 140, (uint8_t *)"Single Board", Red, Black);
	GUI_Text(75, 220, (uint8_t *)"Two Boards", Red, Black);
}


 void choose_single_mode(){
	LCD_Clear(Black);
	GUI_Text(58, 50, (uint8_t *)"Single Board: select", Red, Black);
	GUI_Text(58, 75, (uint8_t *)"the opposite player", Red, Black);
	LCD_DrawSquare(60, 120, 180, 180 , Red);
	LCD_DrawSquare(60, 200, 180, 260 , Red);
	GUI_Text(75, 140, (uint8_t *)"Human", Red, Black);
	GUI_Text(75, 220, (uint8_t *)"NPC", Red, Black);
}

void choose_multiplayer_mode(){
	LCD_Clear(Black);
	GUI_Text(58, 50, (uint8_t *)"Two Boards: select", Red, Black);
	GUI_Text(58, 75, (uint8_t *)"the opposite player", Red, Black);
	LCD_DrawSquare(60, 120, 180, 180 , Red);
	LCD_DrawSquare(60, 200, 180, 260 , Red);
	GUI_Text(75, 140, (uint8_t *)"Human", Red, Black);
	GUI_Text(75, 220, (uint8_t *)"NPC", Red, Black);
}




void startscreen(){				//semplcie start screen
	 LCD_Clear(Black);
	 GUI_Text(10, 10, (uint8_t *)"Benvenuti in Quoridor!", Red, Black);
   GUI_Text(10, 35, (uint8_t *)"Vince il primo che arriva al", Red, Black);
	 GUI_Text(10, 60, (uint8_t *)"lato opposto, e' possibile", Red, Black);
   GUI_Text(10, 85, (uint8_t *)"piazzare 8 muri a testa che", Red, Black);
   GUI_Text(10, 110, (uint8_t *)"bloccheranno i movimenti di", Red, Black);
	 GUI_Text(10, 135, (uint8_t *)"entrambi. Non e' consentito", Red, Black);
	 GUI_Text(10, 160, (uint8_t *)"disporre muri a croce o", Red, Black);
   GUI_Text(10, 185, (uint8_t *)"intrappolare un giocatore.", Red, Black);
	 GUI_Text(10, 210, (uint8_t *)"Tempo per una mossa: 20 sec", Red, Black);
	 GUI_Text(10, 235, (uint8_t *)"Clicca INT0 per iniziare", Red, Black);
   GUI_Text(10, 260, (uint8_t *)"e che vinca il migliore!", Red, Black);
}
void start_single_human(){						//funzione di start dove vengono inizializzati board e campi delle strutture pedine e wall per inizio game
	LCD_Clear(White);
	enable_timer(0);
	w.counter=0;
	p1.win=0;
	p2.win=0;
	LCD_DrawBoard(Black);
	p1.botmode = 0;
	p1.name = 1;
	p2.name = 2;
	p1.turnOn = 0;
	p2.turnOn = 1;
	p2.wall_mode = 0;
	p1.position[0]=3;
	p1.position[1]=0;
	p1.wall_remain = 8;
	LCD_Pedina1(p1.position[0],p1.position[1]);	
	p2.position[0]=3;
	p2.position[1]=6;
	LCD_Pedina2(p2.position[0],p2.position[1]);	
	p2.wall_remain = 8;
	LCD_DisplayWalls1(p1.wall_remain);
	LCD_DisplayWalls2(p2.wall_remain);
	p1.time_left = 20;
	p2.time_left = 20;
	calculateP2Moves(p1,&p2);
}

void start_single_NPC(){						//funzione di start dove vengono inizializzati board e campi delle strutture pedine e wall per inizio game
	LCD_Clear(White);
	enable_timer(0);
	w.counter=0;
	p1.botmode = 1;
	p1.win=0;
	p2.win=0;
	LCD_DrawBoard(Black);
	p1.name = 1;
	p2.name = 2;
	p1.turnOn = 0;
	p2.turnOn = 1;
	p2.wall_mode = 0;
	p1.position[0]=3;
	p1.position[1]=0;
	p1.wall_remain = 8;
	LCD_Pedina1(p1.position[0],p1.position[1]);	
	p2.position[0]=3;
	p2.position[1]=6;
	LCD_Pedina2(p2.position[0],p2.position[1]);	
	p2.wall_remain = 8;
	LCD_DisplayWalls1(p1.wall_remain);
	LCD_DisplayWalls2(p2.wall_remain);
	p1.time_left = 20;
	p2.time_left = 20;
	p1.counter = 0;
	p2.lastWall = 0;
	calculateP2Moves(p1,&p2);
	resetVisited();
	resetVisited2();
	populateSchema();
	dfs2bot(Traps, visited2,  p1.position[1]*2, p1.position[0]*2,&p1);
}


void NPCmove(){																				//logica NPC, prima si vede la posizione dell'opponent rispetto al bot e si decide se entrare in wallMode e piazzare muri
	char x;																							// se ci sono muri disponibili allora si piazzano muri cercando di bloccare le mosse prima in avanti(n+2) e poi di lato(n+1)
	char y;																						// del player, se no ci si muove seguendo la dfs. Se l'opponent ha creato dei muri viene ricalcolato un nuovo percorso per il bot
	char z;																							// il percorso è salvato in un array con il corrispettivo joyup, down, left e right in base alla dfs, ma prima di committare
	int h;																							// rifaccio una validation della mossa nelle possible moves, che sarà sicuramente giusta per fare un double check.
	int i = 0;
	h = 1;
	if(p1.extendedMoves[0] != 255 && p1.extendedMoves[1] >= p1.position[1]){
	p1.extendedMode = 1;
	calculateP2Moves(p2,&p1);
	if(p1.orientation == 1){
		confirmMove(12,&p1);
		swapTurn(&p1,&p2);
		LCD_Colora(1+30*p2.extendedMoves[0]+5*p2.extendedMoves[0], 1+30*p2.extendedMoves[1]+5*p2.extendedMoves[1], 30*(p2.extendedMoves[0]+1)+5*p2.extendedMoves[0]-1, 30*(p2.extendedMoves[1]+1)+5*p2.extendedMoves[1]-1, White);
		LCD_Colora(1+30*p2.extendedMoves[2]+5*p2.extendedMoves[2], 1+30*p2.extendedMoves[3]+5*p2.extendedMoves[3], 30*(p2.extendedMoves[2]+1)+5*p2.extendedMoves[2]-1, 30*(p2.extendedMoves[3]+1)+5*p2.extendedMoves[3]-1, White);

		return;
	}
	else if(p1.orientation == 0){
		confirmMove(10,&p1);
		swapTurn(&p1,&p2);
		LCD_Colora(1+30*p2.extendedMoves[0]+5*p2.extendedMoves[0], 1+30*p2.extendedMoves[1]+5*p2.extendedMoves[1], 30*(p2.extendedMoves[0]+1)+5*p2.extendedMoves[0]-1, 30*(p2.extendedMoves[1]+1)+5*p2.extendedMoves[1]-1, White);
		LCD_Colora(1+30*p2.extendedMoves[2]+5*p2.extendedMoves[2], 1+30*p2.extendedMoves[3]+5*p2.extendedMoves[3], 30*(p2.extendedMoves[2]+1)+5*p2.extendedMoves[2]-1, 30*(p2.extendedMoves[3]+1)+5*p2.extendedMoves[3]-1, White);
		return;
	}
}
	if(p1.extendedMoves[2] != 255 && p1.extendedMoves[3] >= p1.position[1]){
	p1.extendedMode = 1;
	calculateP2Moves(p2,&p1);	
	if(p1.orientation == 1){
		confirmMove(11,&p1);
		swapTurn(&p1,&p2);
		p2.extendedMode = 0;
		return;
	}
	}
 	if(p2.position[1]<=3 && p1.position[1] <= 4 && p1.wall_remain != 0){
		z = w.counter;
		p1.wall_mode = 1;
		LCD_DisplayMove(5,p1);
		if(w.counter == z && p2.position[1]>=1 && p2.position[1]<=5 && p2.position[0] <= 5 && h != 0){
		LCD_RemoveWall(3,3,1);	
		w.position[0] = p2.position[0];
		w.position[1] = p2.position[1]-1;
		w.orientation = 1;
		h = confirmBotWall(&w);
		
		}
		if(w.counter == z && p2.position[1]>=1 && p2.position[1]<=5 && p2.position[0]>=1 && h != 0){
			LCD_RemoveWall(3,3,1);	
		w.position[0] = p2.position[0]-1;
		w.position[1] = p2.position[1]-1;
		w.orientation = 1;
		h=confirmBotWall(&w);
		
		}
		if(w.counter == z && p2.position[1]>=1 && p2.position[1]<=5 && p2.position[0] <= 4 && h != 0){
			
			LCD_RemoveWall(3,3,1);	
		w.position[0] = p2.position[0]+1;
		w.position[1] = p2.position[1]-1;
		w.orientation = 1;
		h=confirmBotWall(&w);
		
		}
		if(w.counter == z && p2.position[1] >= 2 && p2.position[1]<=5 && p2.position[0] <=5 && h != 0){
			LCD_RemoveWall(3,3,1);	
		w.position[0] = p2.position[0];
		w.position[1] = p2.position[1]-2;
		w.orientation = 1;
		h=confirmBotWall(&w);
		
		}
		if(w.counter == z && p2.position[0]>= 1 && h != 0){
			LCD_RemoveWall(3,3,1);	
		w.position[0] = p2.position[0]-1;
		w.position[1] = p2.position[1];
		w.orientation = 0;
		h=confirmBotWall(&w);
		
		}
		if(w.counter == z && p2.position[0] <=5 && h != 0){
			LCD_RemoveWall(3,3,1);	
		w.position[0] = p2.position[0]+1;
		w.position[1] = p2.position[1];
		w.orientation = 0;
		h=confirmBotWall(&w);
		
		}
		if(h==0){
			swapTurn(&p1,&p2);
		}
		else{
		LCD_RemoveWall(3,3,1);	
		p1.wall_mode = 0;
		}
	}
	c=0;
	populateSchema();
	dfs2bot(Traps, visited2, p1.position[1]*2, p1.position[0]*2,&p1);
 	if(p1.turnOn == 1 && p1.wall_mode == 0){
		if(p2.lastWall == 1){
			p1.counter = 0;
			c=0;
			resetVisited2();
			populateSchema();
			dfs2bot(Traps, visited2, p1.position[1]*2, p1.position[0]*2,&p1);
		}
		if(p1.botmove[p1.counter] == 1 && p1.possibleMoves[4] == 255){p1.botmove[p1.counter] = 3;}
		if(p1.botmove[p1.counter] == 3 && p1.possibleMoves[0] == 255){p1.botmove[p1.counter] = 2;}
		if(p1.botmove[p1.counter] == 2 && p1.possibleMoves[2] == 255){p1.botmove[p1.counter] = 0;}
		if(p1.botmove[p1.counter] == 0 && p1.possibleMoves[6] == 255){p1.botmove[p1.counter] = 1;}
		if(p1.botmove[p1.counter] == 1 && p1.possibleMoves[4] == 255){p1.botmove[p1.counter] = 3;}
		if(p1.botmove[p1.counter] == 3 && p1.possibleMoves[0] == 255){p1.botmove[p1.counter] = 2;}
		if(p1.botmove[p1.counter] == 2 && p1.possibleMoves[2] == 255){p1.botmove[p1.counter] = 0;}
		confirmMove(p1.botmove[p1.counter],&p1);
		p1.counter++;
 		swapTurn(&p1,&p2);	
		}
	}

	

	



  int dfs2bot(uint8_t matrix[ROWS][COLS], int visited2[ROWS][COLS], int row, int col, Pedina *p1) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS || visited2[row][col] || matrix[row][col] == 1) {
        return 0; // Percorso non valido
    }

    // Se si raggiunge la riga 0, restituisci 1 (percorso valido)
    if (row == 12) {
        return 1;
    }

    // Imposta la cella come visitata
    visited2[row][col] = 1;
    c++;
    // Espandi la DFS alle celle adiacenti
    if (Traps[row+1][col] != 1 && row != 12 && dfs2bot(matrix, visited2, row + 2, col, p1)) {
        p1->botmove[c-2] = 1;
        c--;
        return 1;
    } else if (Traps[row][col+1] != 1 && col != 12  && dfs2bot(matrix, visited2, row, col + 2, p1) ) {
			p1->botmove[c-2] = 3;
        c--;
        return 1;
    } else if (Traps[row][col-1] != 1 && col != 0 && dfs2bot(matrix, visited2, row, col - 2, p1)) {
        p1->botmove[c-2] = 2;
        c--;
        return 1;
    } else if (Traps[row-1][col] != 1 && row != 0 && dfs2bot(matrix, visited2, row - 2, col, p1)) {
        p1->botmove[c-2] = 0;
        c--;
        return 1;
    }

    return 0; // Nessun percorso valido trovato
}
	
		
	





void resetVisited() {									//resetVisited delle celle visitate per dfs
    int k, j;
    for (k = 0; k < ROWS; k++) {
        for (j = 0; j < COLS; j++) {
            visited[k][j] = 0;
        }
    }
}

void resetVisited2() {									//resetVisited delle celle visitate per dfs
    int k, j;
    for (k = 0; k < ROWS; k++) {
        for (j = 0; j < COLS; j++) {
            visited2[k][j] = 0;
        }
    }
}
//0,0,1 / 1,0,1
 int populateSchema() {								//popolo la matrice con i muri
		
		int check1;
		//int check2;
		int i;
		int y;
		int x;	
	  c = 1;
	  for(i=0; i<w.counter; i+=6){
			y = w.allWalls[i];
      x = w.allWalls[i + 1];
		if(w.allWalls[i+2] == 1){	
    Traps[(x*2)+1][(y)*2-1] = 1;
		Traps[(x*2)+1][(y)*2] = 1;	
		Traps[(x*2)+1][(y*2)+1] = 1;
		Traps[(x*2)+1][(y*2)+2] = 1;			
		}
		else{
		Traps[(x*2)-1][(y*2)+1] = 1;
		Traps[(x*2)][(y*2)+1] = 1;
		Traps[(x*2)+1][(y*2)+1] = 1;
		Traps[(x*2)+2][(y*2)+1] = 1;
		}
		
}		
		}
 
int check(){	
	int check1;
	resetVisited();																				// ci sia una strada percorribile(DFS). 
	check1 = dfs(Traps,visited, p2.position[1]*2,p2.position[0]*2);
	if(check1 == 1){
		return 1;
	}
	else return 0;
}

int check2(){	
	int check2;
	resetVisited();																				// ci sia una strada percorribile(DFS). 
	check2 = dfs2(Traps,visited, p1.position[1]*2,p1.position[0]*2);
	if(check2 ==1){
		return 1;
	}
	else return 0;
}
	

void clearInvalidWall(){													//tolgo dalla matrice un muro che rendeva invalida la dfs e che non è stato inserito
		int i;
		int x;
		int y;
		for(i=w.counter-6; i<w.counter; i+=3){
			y = w.allWalls[i];
      x = w.allWalls[i + 1];
		if(w.allWalls[i+2] == 1){	
    Traps[(x*2)+1][(y)*2-1] = 0;
		Traps[(x*2)+1][(y)*2] = 0;	
		Traps[(x*2)+1][(y*2)+1] = 0;
		Traps[(x*2)+1][(y*2)+2] = 0;	
		}
		else{
		Traps[(x*2)-1][(y*2)+1] = 0;
		Traps[(x*2)][(y*2)+1] = 0;
		Traps[(x*2)+1][(y*2)+1] = 0;
		Traps[(x*2)+2][(y*2)+1] = 0;
		}

}

}

int dfs(uint8_t matrix[ROWS][COLS], int visited[ROWS][COLS], int row, int col) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS || visited[row][col] || matrix[row][col] == 1) {
        return 0; // Percorso non valido
    }


    // Se si raggiunge la riga 0, restituisci 1 (percorso valido)
    if (row == 0) {
        return 1;
    }

    // Imposta la cella come visitata
    visited[row][col] = 1;

    // Espandi la DFS alle celle adiacenti
    if (dfs(matrix, visited, row + 1, col) ||
        dfs(matrix, visited, row - 1, col) ||
        dfs(matrix, visited, row, col + 1) ||
        dfs(matrix, visited, row, col - 1)) {
        return 1; // Almeno un percorso valido trovato
    }

    return 0; // Nessun percorso valido trovato
}

int dfs2(uint8_t matrix[ROWS][COLS], int visited[ROWS][COLS], int row, int col) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS || visited[row][col] || matrix[row][col] == 1) {
        return 0; // Percorso non valido
    }


    // Se si raggiunge la riga 0, restituisci 1 (percorso valido)
    if (row == 12) {
        return 1;
    }

    // Imposta la cella come visitata
    visited[row][col] = 1;

    // Espandi la DFS alle celle adiacenti
    if (dfs2(matrix, visited, row + 1, col) ||
        dfs2(matrix, visited, row - 1, col) ||
        dfs2(matrix, visited, row, col + 1) ||
        dfs2(matrix, visited, row, col - 1)) {
        return 1; // Almeno un percorso valido trovato
    }

    return 0; // Nessun percorso valido trovato
}

void PlaceWall(Walls w){													
	uint16_t x = w.position[0];
	uint16_t y = w.position[1];
	char orientation = w.orientation;
		LCD_DisplayWall(x,y,orientation,Green);
}

void WallMode(struct Pedina *p){         //funzione per entrare in wallmode, se non si hanno muri si torna in movemode
	if(p->wall_remain == 0){
		 GUI_Text(20, 250, "No More walls left, Move", Red, BACKGROUND);
		MoveMode(p);
	}
	else{
	LCD_RemoveWall(w.position[0],w.position[1],w.orientation);									//tolgo dei muri simulati che potrebbero essere stati messi magari uscendo e rientrando da wallmode
	LCD_RemoveWall(3,3,1);	
	printAllWalls(&w);
	w.position[0] = 3;												//creo muro simulato al centro board
	w.position[1] = 3;
	w.orientation = 1;
	if(p->wall_mode == 0){
	p->wall_mode = 1;		//tolgo i quadretti di possible move alla pedina e display del muro
	p->extendedMode = 0;
	LCD_DisplayMove(5,*p);
	LCD_DisplayWall(w.position[0],w.position[1],w.orientation,Red);
	}
	else {p->wall_mode = 0;
		LCD_RemoveWall(w.position[0],w.position[1],w.orientation);							//reset dalla wallmode
		LCD_RemoveWall(3,3,1);	
		printAllWalls(&w);									//printo in verde tutti i muri già piazzati
		if (p->name == 1){
		calculateP2Moves(p2,p);						//calcolo le possibili mosse per la pedina in turno, tornando in moveMode
		}
		else 
			calculateP2Moves(p1,p);
	}
}
	}

void MoveMode(struct Pedina *p){
	p->wall_mode = 0;
}
void checkWinP1(){															//funzione per vedere se p1 ha vinto
	if(p1.position[1] == 6){
		p1.win = 1;
	}
	if(p1.win == 1){
		disable_RIT();
		disable_timer(0);
		LCD_Clear(Black);
		GUI_Text(40,105,"P1 ha vinto!", Red, Black);
		GUI_Text(10,130,"Premi RESET per ricominciare", Red, Black);
		LCD_Clear(Black);
	}
}

void checkWinP2(){												//funzione per vedere se p2 ha vinto
	if(p2.position[1] == 0){
		p2.win = 1;
	}
	if(p2.win == 1){
		disable_RIT();
		disable_timer(0);
		LCD_Clear(Black);
		GUI_Text(40,105,"P2 ha vinto!", Red, Black);
		GUI_Text(10,130,"Premi RESET per ricominciare", Red, Black);
		LCD_Clear(Black);
	}
}


void calculateP2Moves(Pedina c, Pedina *d){										//funzione di calcolo delle possibili mosse. qui utilizzo deglia rray temporanei
	int j = 0;																								
	int i = 0;
	int l = 0;
	int m = 0;
	int k = 0;
	int f = 0;
	int r = 0;
	int s = 0;
	int t = 0;
	uint16_t x = d->position[0];																//uso array temporaneo per le posizioni e setto ogni elemento delle possible moves a 255
	uint16_t y = d->position[1];
	uint16_t xcheck = c.position[0];
	uint16_t ycheck = c.position[1];
	char temparr[8];
	int temparr2[4];
	temparr2[0] = 255;
	temparr2[1] = 255;
	temparr2[2] = 255;
	temparr2[3] = 255;
	for(i = 0; i <4; i++){
		d->extendedMoves[i] = 255;
	}
	for(i = 0; i <8; i++){
		d->possibleMoves[i] = 255;
	}
	//posizione relativa alla pedina, e rispetto anche all'altra pedina, faccio dei check per vedere se le 2 pedine sono vicine, nel caso lo sono aumento una coordinata per il salto
	// se no è corretto il valore in temparr e sostituisco a possible move il valore temporaneo che è quello corretto
	//alla fine avrò in possbile moves 255 solo nelle posizioni dove non posso andare, successivamente ci sarà l'altro check di possible moves per controllare non vada oltre dei muri e 
	//assegno alla pedina le possible moves. nel caso ci fossero valori non consentiti(tipo fuori dalla board) questo è gestito in un'altra funzione già commentata
	if(x+1 == xcheck && y == ycheck){temparr[0] = x+2;temparr[1] = y;f = 1;}else{temparr[0] = x+1; temparr[1] = y;} //destra 0,1
	if(x-1 == xcheck && y == ycheck){temparr[2] = x-2;temparr[3] = y; f=2;}else{temparr[2] = x-1; temparr[3] = y;} //sinistra 2,3
	if(x == xcheck && y+1 == ycheck){temparr[4] = x;temparr[5] = y+2;f=3;}else{temparr[4] = x; temparr[5] = y+1;} //giu 4,5
	if(x == xcheck && y-1 == ycheck){temparr[6] = x;temparr[7] = y-2; f = 4;}else{temparr[6] = x; temparr[7] = y-1;}	//su 6,7	
for (k=0; k< w.counter; k+=3){
			if((w.allWalls[k+2] == 0 && w.allWalls[k] == temparr[0]-1 && w.allWalls[k+1] == temparr[1]) || 
				(f == 1 && w.allWalls[k+2] == 0 && w.allWalls[k] == temparr[0]-2 && w.allWalls[k+1] == temparr[1])){
				temparr[0] = 255;
				temparr[1] = 255;
		}
			
			
			if(f == 1 && w.allWalls[k+2] == 0 && w.allWalls[k] == xcheck && w.allWalls[k+1] == ycheck){
				for (r=0; r< w.counter; r+=3){
				if(w.allWalls[r+2] == 1 && w.allWalls[r] == xcheck && w.allWalls[r+1] == ycheck){
				temparr2[0] = 255;
				temparr2[1] = 255;
				s=1;
				}
				else if(s == 0){
					d->extOrien = 1;
					temparr2[0] = xcheck;
					temparr2[1] = ycheck + 1;
				}
				if(w.allWalls[r+2] == 1 && w.allWalls[r] == xcheck && w.allWalls[r+1] == ycheck -1){
					temparr2[2] = 255;
					temparr2[3] = 255;
					t = 1;
				}
				else if(t == 0){
					d->extOrien = 1;
					temparr2[2] = xcheck;
					temparr2[3] = ycheck - 1;
				}
			}
		s = 0;
		t = 0;
		}
			

			if((w.allWalls[k+2] == 0 && w.allWalls[k] == temparr[2] && w.allWalls[k+1] == temparr[3])|| 
				(f == 2 && w.allWalls[k+2] == 0 && w.allWalls[k] == temparr[2]+1 && w.allWalls[k+1] == temparr[3])){
				temparr[2] = 255;
				temparr[3] = 255;
		}
			
			if(f == 2 && w.allWalls[k+2] == 0 && w.allWalls[k] == xcheck-1 && w.allWalls[k+1] == ycheck){
				for (r=0; r< w.counter; r+=3){
				if(w.allWalls[r+2] == 1 && w.allWalls[r] == xcheck && w.allWalls[r+1] == ycheck){
				temparr2[0] = 255;
				temparr2[1] = 255;
				s=1;
				}
				else if(s == 0){
					d->extOrien = 1;
					temparr2[0] = xcheck;
					temparr2[1] = ycheck + 1;
				}
				if(w.allWalls[r+2] == 1 && w.allWalls[r] == xcheck && w.allWalls[r+1] == ycheck -1){
					temparr2[2] = 255;
					temparr2[3] = 255;
					t = 1;
				}
				else if(t == 0){
					d->extOrien = 1;
					temparr2[2] = xcheck;
					temparr2[3] = ycheck - 1;
				}
			}
		s = 0;
		t = 0;
		}
			if((w.allWalls[k+2] == 1 && w.allWalls[k] == temparr[4] && w.allWalls[k+1] == temparr[5]-1)|| 
				(f == 3 && w.allWalls[k+2] == 1 && w.allWalls[k] == temparr[4] && w.allWalls[k+1] == temparr[5]-2)){
				temparr[4] = 255;
				temparr[5] = 255;
		}
		if(w.allWalls[k+2] == 1 && w.allWalls[k] == xcheck && w.allWalls[k+1] == ycheck && f == 3){
			for (r=0; r< w.counter; r+=3){
			if(w.allWalls[r+2] == 0 && w.allWalls[r] == xcheck-1 && w.allWalls[r+1] == ycheck){
				temparr2[0] = 255;
				temparr2[1] = 255;
				s = 1;
			}
			else if ( s==0){
				d->extOrien = 0;
				temparr2[0] = xcheck-1;
				temparr2[1] = ycheck;
			}
			if(w.allWalls[r+2] == 0 && w.allWalls[r] == xcheck && w.allWalls[r+1] == ycheck){
				temparr2[2] = 255;
				temparr2[3] = 255;
				t = 1;
			}
			else if (t == 0){
				d->extOrien = 0;
				temparr2[2] = xcheck+1;
				temparr2[3] = ycheck;
		}
	}
		s = 0;
		t = 0;
	}
		
	if((w.allWalls[k+2] == 1 && w.allWalls[k] == temparr[6] && w.allWalls[k+1] == temparr[7])|| 
				(f == 4 && w.allWalls[k+2] == 1 && w.allWalls[k] == temparr[6] && w.allWalls[k+1] == temparr[7]+1)){
				temparr[6] = 255;
				temparr[7] = 255;
	}
		if(w.allWalls[k+2] == 1 && w.allWalls[k] == xcheck && w.allWalls[k+1] == ycheck -1 && f == 4){
				for (r=0; r< w.counter; r+=3){
				if(w.allWalls[r+2] == 0 && w.allWalls[r] == xcheck-1 && w.allWalls[r+1] == ycheck){
				temparr2[0] = 255;
				temparr2[1] = 255;
				s = 1;
				}
				else if (s==0){
				d->extOrien = 0;
				temparr2[0] = xcheck - 1;
				temparr2[1] = ycheck;
				}
				if(w.allWalls[r+2] == 0 && w.allWalls[r] == xcheck && w.allWalls[r+1] == ycheck ){
				temparr2[2] = 255;
				temparr2[3] = 255;
				t=1;
				}
				else if (t == 0){
				d->extOrien = 0;
				temparr2[2] = xcheck +1;
				temparr2[3] = ycheck;
				}
			}
			s = 0;
			t = 0;
	}
}

	for(j=0;j<8; j=j+2){
		l = temparr[j];
		m = temparr[j+1];
		if(l >= 0 && m < 7){
			d->possibleMoves[j] = l;
			d->possibleMoves[j+1] = m;
			LCD_Colora(1+30*d->possibleMoves[j]+5*d->possibleMoves[j], 1+30*d->possibleMoves[j+1]+5*d->possibleMoves[j+1], 30*(d->possibleMoves[j]+1)+5*d->possibleMoves[j]-1, 30*(d->possibleMoves[j+1]+1)+5*d->possibleMoves[j+1]-1, Grey);
		}
		else {
			d->possibleMoves[j] = 255;
			d->possibleMoves[j+1] = 255;
		}
	}

	if(temparr2[0] != 255 &&  temparr2[0]<=7 && temparr2[2] >= 0 ){d->extendedMoves[0] = temparr2[0]; d->extendedMoves[1] = temparr2[1];}
	if(temparr2[2] != 255 &&  temparr2[2]<=7 && temparr2[2] >= 0 ){d->extendedMoves[2] = temparr2[2]; d->extendedMoves[3] = temparr2[3];}
		
	if(((d->extendedMoves[0] != 255	|| d->extendedMoves[2] !=255) && d->extendedMode == 1) || (p1.botmode == 1 && p1.turnOn == 1 && (d->extendedMoves[0] != 255	|| d->extendedMoves[2] !=255))){
		for(j=0;j<4; j=j+2){
		l = temparr2[j];
		m = temparr2[j+1];
		if(l >= 0 && m < 7){
		LCD_Colora(1+30*d->extendedMoves[j]+5*d->extendedMoves[j], 1+30*d->extendedMoves[j+1]+5*d->extendedMoves[j+1], 30*(d->extendedMoves[j]+1)+5*d->extendedMoves[j]-1, 30*(d->extendedMoves[j+1]+1)+5*d->extendedMoves[j+1]-1, Yellow);
		}
	}
}
}
void swapTurn(Pedina *p, Pedina *q){											//funzione per scambio turno che pulisce tutte le cose simulate, controlla la vittoria e da il controllo all'altro player
	checkWinP1();
	checkWinP2();
	if(p1.win || p2.win ==1){
		return ;
	}
	else{
	if(q->turnOn == 1){
		p->wall_mode = 0;
		q->wall_mode = 0;
		q->extendedMode = 0;
		calculateP2Moves(p2,&p1);
		LCD_RemoveWall(w.position[0],w.position[1],w.orientation);
		printAllWalls(&w);
		p->turnOn = 1;
		q->turnOn = 0;
		p->time_left = 20;
		if(p->extendedMoves[0] != 255 || p->extendedMoves[2] != 255){
		GUI_Text(3, 250, "Side JUMP:Press KEY2", Black, BACKGROUND);
		GUI_Text(3, 250, "                    ", Black, BACKGROUND);
	}
	}
	else {
		q->wall_mode = 0;
		p->wall_mode = 0;
		p->extendedMode = 0;
		calculateP2Moves(p1,&p2);
		LCD_RemoveWall(w.position[0],w.position[1],w.orientation);
		printAllWalls(&w);
		p->turnOn = 0;
		q->turnOn = 1;
		q->time_left = 20;
		if(q->extendedMoves[0] != 255 || q->extendedMoves[2] != 255){
		GUI_Text(3, 250, "Side JUMP:Press KEY2", Black, BACKGROUND);
		GUI_Text(3, 250, "                    ", Black, BACKGROUND);
	}
	}
}
}
void saveLastMove(uint8_t playerId, uint8_t isPlacingBarrier, uint8_t isBarrierHorizontal, uint8_t posY, uint8_t posX) { //funzione per salvare la mossa in memoria
	uint8_t mode = (isPlacingBarrier << 4) | isBarrierHorizontal;
 	last_move = (playerId << 24) | (mode << 16) | (posY << 8) | (posX);
}


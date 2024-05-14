#ifndef GAME_H
#define GAME_H
#define SIZE 7
#define ROWS 13
#define COLS 13

struct Pedina{
	int botmode;
	int extendedMode;
	int extOrien; // 1=x 0=y
	int extendedMoves[4];
	int botmove[256];
	int counter;
	char turnOn;
	char time_left;
	char wall_mode;
	char win;
	char position[2];
	char wall_remain;
	char possibleMoves[8];
	char orientation;
	char name;
	char lastWall;
};
struct Coordinate{
    int row;
    int col;
};
struct Match{
	int mode;    //if mode = 1, you are in choosing mode, else no
	int Board;    // Board = 1 or 2
	int Player;   //  Plyaer->1(vs NPC) player->2(vs human)
};

struct Walls{
	char counter;
	char allWalls[96];
	char orientation;
	char position[2];
	//char allWalls[48]; //nella forma x,y,orientation, e sono il punto piu alto del muro se verticale il punto piu a sinistra se orizzontale
};
int isPlacingBarrierSet();
void resetVisited2();
typedef struct Pedina Pedina;
typedef struct Walls Walls;
typedef struct Match Match;
typedef struct Coordinate Coordinate;
void start_single_human(void);
void WallMode(struct Pedina *p);
void MoveMode(struct Pedina *p);
void calculateP2Moves(Pedina c, Pedina *d);
void swapTurn( Pedina *p,  Pedina *q);
int isValid(int row, int col);
int dfs(uint8_t matrix[ROWS][COLS], int visited[ROWS][COLS], int row, int col);
int dfs2(uint8_t matrix[ROWS][COLS], int visited[ROWS][COLS], int row, int col);
int check(void);
int check2(void);
int populateSchema(void);
void resetVisited(void);
void clearInvalidWall(void);
void startscreen(void);
void saveLastMove(uint8_t playerId, uint8_t isPlacingBarrier, uint8_t isBarrierHorizontal, uint8_t posY, uint8_t posX);
void choose_mode(void);
void choose_single_mode(void);
void choose_multiplayer_mode(void);
void NPCmove(void);
void start_single_NPC(void);
int dfs2bot(uint8_t matrix[ROWS][COLS], int visited[ROWS][COLS], int row, int col,Pedina *p1);
#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

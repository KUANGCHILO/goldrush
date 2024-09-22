//移動方向
#define UP 0
#define LEFT 1
#define RIGHT 2

//表示
#define PLAYER_A 'A'
#define PLAYER_B 'B'
#define GOLD 'G'
#define EMPTY ' '

//盤の大きさを変更する
#define N_COLUMN 50
#define N_ROW 30

//ターン数を変更する
#define MAX_TURN 150

//対戦相手の関数に変更する


#define MOVE_PLAYERB(a,b,c,d,e,f) move6(a,b,c,d,e,f)

//#define MOVE_PLAYERB(a,b,c,d,e,f) moves225055(a,b,c,d,e,f)
//#define MOVE_PLAYERB(a,b,c,d,e,f) moves245034(a,b,c,d,e,f)

#define MOVE_PLAYERA(a,b,c,d,e,f) moves23b006(a,b,c,d,e,f)


//それぞれのプレイヤーの関数
extern int moveA(char player,int row,int column,int score,int turn,const char board[N_ROW][N_COLUMN]);
extern int moveB(char player,int row,int column,int score,int turn,const char board[N_ROW][N_COLUMN]);
extern int move2020(char player,int row,int column,int score,int turn,const char board[N_ROW][N_COLUMN]);
extern int move2021(char player,int row,int column,int score,int turn,const char board[N_ROW][N_COLUMN]);
extern int move2023(char player,int row,int column,int score,int turn,const char board[N_ROW][N_COLUMN]);
extern int moves23b006(char player,int row,int column,int score,int turn,const char board[N_ROW][N_COLUMN]);

extern int moves225055(char player,int row,int column,int score,int turn,const char board[N_ROW][N_COLUMN]);
extern int moves245020(char player,int row,int column,int score,int turn,const char board[N_ROW][N_COLUMN]);
extern int moves245034(char player,int row,int column,int score,int turn,const char board[N_ROW][N_COLUMN]);
extern int move6(char player,int row,int column,int score,int turn,const char board[N_ROW][N_COLUMN]);


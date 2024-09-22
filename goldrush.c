#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include"goldrush.h"

void printBoard(int turn,int score_a,int score_b,const char board[N_ROW][N_COLUMN]);
int evaluate(int row,int column,const char board[N_ROW][N_COLUMN]);
void print_result(int score_a,int score_b);

int main(void)
{
    char board[N_ROW][N_COLUMN];
    int i,j,turn;
    int player[2][4]; // row, column, score, alive
    int gold=0;
    int empty=0;
    
    // 疑似乱数のシードを設定
    srand(time(NULL));
    
    // ボードにプレイヤーとゴールドを配置
    for (i=0;i<N_ROW;i++)
    {
        for (j=0;j<N_COLUMN;j++)
        {
            if (i==0&&j==N_COLUMN/2-1)
            {
                board[i][j]=PLAYER_A;
                
                // player[0]に初期値を代入
                player[0][0]=i;
                player[0][1]=j;
                player[0][2]=0;
                player[0][3]=1;
            }
            else if (i==0&&j==N_COLUMN/2)
            {
                board[i][j]=PLAYER_B;
                
                // player[1]に初期値を代入
                player[1][0]=i;
                player[1][1]=j;
                player[1][2]=0;
                player[1][3]=1;
            }
            else if (rand()%N_COLUMN==0) // 1行に1つの割合
            {
                board[i][j]=GOLD;
                gold++;
            }
            else
            {
                board[i][j]=EMPTY;
                empty++;
            }
        }
        
    }
    
    // goldが1つもない場合は、空きセルに補充
    while (gold==0&&empty>0)
    {
        for (i=0;i<N_ROW;i++)
        {
            for (j=0;j<N_COLUMN;j++)
            {
                if (board[i][j]==EMPTY&&rand()%N_COLUMN==0)
                {
                    board[i][j]=GOLD;
                    gold++;
                }
            }
        }
    }
    
    turn=0;
    printBoard(turn,player[0][2],player[1][2],board);
    
    int player_id=rand()%2;
    
    while (++turn<=MAX_TURN)
    {
        sleep(1);
        
        if (player[player_id][3]==1) // 生きていたら
        {
            int row, column,result;
            char player_char=(player_id==0)?PLAYER_A:PLAYER_B;
            
            if (player_id==0)
                result=MOVE_PLAYERA(player_char,player[player_id][0],player[player_id][1],player[player_id][2],turn,board);
            else
                result=MOVE_PLAYERB(player_char,player[player_id][0],player[player_id][1],player[player_id][2],turn,board);
            
            switch (result)
            {
                case UP:
                    row=player[player_id][0]+1;
                    column=player[player_id][1];
                    break;
                case LEFT:
                    row=player[player_id][0];
                    column=player[player_id][1]-1;
                    break;
                case RIGHT:
                    row=player[player_id][0];
                    column=player[player_id][1]+1;
                    break;
                default:
                    player[player_id][3]=0;
            }
            
            board[player[player_id][0]][player[player_id][1]]=EMPTY;
            
            if (player[player_id][3]==1) // 生きていたら
            {
                switch (evaluate(row,column,board))
                {
                    case 1:
                        player[player_id][2]++;
                    case 0:
                        player[player_id][0]=row;
                        player[player_id][1]=column;
                        board[row][column]=player_char;
                        break;
                    case -1:
                        player[player_id][3]=0;
                        break;
                }
            }
        }
        
        printBoard(turn,player[0][2],player[1][2],board);
        
        if ((player[0][3]==0&&player[1][3]==0)||turn==MAX_TURN)
        {
            print_result(player[0][2],player[1][2]);   
            break;
        }
        else if ((player_id==0&&player[1][3]==1)||(player_id==1&&player[0][3]==0))
            player_id=1;
        else
            player_id=0;
    }
    
    return EXIT_SUCCESS;
}

void print_result(int score_a,int score_b)
{
    if (score_a>score_b)
    {
        printf("%cの勝ち\n",PLAYER_A);
    }
    else if (score_a<score_b)
    {
        printf("%cの勝ち\n",PLAYER_B);
    }
    else
    {
        printf("%cと%cは引き分け\n",PLAYER_A,PLAYER_B);
    }
}

int evaluate(int row, int column, const char board[N_ROW][N_COLUMN])
{
    // ボードの範囲外
    if (row<0||row>=N_ROW||column<0||column>=N_COLUMN)
        return -1;
    // 移動可能
    else if (board[row][column]==EMPTY)
        return 0;
    // ゴールドあり
    else if (board[row][column]==GOLD)
        return 1;
    // それ以外（プレイヤーと重なった場合） 
    else
        return -1;
}

// 引数として与えられたboardを表示する
void printBoard(int turn,int score_a,int score_b,const char board[N_ROW][N_COLUMN])
{
    int i,j;
    // printf("\033[2J"); //画面クリア
    printf("ターン%3d: %c=%d vs %c=%d\n",turn,PLAYER_A,score_a,PLAYER_B,score_b);   
    
    for (i=N_ROW-1;i>=0;i--)
    {
        printf("%2d|",i);
        for (j=0;j<N_COLUMN;j++)
        {
            switch (board[i][j])
            {
                case GOLD:
                    //printf("\x1b[33m");
                    printf("\x1b[43m");
                    break;
                case PLAYER_A:
                    printf("\x1b[1m\x1b[35m");
                    break;
                case PLAYER_B:
                    printf("\x1b[1m\x1b[36m");
                    break;
            }
            
            printf("%c",board[i][j]);
            printf("\x1b[49m\x1b[39m\x1b[0m");
        }
        printf("|\n");
    }
}
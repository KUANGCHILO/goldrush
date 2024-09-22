#include "goldrush.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define near_distance 7

int Distance(int row_A, int column_A, int row_B, int column_B);
int max(int a, int b);
int min(int a,int b);
typedef struct {
    int left;
    int right;
    int up;
} dodge;
dodge get_dodge(int row,int col,int en_row,int en_col);


int moves23b006(char player, int row, int column, int score, int turn, const char board[N_ROW][N_COLUMN]) {
    char enemy_name;
    int enemy_row = -1, enemy_col = -1, enemy_life=0;  
    double gold_list[N_ROW * N_COLUMN][6];   // row, col, self_distance, enemy_distance, nearby_gold_count ,efficiency
    int gold_count = 0;
    int tar_row=-1,tar_col=-1;
    if (player == PLAYER_A) {
        enemy_name = PLAYER_B;
    } else {
        enemy_name = PLAYER_A;
    }
    
    //ボードをチェックする
    for (int i = 0; i < N_ROW; i++) {
        for (int j = 0; j < N_COLUMN; j++) {
            if (board[i][j] == GOLD) {
                int self_distance = Distance(row, column, i, j);
                gold_list[gold_count][0] = i;   //row
                gold_list[gold_count][1] = j;   //col
                gold_list[gold_count][2] = self_distance;
                int nearby_gold_count = 1;  //近くのgoldを数える
                for (int m = max(0, i); m <= min(N_ROW-1, i+near_distance); m++) {
                    for (int n = max(0, j-near_distance); n <= min(N_COLUMN-1, j+near_distance); n++) {
                        if (board[m][n] == GOLD) {
                            nearby_gold_count++;
                        }
                    }
                }
                gold_list[gold_count][4] = nearby_gold_count;
                gold_list[gold_count][5] = 100*nearby_gold_count/self_distance; //効率を計算する
                gold_count++;
            } else if (board[i][j] == enemy_name) {
                enemy_row = i;
                enemy_col = j;
                enemy_life = 1;
            }
        }
    }

    if (enemy_life==1){ //相手とgoldとの距離を計算する
        for (int i =0;i<gold_count;i++){
            gold_list[i][3]=Distance(enemy_row,enemy_col,gold_list[i][0],gold_list[i][1]);
        }
    }

    int min_dis=999;
    for(int i=0;i<gold_count;i++){  //5マス以内にgoldがあるか確認する
        if(tar_col==column){
            if(gold_list[i][2]<min_dis && gold_list[i][2]<=gold_list[i][3]){
                min_dis=gold_list[i][2];
                tar_row=gold_list[i][0];
                tar_col=gold_list[i][1];
            }
            continue;
        }
        if (gold_list[i][2]<=5 && gold_list[i][2]<min_dis && gold_list[i][2]<=gold_list[i][3]){
            min_dis=gold_list[i][2];
            tar_row=gold_list[i][0];
            tar_col=gold_list[i][1];
        }
    }

    if (min_dis<=5){    //5マス以内にgoldがあればそれを対象として選択
        dodge dodge_moves = get_dodge(row, column, enemy_row, enemy_col);
        if (tar_col<column && dodge_moves.left==1){;
            return LEFT;
        }
        if (tar_col>column && dodge_moves.right==1){
            return RIGHT;
        }
        if (tar_row>row && dodge_moves.up==1){
            return UP;
        }
    }
    //相手より早く入手できる効率の良いGOLDを見つける
    int max_eff=0;
    int tar_index=-1;
    int tar_flag=0;
    for(int i=0;i<gold_count;i++){
        if(gold_list[i][5]>max_eff && (gold_list[i][2]<gold_list[i][3] || enemy_life==0) && MAX_TURN-turn>=gold_list[i][2]){
            max_eff=gold_list[i][5];
            tar_index=i;
            tar_row=gold_list[i][0];
            tar_col=gold_list[i][1];
            tar_flag=1;
        }
    }
    dodge dodge_moves = get_dodge(row, column, enemy_row, enemy_col);
    if(tar_col<column && dodge_moves.left==1 && tar_flag==1){
        return LEFT;
    }
    if(tar_col>column && dodge_moves.right==1 && tar_flag==1){
        return RIGHT;
    }
    return UP;

}

int max(int a, int b) {
    return (a > b) ? a : b;
}

dodge get_dodge(int row,int col,int en_row,int en_col){
    dodge d;
    d.left=1;
    d.right=1;
    d.up=1;
    if(en_row==-1){
        return d;
    }
    if (col==0 && (row==en_row || col-1==en_col)){
        d.left=0;
    }
    if (col==N_COLUMN-1 && (row==en_row || col+1==en_col)){
        d.right=0;
    }
    if (row==N_ROW-1 && (col==en_col || row+1==en_row)){
        d.up=0;
    }
    return d;
}

int Distance(int row,int col,int tar_row,int tar_col){
    if (row>tar_row){
        return 999;
    }
    return abs(row-tar_row)+abs(col-tar_col);
}

int min(int a,int b){
    if (a>b){
        return b;
    }
    return a;
}
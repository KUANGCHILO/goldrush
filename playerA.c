#include "goldrush.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define near_distance 7

static int total_gold_count = -1;
int Distance(int row_A, int column_A, int row_B, int column_B);
int max(int a, int b);
int min(int a,int b);
typedef struct {
    int left;
    int right;
    int up;
} dodge;
dodge get_dodge(int row,int col,int en_row,int en_col);


int move6(char player, int row, int column, int score, int turn, const char board[N_ROW][N_COLUMN]) {
    char enemy_name;
    int enemy_row = -1, enemy_col = -1, enemy_life=0;  // 初始化为无效值
    double gold_list[N_ROW * N_COLUMN][6];   // row, col, self_distance, enemy_distance, nearby_gold_count ,efficiency
    int gold_count = 0;
    int tar_row=-1,tar_col=-1;
    int enemy_score=0;
    if (player == PLAYER_A) {
        enemy_name = PLAYER_B;
    } else {
        enemy_name = PLAYER_A;
    }
    
    // 遍历棋盘
    for (int i = 0; i < N_ROW; i++) {
        for (int j = 0; j < N_COLUMN; j++) {
            if (board[i][j] == GOLD) {
                int self_distance = Distance(row, column, i, j);
                gold_list[gold_count][0] = i;   //row
                gold_list[gold_count][1] = j;   //col
                gold_list[gold_count][2] = self_distance;
                int nearby_gold_count = 1;
                for (int m = max(0, i); m <= min(N_ROW-1, i+near_distance); m++) {
                    for (int n = max(0, j-near_distance); n <= min(N_COLUMN-1, j+near_distance); n++) {
                        if (board[m][n] == GOLD) {
                            nearby_gold_count++;
                        }
                    }
                }
                gold_list[gold_count][4] = nearby_gold_count;
                gold_list[gold_count][5] = 100*nearby_gold_count/self_distance;
                gold_count++;
            } else if (board[i][j] == enemy_name) {
                enemy_row = i;
                enemy_col = j;
                enemy_life = 1;
            }
        }
    }



    if (total_gold_count==-1){
        total_gold_count=gold_count;
    }
    enemy_score=total_gold_count-gold_count-score;

    if (enemy_life==1){
        for (int i =0;i<gold_count;i++){
            gold_list[i][3]=Distance(enemy_row,enemy_col,gold_list[i][0],gold_list[i][1]);
        }
    }
    // printf("Gold List:\n");
    // for(int i = 0; i < gold_count; i++) {
    //     printf("Gold %d: row=%.1f, column=%.1f, self_distance=%.1f, enemy_distance=%.1f, nearby_gold_count=%.1f ,efficiency=%.4f\n", i, gold_list[i][0], gold_list[i][1], gold_list[i][2],gold_list[i][3],gold_list[i][4],gold_list[i][5]);
    // }

    int min_dis=999;
    for(int i=0;i<gold_count;i++){
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

    if (min_dis<=5){
        dodge dodge_moves = get_dodge(row, column, enemy_row, enemy_col);
        if (tar_col<column && dodge_moves.left==1){
            // printf("左上5格內有金幣");
            return LEFT;
        }
        if (tar_col>column && dodge_moves.right==1){
            // printf("右上5格內有金幣");
            return RIGHT;
        }
        if (tar_row>row && dodge_moves.up==1){
            // printf("上5格內有金幣");
            return UP;
        }
    }

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
        // printf("目標(%d,%d)在左且無障礙",tar_col,tar_row);
        return LEFT;
    }
    if(tar_col>column && dodge_moves.right==1 && tar_flag==1){
        // printf("目標(%d,%d)在右且無障礙",tar_col,tar_row);
        return RIGHT;
    }
    // printf("目標離對方較近");
    return UP;

}

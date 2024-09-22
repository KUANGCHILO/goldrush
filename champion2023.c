#include "goldrush.h"
#include <stdio.h>
int DistanceCalculator(int row_A, int column_A, int row_B, int column_B);
int move2023(char player,int row,int column,int score,int turn,const char board[N_ROW][N_COLUMN])
{
    // 自分の座標を表示
    printf("Player A: row=%d, column=%d\n", row, column);
    
    // ゴールドの座標と距離を格納する配列
    int gold_list[N_ROW * N_COLUMN][3];
    int gold_count = 0;
    
    // ゴールドの位置を検索し、配列に格納
    for(int i = 0; i < N_ROW; i++) {
        for(int j = 0; j < N_COLUMN; j++) {
            if(board[i][j] == GOLD) {
                int distance = DistanceCalculator(row, column, i, j);
                gold_list[gold_count][0] = i;
                gold_list[gold_count][1] = j;
                gold_list[gold_count][2] = distance;
                gold_count++;
            }
        }
    }
    
    // ゴールドが存在しない場合は右に移動
    if(gold_count == 0) {
        return RIGHT;
    }
    
    // ゴールドを距離の近い順にソート
    for(int i = 0; i < gold_count - 1; i++) {
        for(int j = 0; j < gold_count - i - 1; j++) {
            if(gold_list[j][2] > gold_list[j+1][2]) {
                // 距離が近いゴールドと入れ替え
                int temp_row = gold_list[j][0];
                int temp_col = gold_list[j][1];
                int temp_distance = gold_list[j][2];
                gold_list[j][0] = gold_list[j+1][0];
                gold_list[j][1] = gold_list[j+1][1];
                gold_list[j][2] = gold_list[j+1][2];
                gold_list[j+1][0] = temp_row;
                gold_list[j+1][1] = temp_col;
                gold_list[j+1][2] = temp_distance;
            }
        }
    }
    
    // ゴールドの座標と距離を表示
    printf("Gold List:\n");
    for(int i = 0; i < gold_count; i++) {
        printf("Gold %d: row=%d, column=%d, distance=%d\n", i+1, gold_list[i][0], gold_list[i][1], gold_list[i][2]);
    }
    
    // 最も近いゴールドの座標を取得
    int target_row = gold_list[0][0];
    int target_col = gold_list[0][1];
    
    // ゴールドの位置と自分の位置を比較して移動方向を決定
    if(target_row > row) {
        return UP;
    } else if(target_col < column) {
        return LEFT;
    } else if(target_col > column) {
        return RIGHT;
    } else {
        return UP;
    }
}
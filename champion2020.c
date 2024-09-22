#include "goldrush.h"

void NearestGoldSearch(int row,int column,const char board[N_ROW][N_COLUMN],int target_list[3][3],int start_row);
// 近くのゴールドを探す関数

int DistanceCalculator(int row_A,int column_A,int row_B,int column_B);
// 2点間の距離を計算する関数

int move2020(char player,int row,int column,int score,int turn,const char board[N_ROW][N_COLUMN]){
    
    char ENEMY;
    if(player==PLAYER_A)
        ENEMY=PLAYER_B;
    else
        ENEMY=PLAYER_A;
    // 対戦相手を表す文字を取得
    
    
    int enemy_coordinate[2]={N_ROW-1,N_COLUMN-1};
    // 対戦相手の座標は配列に存在しない要素へのアクセスを防ぐため、仮の値を入れておく
    int enemy_exist_flag=0;
    // 対戦相手が現存するか確認するためのフラグ
    for(int i=N_ROW-1;i>=0;i--){
        for (int j=0;j<N_COLUMN;j++){
        // 基準2の1　繰り返し処理に対応
            if(board[i][j]==ENEMY){
            // 対戦相手が発見されたら
                enemy_coordinate[0]=i;
                enemy_coordinate[1]=j;
                // 対戦相手の座標を取得
                enemy_exist_flag=1;
                // 
            }
        }
    }
    
    
    int dont_move_flag=1;
    // 相手のいるマスや盤の外に移動しないようにするフラグ
    // 当初は要素3つの配列で三方向のフラグとしていたが、誤った返り値につながったため単一の変数とした
    
    if(board[row+1][column]==ENEMY)dont_move_flag=dont_move_flag*2;
        // 直上のマスに相手がいる場合、上に動かないようにする
    if(board[row][column-1]==ENEMY||column==0)dont_move_flag=dont_move_flag*3;
        // 左隣に相手がいる場合、あるいは自分が左端にいる場合、左に動かないようにする
    if(board[row][column+1]==ENEMY||column==N_COLUMN-1)dont_move_flag=dont_move_flag*5;
        // 右隣に相手がいる場合、あるいは自分が右端にいる場合、右に動かないようにする
    // 以上のように、異なる素数をかけることで三方向のフラグのON・OFFを区別している。
    
    
    int my_target_list[3][3]={{N_ROW-1,N_COLUMN-1,N_ROW+N_COLUMN-2},{N_ROW-1,N_COLUMN-1,N_ROW+N_COLUMN-2},{N_ROW-1,N_COLUMN-1,N_ROW+N_COLUMN-2}};
    // 自分に近い順に3つのゴールドの情報を格納する配列
    // 格納する情報はゴールドの座標と、自分からの距離
    int start_row=N_ROW-1;
    if(enemy_exist_flag==0){
    // 相手が何らかの原因で消えた場合
        for(int i=N_ROW-1;i>=row;i--){
            for (int j=0;j<N_COLUMN;j++){
                if(board[i][j]==GOLD){
                    start_row=i;
                    // start_rowの調整で行動パターンを変える
                    // 通常時は自分に最も近いゴールドを目指すが、相手が消えた場合は盤上の全てのゴールドを取る
                }
            }
        }
    }
    NearestGoldSearch(row,column,board,my_target_list,start_row);
    // 自分から最も近いゴールドを特定し、行先とする
    // 基準2の2　board配列の要素を参照してその値を分析し、進行方向を決めている
    
    
    int enemy_target_list[3][3]={{N_ROW-1,N_COLUMN-1,N_ROW+N_COLUMN-2},{N_ROW-1,N_COLUMN-1,N_ROW+N_COLUMN-2},{N_ROW-1,N_COLUMN-1,N_ROW+N_COLUMN-2}};
    if(enemy_exist_flag!=0)NearestGoldSearch(enemy_coordinate[0],enemy_coordinate[1],board,enemy_target_list,N_ROW-1);
    // 相手に最も近いゴールドを算出
    
    
    
    // この部分は改良の余地があるが、非常に複雑になりそうなので改善は保留
    // 当初は相手の動きを先回りして、相手の目指すゴールドを取る機能があったが、想定外の動作をしたため削除
    if(enemy_exist_flag!=0){
        if(enemy_target_list[0][0]==my_target_list[0][0]&&enemy_target_list[0][1]==my_target_list[0][1]){
        // 自分の標的に対し相手の方が近ければ、方針転換
            if(enemy_target_list[0][2]<my_target_list[0][2]){
                if(my_target_list[0][1]==my_target_list[1][1]){
                    my_target_list[0][0]=my_target_list[2][0];
                    my_target_list[0][1]=my_target_list[2][1];
                    my_target_list[0][2]=my_target_list[2][2];
                }else{
                    my_target_list[0][0]=my_target_list[1][0];
                    my_target_list[0][1]=my_target_list[1][1];
                    my_target_list[0][2]=my_target_list[1][2];
                }
            }
        }
    }
    
    
    
    // dont_move_flagや標的の座標に基づいて返り値を設定
    if(my_target_list[0][1]>column){
        switch(dont_move_flag%5){
            case 0:
                if(dont_move_flag%2==0){
                    return LEFT;
                }else{
                    return UP;
                }
            default:
                return RIGHT;
                break;
        }
    }else if(my_target_list[0][1]<column){
        switch(dont_move_flag%3){
            case 0:
                if(dont_move_flag%2==0){
                    return RIGHT;
                }else{
                    return UP;
                }
            default:
                return LEFT;
                break;
        }
    }else{
        switch(dont_move_flag%2){
            case 0:
                if(column>=(N_COLUMN/2)){
                    return LEFT;
                }else{
                    return RIGHT;
                }
            default:
                return UP;
                break;
        }
    }
}


int DistanceCalculator(int row_A,int column_A,int row_B,int column_B){
    int distance_row=row_A-row_B;
    // 縦方向の距離を計算
    if(distance_row<0)distance_row=-distance_row;
    // 長さは非負整数なので、負になったら修正
        
    int distance_column=column_A-column_B;
    // 横方向の距離を計算
    if(distance_column<0)distance_column=-distance_column;
    // 長さは非負整数なので、負になったら修正
    
    return distance_row+distance_column;
    // 今回は斜めに動けないため、縦横の距離を単純に足して距離とする
}


void NearestGoldSearch(int row,int column,const char board[N_ROW][N_COLUMN],int target_list[3][3],int start_row){
    int distance;
    
    for(int i=start_row;i>=row;i--){
        for (int j=0;j<N_COLUMN;j++){
            if(board[i][j]==GOLD){
            // ゴールドが見つかったら
                distance=DistanceCalculator(row,column,i,j);
                // 距離を取得
                
                if(distance<=target_list[0][2]){
                // 今までで最も近いゴールドであれば、配列の全要素を更新
                    target_list[2][0]=target_list[1][0];
                    target_list[2][1]=target_list[1][1];
                    target_list[2][2]=target_list[1][2];
                    
                    target_list[1][0]=target_list[0][0];
                    target_list[1][1]=target_list[0][1];
                    target_list[1][2]=target_list[0][2];
                    
                    target_list[0][0]=i;
                    target_list[0][1]=j;
                    target_list[0][2]=distance;
                    
                }else if(distance<=target_list[1][2]){
                // 今までで2番目に近いゴールドであれば、配列の2番目以降を更新
                    target_list[2][0]=target_list[1][0];
                    target_list[2][1]=target_list[1][1];
                    target_list[2][2]=target_list[1][2];
                    
                    target_list[1][0]=i;
                    target_list[1][1]=j;
                    target_list[1][2]=distance;
                    
                }else if(distance<=target_list[2][2]){
                // 今までで3番目に近いゴールドであれば、配列の3番目のみ更新
                    target_list[2][0]=i;
                    target_list[2][1]=j;
                    target_list[2][2]=distance;
                }
            }
        }
    }
}
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include"goldrush.h"

#define SEL 'S'
#define ENE 'E'

/*
名称や数値の説明
mob  ゲームに出てくるプレイヤー
selfy  このプログラム自身が動かすプレイヤー　語感で名付けています 
enemy  このプログラムでないプログラムで動くプレイヤー
buffer  仮の入れ物
333  はじくべき数値　登場物が存在しない場合や避けるべき場合に使います
*/

//ゲームを通して変化してはいけないデータを格納
typedef struct Game_date{  
    
    char Gplayer_char;
    char Genemy_char;
    int Gcoin_sum;
    
}Game_date;

//mobのデータを格納
typedef struct Mob_date{
    
    char Mmob_char;
    int Mcoin_remain_num;
    int Mmob_place[2]; 
    int Mmob_exist;
    
    //絶対座標・コインの識別番号　[a][b] = { a = コインの優先度 , b[0] = y座標 b[1] = x座標 }
    int Mcoin_abs_place_list[N_ROW*N_COLUMN][2];
    
    //mobとコインの相対座標 [a][b] = { a = コインの優先度 , b[0] = y座標 b[1] = x座標 }
    int Mcoin_rel_place_list[N_ROW*N_COLUMN][3];
    
}Mob_date;

//-----------------------------------

struct Game_date Constructor(char player,int row,int column,int turn,const char board[N_ROW][N_COLUMN]);
struct Mob_date  InitializeM(char m,struct Game_date g);
struct Mob_date  Seach_mob_date(struct Game_date g,struct Mob_date m,const char board[N_ROW][N_COLUMN]);
struct Mob_date  Sort(struct Mob_date m);
struct Mob_date  Avoing(struct Mob_date s,struct Mob_date e);
int Determine(struct Mob_date s,struct Mob_date e);

//-------------------------------------

int move2021(char player,int row,int column,int score,int turn,const char board[N_ROW][N_COLUMN]){
    
    static Game_date game_date;
    int move_way;

    if(turn<=2){
        //初期設定
        game_date = Constructor(player,row,turn,column,board);
        
    }
    
    //mob_dateの生成と初期化
    struct Mob_date selfy = InitializeM(SEL,game_date);
    struct Mob_date enemy = InitializeM(ENE,game_date);
    
    //mobに関する情報を代入
    selfy = Seach_mob_date(game_date,selfy,board);
    enemy = Seach_mob_date(game_date,enemy,board);
    
    //相対距離が小さい順に並び変える
    enemy = Sort(enemy);
    selfy = Sort(selfy);
    
    //enemyのコインの座標をもとにした優先度の変更
    selfy = Avoing(selfy,enemy);
    
    //動く方向を決定
    move_way = Determine(selfy,enemy);
    
    return move_way;
}

//--------------------------

//Game_dateに値を代入する関数
Game_date Constructor(char player,int row,int column,int turn,const char board[N_ROW][N_COLUMN]){
    
    int i,j;
    Game_date game_date = {'A','B',0};
    
    //自身が操作するプレイヤーと相手の操作するプレイヤーを識別
    if(player == PLAYER_A){ 
        
        game_date.Gplayer_char = PLAYER_A;
        game_date.Genemy_char = PLAYER_B;
        
    }else{
        
        game_date.Gplayer_char = PLAYER_B;
        game_date.Genemy_char = PLAYER_A;
        
    }
    
    //コインの総数をカウント
    for (i=0;i<N_ROW;i++){
        for (j=0;j<N_COLUMN;j++){
            
            if(board[i][j] == GOLD){
                
                ++game_date.Gcoin_sum;
                
            }
        }
    }
    
    return game_date;
}

//Mob_dateの初期化をする関数
Mob_date InitializeM(char status ,Game_date g){
    
    int i;
    char buffer;
    
    //mobのcharを決定
    if(status == SEL){
        
        buffer = g.Gplayer_char;
        
    }else if(status == ENE){
        
        buffer = g.Genemy_char;
        
    }
    
    //Mob_dateの宣言・初期化
    Mob_date m = {buffer,0,{333,333},1};
    
    //各種配列の初期化
    for(i=0;i<=g.Gcoin_sum-1;i++){
        
        m.Mcoin_abs_place_list[i][0] = 0;
        m.Mcoin_abs_place_list[i][1] = 0;
        m.Mcoin_rel_place_list[i][0] = 0;
        m.Mcoin_rel_place_list[i][1] = 0;
        m.Mcoin_rel_place_list[i][2] = 0;
        
    }
    
    return m;
}

//mob_dateに関するデータを代入
Mob_date Seach_mob_date(Game_date g,Mob_date m,const char board[N_ROW][N_COLUMN]){
    
    int i,j,coin_index;
    coin_index = 0;
    
    //mobの場所を取得・代入
    for (i=0;i<N_ROW;i++){
        for (j=0;j<N_COLUMN;j++){
            
            if(board[i][j] == m.Mmob_char){
                
                m.Mmob_place[0] = i;
                m.Mmob_place[1] = j;
                
            }
        }
    }
    
    //mobが存在しているかどうかを判定
    if(m.Mmob_place[0] == 333) m.Mmob_exist = 0;
    
    //コインに関するデータを代入
    for (i=0;i<N_ROW;i++){
        for (j=0;j<N_COLUMN;j++){
            
            if(board[i][j] == GOLD){
                
                m.Mcoin_abs_place_list[coin_index][0] = i;
                m.Mcoin_abs_place_list[coin_index][1] = j;
                m.Mcoin_rel_place_list[coin_index][0] = i - m.Mmob_place[0];
                m.Mcoin_rel_place_list[coin_index][1] = j - m.Mmob_place[1];
                m.Mcoin_rel_place_list[coin_index][2] = 
                      abs(m.Mcoin_rel_place_list[coin_index][0])
                    + abs(m.Mcoin_rel_place_list[coin_index][1]);
                ++m.Mcoin_remain_num;
                ++coin_index;
            }
        }
    }
    
    return m;
}

//相対距離の小さい順によってコインの配列を並び替える関数
Mob_date Sort(Mob_date m){
    
    int i = 0;
    
    int roopfrag = 1;
    int times = 0;
    
    int buffer[5] = {
         m.Mcoin_abs_place_list[0][0]
        ,m.Mcoin_abs_place_list[0][1]
        ,m.Mcoin_rel_place_list[0][0]
        ,m.Mcoin_rel_place_list[0][1]
        ,m.Mcoin_rel_place_list[0][2]};
    
    while(roopfrag == 1){
        
        int count = 0;
        
        for(i=0;i<m.Mcoin_remain_num - 1;i++){
            
            //i番目の相対距離よりi+1番目の相対距離が小さいとき、それらのコインの情報を入れ替える
            if(m.Mcoin_rel_place_list[i][2]>m.Mcoin_rel_place_list[i+1][2]){
                
                buffer[0] = m.Mcoin_abs_place_list[i+1][0];
                buffer[1] = m.Mcoin_abs_place_list[i+1][1];
                buffer[2] = m.Mcoin_rel_place_list[i+1][0];
                buffer[3] = m.Mcoin_rel_place_list[i+1][1];
                buffer[4] = m.Mcoin_rel_place_list[i+1][2];
                
                m.Mcoin_abs_place_list[i+1][0] = m.Mcoin_abs_place_list[i][0];
                m.Mcoin_abs_place_list[i+1][1] = m.Mcoin_abs_place_list[i][1];
                m.Mcoin_rel_place_list[i+1][0] = m.Mcoin_rel_place_list[i][0];
                m.Mcoin_rel_place_list[i+1][1] = m.Mcoin_rel_place_list[i][1];
                m.Mcoin_rel_place_list[i+1][2] = m.Mcoin_rel_place_list[i][2];
                
                m.Mcoin_abs_place_list[i][0] = buffer[0];
                m.Mcoin_abs_place_list[i][1] = buffer[1];
                m.Mcoin_rel_place_list[i][0] = buffer[2];
                m.Mcoin_rel_place_list[i][1] = buffer[3];
                m.Mcoin_rel_place_list[i][2] = buffer[4];
            }
        }
        
        //小さい順になっているかどうか確認
        for(i=0;i<m.Mcoin_remain_num - 1;i++){
            
            if(m.Mcoin_rel_place_list[i][2]<=m.Mcoin_rel_place_list[i+1][2]){ ++count;}
        }
        
        ++times;
        
        //小さい順になっていたらループを終わる
        if(count >= m.Mcoin_remain_num - 1){ roopfrag = 0; }
        
        //安全装置　無限ループにことを防ぐ
        if(times >= 500){
            break;
        }
    }
    
    return m;
    
}

//自分の最も近いコインに敵の方が早く近づく場合、そのコインを回避する関数
Mob_date Avoing(Mob_date s,Mob_date e){
    
    int i;
    
    int buffer1[3] = {0,0,0};
    int buffer2[3] = {0,0,0};
   
    int index = 0;
    
    //初期値を代入
    buffer1[0] = e.Mcoin_abs_place_list[0][0];
    buffer1[1] = e.Mcoin_abs_place_list[0][1];
    buffer1[2] = e.Mcoin_rel_place_list[0][2];
    
    //自分の最も近いコインに敵の方が早く近づくそのコインの情報を取得
    for(i=0;i<s.Mcoin_remain_num-1;i++){
        
        if(buffer1[0] == s.Mcoin_abs_place_list[i][0] && 
            buffer1[1] == s.Mcoin_abs_place_list[i][1]){
            
            buffer2[0] = s.Mcoin_abs_place_list[i][0];
            buffer2[1] = s.Mcoin_abs_place_list[i][1];
            buffer2[2] = s.Mcoin_rel_place_list[i][2];
            
            index = i;
        }
    }
    
    //回避すべきコインを除外
    if(buffer1[2] < buffer2[2] && index == 0){
        
        s.Mcoin_rel_place_list[0][2] = 333;
        
        s = Sort(s);
    }
    
    return s;
    
}

//最終的に動く方向を決める関数

/*
ポイント制になっており、最もポイントが高い方向に動く
phase1
-動いてはいけない方向へ減点、動いてもよい方向に加点する
phase2
-最も近いコインへ移動できる方向へ加点、そうでない場合は減点する
phase3　enemyよりselfyが上にいた方が有利という考えをもとに設定されている
-phase2での同じ点数になる時のための調整配点
-enemyが自分よりも下にいる場合左右方向への移動に加点
-enemyが自分以上の高さにいる場合上方向への移動に加点
*/

int Determine(Mob_date s,Mob_date e){
    
    int i;
    int way_move = UP;
    
    //敵が存在していた場合
    if(e.Mmob_exist == 1){
        
        //評価するためのデータを格納
        int ban_list[3] = {0,0,0}; 
        //それぞれの方向の点数として宣言
        int po_up = 0;
        int po_left = 0;
        int po_right = 0;
        
        //自分と敵との距離と自分のx座標を保存
        //[0] = {y軸上でのenemyとselfyとの相対距離}
        //[1] = {x軸上でのenemyとselfyとの相対距離}
        //[2] = {selfyのx座標}
        ban_list[0] = e.Mmob_place[0] - s.Mmob_place[0];
        ban_list[1] = e.Mmob_place[1] - s.Mmob_place[1];
        ban_list[2] = s.Mmob_place[1]; 
        
        //phase1
        
        if(ban_list[0] == 1 && ban_list[1] == 0) po_up = -7;
            else po_up = 7;
        
        if((ban_list[1] ==-1 && ban_list[0] == 0) || ban_list[2] == 0) po_left = -7;
            else po_left = 7;
        
        if((ban_list[1] == 1 && ban_list[0] == 0) || ban_list[2] == N_COLUMN-1) po_right = -7;
            else po_right = 7;
       
        //phase2
        
        if(s.Mcoin_rel_place_list[0][0] != 0) po_up += 3;
            else po_up += -3;
        
        if(s.Mcoin_rel_place_list[0][1] < 0) po_left += 3;
            else po_left += -3;
        
        if(s.Mcoin_rel_place_list[0][1] > 0) po_right += 3;
            else po_right += -3;
        
        //phase3
        
        if(ban_list[0] >= 0) po_up += 2;
        if(ban_list[0] <  0){++po_left; ++po_right;}
        
        
        int po_list[3] = {po_up,po_left,po_right};
        int buffer1 = 0;
        int buffer2 = po_list[0];
        
        //最大値を検索
        for(i=1;i<3;i++){
            
            if(buffer2 < po_list[i]){
                
                buffer1 = i;
                buffer2 = po_list[i];
                
            }
            
        }
        
        way_move = buffer1;
        
    }
    //敵が存在していなかった場合　すべてのコインを回収する
    else{
        
        for(i=0;i<s.Mcoin_remain_num-1;i++){
            
            //selfyがいる行にコインがあった場合にそのコインに向かって動く
            if(s.Mcoin_abs_place_list[i][0] == s.Mmob_place[0]){
                
                if(s.Mcoin_rel_place_list[i][1] > 0){
                    way_move = RIGHT;
                }else if(s.Mcoin_rel_place_list[i][1] < 0){
                    way_move = LEFT;
                }
            }
            //selfyがいる行にコインがなかった場合上に向かって動く
        }
    }
    
    return way_move;

}

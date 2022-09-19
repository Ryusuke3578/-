#include <stdio.h>
#include <stdlib.h>

/* コマンドの最大長 */
#define MAX_COMMAND 10

/* 入力コマンドを保持 */



/*
  マップのパターン
  8 9 a b
  4 5 6 7
  0 1 2 3
*/

/*
  マップの水平方向と垂直方向のサイズ
*/
#define MAP_V 3
#define MAP_H 4

/*
  パス毎の情報
*/
typedef struct {
  char v; /* 垂直方向の移動量 */
  char h; /* 水平方向の移動量 */
  char start_heading; /* 開始時の向き */
  char end_heading; /* 終了時の向き */
} PATH;

/*
  パスを記憶する変数
*/
PATH path[MAX_COMMAND * 4];

/*
  向きを示すマイクロ
*/
#define UPWARD 0
#define DOWNWORD 1
#define RIGHTWARD 2
#define LEFTWARD 3

/*
  現在の向きから次の向きへ移行するためのコマンドのテーブル
  （ToDo） 表を埋める
*/
const char command_matrix[4][4] = {
  {'f', 'b', 'r', 'l'}, /* UPWARDから UPWARD, DOWNWORD, RIGHTWARD, LEFTWARD に */
  {'b', 'f', 'l', 'r'}, /* DOWNWARDから UPWARD, DOWNWORD, RIGHTWARD, LEFTWARD に */
  {'l', 'r', 'f', 'b'}, /* RIGHTWARDから UPWARD, DOWNWORD, RIGHTWARD, LEFTWARD に */
  {'r', 'l', 'b', 'f'} /* LEFTWARDから UPWARD, DOWNWORD, RIGHTWARD, LEFTWARD に */
};

void genCommand(void) {
  int cnt = 0;
  int src, dest;
  int last_heading, new_heading;
  int command_cnt = 0;
  int v_abs, h_abs;
  int path_cnt;

  //数値に変換 . はそのまま(46)
  cnt = 0;

  do {
    if ((scommand[cnt] >= '0') && (scommand[cnt] <= '9')) {
      scommand[cnt] = scommand[cnt] - '0';
    }
    else {
      scommand[cnt] = scommand[cnt] - 'a' + 10;
    }
  } while (scommand[++cnt] != '.');

  /* 各パスの移動量を計算 */
  dest = 0; /* スタート地点は(0) */

  path_cnt = 0;
  while (1) {
    src = dest;
    dest = scommand[path_cnt];
    if (dest == '.') break;
    if (src == dest) continue;
    // （ToDo） 縦横の移動量を計算
    path[path_cnt].v = (dest / 4) - (src / 4);
    path[path_cnt].h = (dest % 4) - (src % 4);
    path_cnt++;
  }

  /* 各移動毎のスタート時に向く方向と目的交差点到着時に向いている方向を計算 */
  /* 上下移動 -> 左右移動 とする．*/
  for (cnt = 0; cnt < path_cnt; cnt++) {
    /* スタート時の向きを決定 */
    if (path[cnt].v != 0) {
      /* 上下方向の移動があれば上下方向の移動で向きが決まる */
      // （ToDo） 向きを決定する．UPWARD か DOWNWORD を代入
      if (path[cnt].v > 0 ) {
        path[cnt].start_heading = UPWARD;
      }
      else {
        path[cnt].start_heading = DOWNWORD;
      }
    }
    else {
      /* 上下方向の移動がなければ左右方向の移動で向きが決まる */
      // （ToDo） 向きを決定する．
      if (path[cnt].h > 0) {
        path[cnt].start_heading = RIGHTWARD ;
      } else {
        path[cnt].start_heading = LEFTWARD ;
      }
    }

    /* 交差点到着時の向きを決定 */
    if (path[cnt].h != 0) {
      /* 左右方向の移動があれば左右方向の移動で向きが決まる．*/
      // （ToDo） 向きを決定する．
      if (path[cnt].h > 0) {
        path[cnt].end_heading = RIGHTWARD;
      } else {
        path[cnt].end_heading = LEFTWARD;
      }
    }
    else {
      /* 左右方向の移動がなければ上下方向の移動で向きが決まる．*/
      // （ToDo） 向きを決定する．
      if (path[cnt].v > 0 ) {
        path[cnt].start_heading = UPWARD;
      }
      else {
        path[cnt].start_heading = DOWNWORD;
      }
    }
  }

  /* 各パスを実現するためのコマンドを生成 */
  new_heading = UPWARD; /* 初期値はUPWARD */
  for (cnt = 0; cnt < path_cnt; cnt++) {

    /* パス開始時の向き */
    last_heading = new_heading;
    /* パスでの開始時の向き */
    new_heading = path[cnt].start_heading;
    /* コマンドを取得 */
    command[command_cnt++] = command_matrix[last_heading][new_heading];

    /* 垂直方向と水平方向の移動量の絶対値を取得 */
    v_abs = abs(path[cnt].v);
    // （ToDo） 水平方向の絶対値を取得する
    h_abs = abs(path[cnt].h);

    /* 垂直方向の移動のためのコマンドを生成 */
    if (v_abs != 0) {
      /* 1ブロック分はターンのためのコマンドで進むため削除 */
      v_abs--;
      /* 垂直方向の移動分'f'コマンドを追加する */
      while (v_abs != 0) {
        // （ToDo） コマンドの追加
        scommand[command_cnt++] = 'f';
        v_abs--;
      }

      /* 水平方向への移動のためのコマンドを追加 */
      if (h_abs != 0) {
        /* 垂直方向と水平方向の移動の向きによりコマンドを選択 */
        // （ToDo） command_matrixを使用
        command[command_cnt++] = command_matrix[new_heading][path[cnt].end_heading];
      }
    }

    /* 水平方向の移動のためのコマンドを生成 */
    if (h_abs != 0) {
      /* 1ブロック分はターンのためのコマンドで進むため削除 */
      h_abs--;
      /* 水平方向の移動分'f'コマンドを追加する */
      while (h_abs != 0) {
        // （ToDo） コマンドの追加
        command[command_cnt++] = 'f';
        h_abs--;
      }
    }

    /* 今回のパスの最終的な向きを次のパスの開始時の向きとする */
    new_heading = path[cnt].end_heading;
  }

  /* 終了コマンドを追加 */
  command[command_cnt] = '.';

  for (int t = 0; t < MAX_COMMAND; t++) {
    Serial.print(command[t]);

  }
  Serial.println();
}

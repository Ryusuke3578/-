#include <Wire.h>
#include <ZumoShieldN.h>

/* コマンドの最大長 */
#define MAX_COMMAND  10
/* コマンドを入力するバッファ */
char scommand[MAX_COMMAND];
char command[MAX_COMMAND * 2];


void setup() {
  /* シリアルモニタ初期化 */
  Serial.begin(9600);

  /* コマンド入力モードへ */
  getCommand();
  genCommand();
  /* プッシュボタンが押されるのを待つ */
  Serial.println("Pusy button to start.");
  button.waitForPress();


  /* ブザーを鳴らす */
  buzzer.playNum(1);
}



/*
   コマンド入力モードの関数
*/
void getCommand(void) {
  char c;      /* 読み込んだ文字を入れる変数 */
  int cnt = 0; /* 読み込んだ文字数を入れる変数 */
  int i;
  /* ブザーを1回鳴らす */
  buzzer.playOn();

  /* シリアルモニタに入力が可能となった事を示す文字列を出力 */
  Serial.print("Input command.");
  Serial.println();

  /* 無限ループ */
  while (1) {
    /* シリアルモニタから入力を受け付ける */
    if (!(Serial.available() > 0)) {
      continue; /* while(1)の最初から実行 */
    }
    /* 文字の表示 */
    //Serial.print(cnt) / printf("%d",command[cnt]);
    /* 文字を読み込み */
    c = Serial.read();
    /* 必要な処理を記載する */
    if ((('a' <= c) && (c <= 'z')) || ('0' <= c && c <= '9') || ('A' <= c) && (c <= 'Z')) {
      scommand[cnt] = c;
      cnt = cnt + 1;
    } else if (c ==   '.') {
      scommand[cnt] = c;

      break;
    } else if (c == '*' && (0 < cnt && cnt < 9)) {
      Serial.println();
      scommand[cnt - 1] = '\0';
      cnt = cnt - 1;
    } else if (c == '\n') {
    } else {
      Serial.println();
      Serial.println("エラー");
    }

    if ((MAX_COMMAND) - 1 < cnt) {
      Serial.println();
      Serial.println("reset");
      cnt = 0;
      Serial.print("Input command.");
      Serial.println();
    }
  }

  for (int i = 0; i < MAX_COMMAND; i++) {
    Serial.print(scommand[i]);

  }
  Serial.println();
}
void loop() {
  /* 運行モードへ */
  doOperation();

}

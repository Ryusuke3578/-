#define MOTOR_SPEED 140  /* モータの速度 */
#define TURN_SPEED  140  /* ターンの速度 */
#define REFLECTANCE_THRESHOLD  600  /* リフレクタセンサーの境界値 */
#define INTERSECTION_OSTIM     240 /* 交差点の中心までに進むまでの時間 */

/* リフレクタセンターが黒線上にあるかの判断 */
#define ON_LINE(val)  ((val) > REFLECTANCE_THRESHOLD)

/*
   運行モードの関数
*/
void doOperation(void) {
  int command_cnt = 0; /* 実行中のコマンドの配列番号 */
  char cmd ; /* 実行中のコマンド */

  /* 最初の交差点まで直進 */
  goStraight();

  while (1) {
    /* 実行するコマンドを取り出し */
    cmd = command[command_cnt];
    Serial.println(cmd);
    /* コマンド処理 */
    if (cmd == '.') {
      /* 停止コマンド */
      /* 停止時の処理を記載 */
      motors.setSpeeds(0, 0);
      buzzer.playOn();
      while (1) {
        led.on();
        delay(1000);
        led.off();
        delay(1000);
      }

    }
    else if ((cmd == 'r') || (cmd == 'l') || (cmd == 'b') ) {
      /* 左右へのターンコマンド */
      doTurn(cmd);
      /* 交差点まで直進 */
      goStraight();
    }
    else {
      /* 直進コマンド('f') */
      goStraight();
    }
    /* 次に読み込むコマンドの配列番号 */
    command_cnt = command_cnt + 1;
  }
}

/*
   交差点まで直進する関数
*/
void goStraight(void) {
  /* 交差点まで直進する処理を記載 */

  while (1) {
    reflectances.update();
    if (reflectances.value(2) > REFLECTANCE_THRESHOLD) {
      motors.setSpeeds(-MOTOR_SPEED, MOTOR_SPEED);
    } else if (reflectances.value(5) > REFLECTANCE_THRESHOLD) {
      motors.setSpeeds(MOTOR_SPEED, -MOTOR_SPEED);
    } else {
      motors.setSpeeds(MOTOR_SPEED, MOTOR_SPEED);
    }
    reflectances.update();
    if ((reflectances.value(1) > REFLECTANCE_THRESHOLD) || (reflectances.value(6) > REFLECTANCE_THRESHOLD)) {
      motors.setSpeeds(0, 0);
      delay(1000);
      break;
    }
  }


  /* 交差点の中心まで進む */
  motors.setSpeeds(MOTOR_SPEED, MOTOR_SPEED);
  delay(INTERSECTION_OSTIM);
  motors.setSpeeds(0, 0);
}

/*
   指定された方向へターンする関数*/

void doTurn(char dir) {
  int count = 0;
  int pstate;
  /* リフレクタセンタの更新 */
  reflectances.update();
  imu.begin();
  imu.configureForTurnSensing();
  imu.turnSensorReset();


  switch (dir) {
    case 'r':
      while (imu.turnSensorAngleDegree() >= -90) {
        motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
        imu.turnSensorUpdate();
      }
      motors.setSpeeds(0, 0);
      break;

    /*buzzer.playNum(1);*/
    case 'l':
      /* 左回りの処理を記載 */
      while (imu.turnSensorAngleDegree() <= 90) {
        motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
        imu.turnSensorUpdate();
      }
      motors.setSpeeds(0, 0);
      break;
    case 'b':
      while (imu.turnSensorAngleDegree() <= 90) {
        motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
        imu.turnSensorUpdate();
      }
      imu.turnSensorReset();
      while (imu.turnSensorAngleDegree() <= 90) {
        motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
        imu.turnSensorUpdate();
      }
      motors.setSpeeds(0, 0);
      break;
  }
}

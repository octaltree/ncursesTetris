//描画とあたり判定などゲームの制御

#include"mino.h"
#pragma once

class tetris{
  private:
    int nmino[21][10];

    void start();
    int instruct();
    int gameframe();// -1:強制終了, 0:ゲームクリア, 1:ゲームオーバー
      //int inputkey(char);
      //int game();
      int deleteline();//消した本数を返す
      bool judgeclear();
    bool CLEAR();
    bool GAMEOVER();

    void showboard();


  public:
    void Main();

};

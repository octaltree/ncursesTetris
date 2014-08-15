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
    bool CLEAR();
    bool GAMEOVER();

    int deleteline();
    void showboard();


  public:
    void Main();

};

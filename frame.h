//描画とあたり判定などゲームの制御

#include"mino.h"
#pragma once

class tetris{
  private:
    int nmino[20][10];

    void start();
    int instruct();
    int game();// -1:強制終了, 0:ゲームクリア, 1:ゲームオーバー
      //void inputkey(char);
    bool CLEAR();
    bool GAMEOVER();

    //bool deleteline(status s);
    void showboard();


  public:
    void Main();

};

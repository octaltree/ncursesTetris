//描画とあたり判定などゲームの制御

#include"mino.h"
#pragma once

class tetris{
  private:
    int nmino[20][10];

    void start();
    int instruct();
    int gameframe();// -1:強制終了, 0:ゲームクリア, 1:ゲームオーバー
      int inputkey(char, mino);
      //int game();
      int deleteline();//消した本数を返す
      bool judgeclear();
      int getrandomtype();
    bool CLEAR();
    bool GAMEOVER();

    void clearnmino();
    bool enabletomove(mino);

    void showboard();


  public:
    void Main();

};

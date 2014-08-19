//描画とあたり判定などゲームの制御

#include"mino.h"
#pragma once

class tetris{
  private:
    mino stock;
    int nmino[20][10];

    void start();
    int instruct();
    int gameframe();// -1:強制終了, 0:ゲームクリア, 1:ゲームオーバー
      int inputkey(char, queue<mino>&);
      //int game();
      int deleteline();//消した本数を返す
      bool judgeclear();
      int getrandomtype();
      void clearnmino();
      bool enabletomove(mino);
      void showboard();
      void quepush(queue<mino>&);
      //void stock(queue<mino>&);
      //void release(queue<mino>&);
    bool CLEAR();
    bool GAMEOVER();



  public:
    void Main();

};

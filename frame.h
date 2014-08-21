//描画とあたり判定などゲームの制御

#pragma once
#include<queue>
#include<ncurses.h>
#include<string>
#include<random>
#include<chrono>
#include"mino.h"

class tetris{
  private:
    mino stocked;
    int nmino[20][10];

    void start();
    int instruct();
    int gameframe();// -1:強制終了, 1:ゲームクリア, 0:ゲームオーバー
      int inputkey(char, std::queue<mino>&);
      //int game();
      int deleteline();//消した本数を返す
      bool judgeclear();
      int getrandomtype();
      void clearnmino();
      bool enabletomove(mino);
      void showboard();
      void quepush(std::queue<mino>&);
      void showque(std::queue<mino>);
      void stock(std::queue<mino>&);
      void release(std::queue<mino>&);
      void fall(std::queue<mino>&);
      bool fall_enable(std::queue<mino>&);
    bool CLEAR();
    bool GAMEOVER();

  public:
    void Main();

};

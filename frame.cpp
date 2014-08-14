#include"frame.h"
#include<ncurses.h>
#include<string>
#include<random>

void tetris::start(){
  erase();
  attrset(COLOR_PAIR(0));
  mvaddstr(15, 3, "T");
  mvaddstr(15, 6, "E");
  mvaddstr(15, 9, "T");
  mvaddstr(15, 12, "R");
  mvaddstr(15, 15, "i");
  mvaddstr(15, 18, "S");
  attrset(COLOR_PAIR(0));
  mvaddstr(35, 1, "--- put any key ---");

  showboard();

  move(0, 0);
  refresh();
  timeout(-1);
  getch();
}


int tetris::instruct(){
  erase();
  int xmax, ymax;
  getmaxyx(stdscr, ymax, xmax);

  attrset(COLOR_PAIR(0));
  mvaddstr(3, 5, "全消しでクリア");
  mvaddstr(7, 0,"   k  :ブロックを回転");
  mvaddstr(9, 0,"   h  :ブロックを左に移動");
  mvaddstr(12, 0,"   l  :ブロックを右に移動");
  mvaddstr(15, 0,"   j  :ブロックを下に移動");
  mvaddstr(18, 0,"Space :ブロックをストック/リリース");
  mvaddstr(21, 0,"   f  :ブロックを落下");
  mvaddstr(24, 0,"q/Esc :ゲームを終了");
  mvaddstr(35, 1, "--- put any key ---");

  move(0, 0);
  refresh();
  timeout(-1);
  int flg = getch();
  if( flg == (int)'q' || flg == 27 )
    return -1;
  else
    return 0;
}


void tetris::Main(){
  bool end = 0;
  setlocale(LC_ALL,"");
  initscr();
  noecho();//keyのinputを画面に表示しない
  cbreak();//Enterなしでinput
  start_color();

  start();
  if( instruct() == -1 ) end = true;


  int flag = -1;
  bool replay = false;

  //if( !end )
  //int flag = game();// -1, 0, 1で返す

  //test
  flag = 0;
  //test

  if( flag == 1 )
    replay = CLEAR();
  else if( !flag )
    replay = GAMEOVER();

  endwin();

  if( replay ) Main();
}

int game(){
  erase();
  timeout(100);// 100ミリ秒でgetchの入力待ちを終わらせる
  //最遅10fps


  long long int frame = 0;//overflow
  int in;
  while(1){
    in = getch();//timeoutした場合-1を返す
    if( in == (int)'q' || in == 27 ) return -1;
    //                 || ESCが押されたとき


    // do something here


    mvprintw(0, 1, "frame : %d", frame++);
    move(0, 0);
    refresh();
  }

}

/*
void tetris::inputkey(char in, mino block){
  switch(in){
    case (int)'h': 
      block.x--;
    break;
    case (int)'j': 
      block.y--; 
    break;
    case (int)'l': 
      block.x++; 
    break;
    case (int)'k': 
      block.rotation++; 
    break;
    defalt: break;
  }
}
*/

void tetris::showboard(){
  attrset(COLOR_PAIR(0));
  for(int i = 10; i < 30; i++){
    mvaddstr(i, 5, "|");
    mvaddstr(i, 16, "|");
  }
  for(int j = 5; j < 17; j++)
    mvaddstr(30, j, "-");

  move(0, 0);
  refresh();
}

bool tetris::CLEAR(){
  /* *
  erase();
  mvaddstr(0, 1,"debug:gameclear");
  move(0, 0);
  refresh();
  timeout(-1);
  getch();
  return false;
  /*/

  erase();
  int xmax, ymax;
  getmaxyx(stdscr,ymax,xmax);
  mvaddstr(10, 2,"C o n g r a t u l a t i o n s");

  //string esc = "one more time? (y/n)";
  //mvprintw(ymax, (xmax-esc.length()-4)/2, "%s", esc.c_str());
  mvaddstr(35, 1, "one more time? (y/n)");
  move(0, 0);
  refresh();
  timeout(-1);
  int flag = getch();
  while( flag != (int)'y' && flag != (int)'Y' && flag != (int)'n' && flag != (int)'N' && flag != (int)'q' && flag != 27 ){
    flag = getch();
  }
  if( flag == (int)'y' || flag == (int)'Y' )
    return true;
  else if( flag == (int)'n' || flag == (int)'N'|| flag == 27 || flag == (int)'q' )
    return false;
  /* */
}

bool tetris::GAMEOVER(){
  /* *
  erase();
  mvaddstr(0, 1,"debug:gameover");
  move(0, 0);
  refresh();
  timeout(-1);
  getch();
  return false;
  /*/

  erase();
  int xmax, ymax;
  getmaxyx(stdscr,ymax,xmax);
  mvaddstr(10, 1,"G A M E O V E R");

  //string esc;
  //esc = "one more time? (y/n)";
  //mvprintw(ymax, (xmax-esc.length()-4)/2, "%s", esc.c_str());
  mvaddstr(35, 2, "one more time? (y/n)");
  move(0, 0);
  refresh();
  timeout(-1);
  int flag = getch();
  while( flag != (int)'y' && flag != (int)'Y' && flag != (int)'n' && flag != (int)'N' && flag != (int)'q' && flag != 27 ){
    flag = getch();
  }
  if( flag == (int)'y' || flag == (int)'Y' )
    return true;
  else if( flag == (int)'n' || flag == (int)'N'|| flag == 27 || flag == (int)'q' )
    return false;
  /* */
}

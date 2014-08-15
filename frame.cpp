#include"frame.h"
#include<ncurses.h>
#include<string>
#include<random>
#include<chrono>

using namespace std::chrono;

void tetris::Main(){
  bool end = 0;
  setlocale(LC_ALL,"");//全角文字に対応するおまじない
  initscr();
  noecho();//keyのinputを画面に表示しない
  cbreak();//Enterなしでinput

  start_color();
  for(int f = 0; f < 8; f++)
    for(int b = 0; b < 8; b++)
      init_pair(f*10+b, f, b);
 

  start();
  if( instruct() == -1 ) end = true;


  int flag = -1;
  bool replay = false;

  if( !end )
    flag = gameframe();// -1, 0, 1で返す

  if( flag == -1 ) end = true;


  //debug
  flag = 0;
  //debug


  if( flag == 1 )
    replay = CLEAR();
  else if( flag == 0 )
    replay = GAMEOVER();

  endwin();

  if( replay ) Main();
}

int tetris::gameframe(){
  for(int l = 0; l < 20; l++)
    for(int r = 0; r < 10; r++)
      nmino[l][r] = 0;

  erase();
  timeout(10);//10ミリ秒

  unsigned long long int frame = 0;
  int in;
  bool nino = false;
  mino block;

  while(1){
    system_clock::time_point begin = system_clock::now();
    milliseconds ms(17);

    in = getch();//timeoutした場合-1を返す
    if( in == (int)'q' || in == 27 ) return -1;
    //                 || ESCが押されたとき

    //ゲーム処理のメイン////////////////////
    if( !nino){ 
      block.center.x = 0;
      block.center.y = 0;
      block.rotate = 0;
      block.type = getrandomtype();
    }//初期化
    inputkey(in, block);

    if( !nino) 
    //inputkey(in);
    int dl = deleteline();
    if( dl > 0 && judgeclear()) return 1;

    //ゲーム処理のメイン////////////////////
    //初期化して
    //動かして
    //終了条件照らしあわせて
    //描画する

    mvprintw(0, 1, "frame : %d", ++frame);
    move(0, 0);
    refresh();

    for(unsigned long long int weight = 0; weight < (1 << 30); weight++)
        if( time_point_cast<milliseconds>(system_clock::now()) - time_point_cast<milliseconds>(begin) > ms )
          break;
  }
}

int tetris::inputkey(char in, mino block){
  switch(in){
    case (int)'h': 
      block.center.x--;
    break;
    case (int)'j': 
      block.center.y++; 
    break;
    case (int)'l': 
      block.center.x++; 
    break;
    case (int)'k': 
      block.rotate++; 
    break;
    case (int)'f':
      //落下操作
    break;
    case 32:
      //spaceが押された時ホールド/リリース
    break;
    defalt: 

    break;
  }

  return 0;
}

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
  attrset(COLOR_PAIR(10));
  mvaddstr(10, 5,"G A M E O V E R");

  //string esc;
  //esc = "one more time? (y/n)";
  //mvprintw(ymax, (xmax-esc.length()-4)/2, "%s", esc.c_str());
  attrset(COLOR_PAIR(0));
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
  mvaddstr(7, 0, "   k  :ブロックを回転");
  mvaddstr(9, 0, "   h  :ブロックを左に移動");
  mvaddstr(12, 0, "   l  :ブロックを右に移動");
  mvaddstr(15, 0, "   j  :ブロックを下に移動");
  mvaddstr(18, 0, "Space :ブロックをストック/リリース");
  mvaddstr(21, 0, "   f  :ブロックを落下");
  mvaddstr(24, 0, "q/Esc :ゲームを終了");
  mvaddstr(28, 0, "ゲームが長引くとframe数のカウントがoverflow");
  mvaddstr(29, 0, "(ゲームの進行に影響はない)");
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

int tetris::deleteline(){
  int dlcount = 0;//消える段数
  for(int i = 19; i >= 0; i--){
    if( nmino[i][0] && nmino[i][1] && nmino[i][2] && nmino[i][3] && nmino[i][4] && nmino[i][5] && nmino[i][6] && nmino[i][7] && nmino[i][8] && nmino[i][9] ){
      dlcount++;
      for(int j = 0; j < 10; j++) nmino[i][j] = -1;
    }
  }//消すところに とりま−1を入れとく

  int tmp[20][10];
  int pushline = 19;//tmpの何段目に入れるか
  for(int m = 0; m < 20; m++)
    for(int n = 0; n < 10; n++)
      tmp[m][n] = 0;//初期化

  for(int l = 19; l >= 0; l--){
    if( nmino[l][0] != -1 ){
      for(int o = 0; o < 10; o++){
        tmp[pushline][o] = nmino[l][o];
      }
      pushline--;
    }
  }

  for(int m = 0; m < 20; m++)
    for(int n = 0; n < 10; n++)
      nmino[m][n] = tmp[m][n];

  return dlcount;
}

bool tetris::judgeclear(){
  bool clear = true;
  for(int l = 0; l  < 20; l++){
    for(int r = 0; r < 10; r++){
      if( nmino[l][r] != 0 ) clear *= 0;
    }
  }
  
  return clear;
}

int tetris::getrandomtype(){
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<int> minotype(1, 7);
  return minotype(mt);
}

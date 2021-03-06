#include"frame.h"

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
 

  clearnmino();
  start();
  if( instruct() == -1 ) end = true;


  int flag = -1;
  bool replay = false;

  if( !end )
    flag = gameframe();// -1, 0, 1で返す

  if( flag == -1 ) end = true;


  if( flag == 1 )
    replay = CLEAR();
  else if( flag == 0 )
    replay = GAMEOVER();

  endwin();

  if( replay ) Main();
}

int tetris::gameframe(){
  clearnmino();
  stocked.type = 0;
  erase();
  timeout(10);//10ミリ秒

  std::queue<mino> que;
  unsigned long long int frame = 0;
  int in, frameperfall = 10;
  bool nino = false;//固定フラグ
  for(int i = 0; i < 4; i++) quepush(que);

  while(1){
    system_clock::time_point begin = system_clock::now();
    milliseconds ms(17);

    in = getch();//timeoutした場合-1を返す
    if( in == (int)'q' || in == 27 ) return -1;
    //                 || ESCが押されたとき
    //ゲーム処理のメイン////////////////////
    inputkey(in, que);
    //キー入力

    int dl = deleteline();
    if( dl > 0 && judgeclear()) return 1;//clear
    if( frame % frameperfall == 0 ){
      que.front().center.y++;
      if( !enabletomove(que.front()) ){
        nino = true;
        que.front().center.y--;
      }
    }
    if( nino ){
      que.front().center.y++;
      if( enabletomove(que.front()) ) nino = false;
      que.front().center.y--;
    }
    if( nino == true && frame % frameperfall == frameperfall - 1 ){
      nino = false;
      que.front().center.y++;
      if( que.front().center.y == 1 && que.front().center.x == 4 ) return 0;
      //nminoに固定する作業
      que.front().center.y--;
      nmino[que.front().center.y][que.front().center.x] = que.front().type;
      coordinate rest[3];
      que.front().getrestblock(rest);
      for(int i = 0; i < 3; i++){
        nmino[rest[i].y][rest[i].x] = que.front().type;
      }
      que.pop();
      quepush(que);
    }
    //ここまで終了条件とmino固定条件を見た

    
    //色が残るので消す
    attrset(COLOR_PAIR(0));
    for(int i = 5; i < 16; i++)
      mvaddstr(9, i, " ");
    //色が残るので消す
    
    showboard();
    attrset(COLOR_PAIR(que.front().type*10+que.front().type));
    mvaddstr(que.front().center.y + 10, que.front().center.x + 6, " ");
    coordinate out[3];
    que.front().getrestblock(out);
    for(int j = 0; j < 3; j++)
      mvaddstr(out[j].y + 10, out[j].x + 6, " ");
    showque(que);
    showstocked();
    //ここまで描画



    //ゲーム処理のメイン////////////////////
    //キー読んで
    //終了条件見て
    //処理して
    //描画する

    attrset(COLOR_PAIR(0));
    mvprintw(0, 1, "frame : %d", ++frame);
    move(0, 0);
    refresh();

    
    for(unsigned long long int weight = 0; weight < (1 << 30); weight++){
      for(unsigned long long int weight2 = 0; weight < (1 << 30); weight2++){
        if( time_point_cast<milliseconds>(system_clock::now()) - time_point_cast<milliseconds>(begin) > ms )
          break;
      }
      if( time_point_cast<milliseconds>(system_clock::now()) - time_point_cast<milliseconds>(begin) > ms )
        break;
    }
  }
}

void tetris::quepush(std::queue<mino>& que){
  mino fourth;
  fourth.center.x = 4; fourth.center.y = 0;
  fourth.rotate = 0; fourth.type = getrandomtype();
  que.push(fourth);
}

void tetris::showque(std::queue<mino> que){
  mino minoarr[4];
  for(int i = 0; i < 4; i++){
    minoarr[i] = que.front();
    que.pop();
  }//準備
  attrset(COLOR_PAIR(0));
  for(int i = 10; i < 30; i++){
    for(int j = 17; j < 25; j++){
      mvaddstr(i, j, " ");
    }
  }//掃除
  for(int j = 1; j < 4; j++){
    int type = minoarr[j].type;
    coordinate out[3];
    minoarr[j].getrestblock(out);
    attrset(COLOR_PAIR(type*10+type));
    mvaddstr(minoarr[j].center.y + 8 + 5*j, minoarr[j].center.x + 19, " ");
    for(int k = 0; k < 3; k++)
      mvaddstr(out[k].y + 8 + 5 * j, out[k].x + 19, " ");
  }
}

int tetris::inputkey(char in, std::queue<mino>& que){
  switch(in){
    case (int)'h': 
      que.front().center.x--;
      if( !enabletomove(que.front()) )
        que.front().center.x++;
    break;
    case (int)'j': 
      que.front().center.y++; 
      if( !enabletomove(que.front()) )
        que.front().center.y--; 
    break;
    case (int)'l': 
      que.front().center.x++;
      if( !enabletomove(que.front()) )
        que.front().center.x--; 
    break;
    case (int)'k': 
      que.front().rotate++;
      if( !enabletomove(que.front()) ){
        que.front().center.x++;//インクリ方向で試して
        if( !enabletomove(que.front()) ){
          que.front().center.x -= 2;//だめならデクリ方向で試す
          if( !enabletomove(que.front()) ){
            que.front().center.x++;//だめなら諦める
            que.front().rotate--;
          }
        }
      }
    break;
    case (int)'f':
      //落下操作
      fall(que);
    break;
    case 32:
      //spaceが押された時ホールド/リリース
      if( !stocked.type ) stock(que);
      else release(que);
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

  for(int l = 0; l < 20; l++){
    for(int r = 0; r < 10; r++){
      switch(nmino[l][r]){
        case 0: attrset(COLOR_PAIR(0)); break;
        case 1: attrset(COLOR_PAIR(11)); break;
        case 2: attrset(COLOR_PAIR(22)); break;
        case 3: attrset(COLOR_PAIR(33)); break;
        case 4: attrset(COLOR_PAIR(44)); break;
        case 5: attrset(COLOR_PAIR(55)); break;
        case 6: attrset(COLOR_PAIR(66)); break;
        case 7: attrset(COLOR_PAIR(77)); break;
      }
      mvaddstr(10+l, 6+r, " ");
    }
  }

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

  for(int l = 4; l < 8; l++) nmino[l][9] = 6;
  for(int j = 17; j < 20; j++) nmino[j][0] = 7; 
  nmino[19][1] = 7; nmino[18][1] = 1; nmino[18][2] = 1;
  nmino[19][2] = 1; nmino[19][3] = 1; nmino[18][4] = 4;
  for(int b = 4; b < 8; b++) nmino[19][b] = 4;
  for(int i = 17; i < 20; i++) nmino[i][8] = 5;
  nmino[18][7] = 5; nmino[18][6] = 1; nmino[17][6] = 1;
  nmino[17][7] = 1; nmino[16][7] = 1; nmino[16][6] = 4;
  for(int x = 16; x < 19; x++) nmino[x][5] = 4;
  nmino[17][1] = 2; nmino[16][1] = 2; nmino[16][0] = 2;
  nmino[15][0] = 2; nmino[18][3] = 5;
  for(int z = 2; z <= 4; z++) nmino[17][z] = 5;
  nmino[16][2] = 3; nmino[16][3] = 3; nmino[15][2] = 3;
  nmino[15][3] = 3; nmino[16][4] = 7;
  for(int c = 4; c <= 6; c++) nmino[15][c] = 7;
  nmino[15][8] = 2; nmino[16][8] = 2; nmino[15][7] = 2;
  nmino[14][7] = 2;

  showboard();
  attrset(COLOR_PAIR(0));
  mvaddstr(15, 3, "T");
  mvaddstr(15, 6, "E");
  mvaddstr(15, 9, "T");
  mvaddstr(15, 12, "R");
  //mvaddstr(15, 15, "i");
  mvaddstr(15, 18, "S");
  attrset(COLOR_PAIR(0));
  mvaddstr(35, 1, "--- put any key ---");


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
  mvaddstr(19, 0, "(挙動が普通でないのは仕様です)");
  mvaddstr(21, 0, "   f  :ブロックを落下");
  mvaddstr(24, 0, "q/Esc :ゲームを終了");
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

void tetris::clearnmino(){
  for(int i = 0; i < 20; i++)
    for(int j = 0; j < 10; j++)
      nmino[i][j] = 0;
}

bool tetris::enabletomove(mino block){
  bool enable = true;
  coordinate list[4];
  block.getrestblock(list);
  list[3].x = block.center.x;
  list[3].y = block.center.y;

  for(int i = 0; i < 4; i++){
    if( list[i].x > 9 ) enable *= 0;
    if( list[i].x < 0 ) enable *= 0;
    if( nmino[list[i].y][list[i].x] != 0 ) enable *= 0;

    if( list[i].y > 19 ) enable *= 0;
    if( nmino[list[i].y][list[i].x] != 0 ) enable *= 0;
  }

  return enable;
}

void tetris::stock(std::queue<mino>& que){
  stocked = que.front();
  que.pop();
  quepush(que);
}

void tetris::release(std::queue<mino>& que){
  mino tmpfront = que.front();
  stocked.center.x = tmpfront.center.x;
  stocked.center.y = tmpfront.center.y;
  stocked.rotate = 0;
  que.front() = stocked;
  stocked.type = 0;
}

void tetris::fall(std::queue<mino>& que){
  int count = 0;
  for( ; fall_enable(que); que.front().center.y++, count++);
  if( count > 0 ) que.front().center.y--;
}

bool tetris::fall_enable(std::queue<mino>& que){
  coordinate out[3];
  que.front().getrestblock(out);
  bool Return = true;

  for(int i = 0; i < 3; i++){
    if( out[i].y == 20 ) Return *= 0;
    if( nmino[out[i].y][out[i].x] )
      Return *= 0;
  }
  if( nmino[que.front().center.y][que.front().center.x] )
    Return *= 0;
  return Return;
}

void tetris::showstocked(){
  attrset(COLOR_PAIR(0));
  mvaddstr(1, 3, "stocked");
  for(int i = 2; i < 8; i++){
    for(int j = 0; j < 7; j++){
      mvaddstr(i, j, " ");
    }
  }
  if( stocked.type ){
    stocked.center.y = 4;
    stocked.center.x = 5;
    stocked.rotate = 0;
    coordinate out[3];
    stocked.getrestblock(out);
    attrset(COLOR_PAIR(stocked.type*10+stocked.type));
    mvaddstr(stocked.center.y, stocked.center.x, " ");
    for(int l = 0; l < 3; l++)
      mvaddstr(out[l].y, out[l].x, " ");
  }
}

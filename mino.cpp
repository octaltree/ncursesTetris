#include"localmino.h"

/*
void mino::getrestblock(coordinate out[]){
  int N = rotate % block[type].rotate;//行列をかける回数

  coordinate tmp;
  int itmp;

  for(int i = 0; i < 3; i++){
    tmp.x = block[type].p[i].x;
    tmp.y = block[type].p[i].y;
    for(int j = 0; j < N; j++){
      itmp = tmp.x;
      tmp.x = -1 * block[type].p[i].y;
      tmp.y = itmp;
    }
    tmp.x += center.x;
    tmp.y += center.y;
    out[i] = tmp;
  }
}
*/

//type4まではこの関数の定義でおｋ
void mino::getrestblock(coordinate out[]){
  int N = rotate % block[type].rotate;//行列をかける回数

  coordinate tmp;
  int itmp;

  for(int i = 0; i < 3; i++){
    tmp.x = block[type].p[i].x;
    tmp.y = block[type].p[i].y;
    for(int j = 0; j < N; j++){
      itmp = tmp.x;
      tmp.x = tmp.y;
      tmp.y = -1 * itmp;
    }
    tmp.x += center.x;
    tmp.y += center.y;
    out[i] = tmp;
  }
}

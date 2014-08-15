#include"mino.h"

void mino::getrestblock(coordinate out[]){
  int N = rotate % block[type].rotate;//行列をかける回数
  /*
  -90度回転行列
 (0  1)
 (-1 0)
 を適用し時計回りを実現
 */

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

#pragma once

typedef struct{
  int x, y;
}coordinate;

//x, y, rotate, typeを所持し、呼ばれたら残り3ブロックの座標を返す
class mino{
  private:
    typedef struct{
      int rotate;
      coordinate p[3];
    }BLOCK;

    BLOCK block[8] = {
      {1, {{0, 0}, {0, 0},  {0, 0}}}, //null
      {2, {{0, -1}, {0, 1}, {0, 2}}}, // I 6
      {1, {{0, 1}, {1, 0},  {1, 1}}}, // O 3
      {2, {{0, -1}, {-1, 0},  {-1, 1}}}, //S 2
      {2, {{0, -1}, {1, 0},  {1, 1}}}, // Z 1
      {4, {{0, -1}, {0, 1},  {-1, 1}}}, //J 4
      {4, {{0, -1}, {0, 1},  {1, 1}}}, // L 7
      {4, {{0, -1}, {1, 0},  {-1, 0}}}, // T 5
    };

  public:
    coordinate center;
    int rotate, type;
    void getrestblock(coordinate[]);

    /* 
    90度回転行列
    (0 -1)
    (1  0)

    -90度回転行列
    (0  1)
    (-1 0)
    */
};

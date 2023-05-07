#include <iostream>
#include "board.h"

int main()
{
    Board board;

    //プレイヤー初期位置
    Point* start = new Point(8, 8);
    //ゴール位置
    Point* goal = new Point(1, 1);
    board.find(*start, *goal);

    board.show();

    system("PAUSE");
}
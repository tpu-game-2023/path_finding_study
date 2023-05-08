#include <iostream>
#include "board.h"

int main()
{
    Board board;

    //startは(8，8)の位置。goalは(1, 1)の位置
    Point* start = new Point(8, 8);
    Point* goal = new Point(1, 1);

    board.find(*start, *goal);

    //ボードの描画
    board.show();

    system("PAUSE");
}

#include <iostream>
#include "board.h"

int main()
{
    Board board;

    //�v���C���[�����ʒu
    Point* start = new Point(8, 8);
    //�S�[���ʒu
    Point* goal = new Point(1, 1);
    board.find(*start, *goal);

    board.show();

    system("PAUSE");
}
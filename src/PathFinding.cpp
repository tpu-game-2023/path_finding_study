#include <iostream>
#include "board.h"

int main()
{
    Board board;

    //start��(8�C8)�̈ʒu�Bgoal��(1, 1)�̈ʒu
    Point* start = new Point(8, 8);
    Point* goal = new Point(1, 1);

    board.find(*start, *goal);

    //�{�[�h�̕`��
    board.show();

    system("PAUSE");
}

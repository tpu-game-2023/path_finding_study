#include <iostream>
#include "board.h"

int main()
{
    Board board;                     //�{�[�h�̐錾

    Point* start = new Point(8, 8);  //�J�n�n�_�̐ݒ�B
    Point* goal = new Point(1, 1);   //�I���n�_�̐ݒ�B
    board.find(*start, *goal);       //�J�n�n�_����I���n�_�̃��[�g��������B

    board.show();                    //�{�[�h��̌��ʂ�s���\��

    system("PAUSE");         //����ɃR�}���h���s����ƂO�ɂȂ�H
}

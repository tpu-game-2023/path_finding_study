#include <iostream>
#include "board.h"

int main()
{
    //�{�[�h�̃C���X�^���X����
    Board board;

    //�X�^�[�g�ƃS�[�����w��
    Point* start = new Point(8, 8);
    Point* goal = new Point(1, 1);

    //�}�X��T�����ă}�X�ɃX�e�[�^�X�����Ă���
    board.find(*start, *goal);

    //find�ł����X�e�[�^�X�����Ȃ���}�b�v��`�悷��
    board.show();

    //�L�[���������܂ňꎞ��~
    system("PAUSE");
}

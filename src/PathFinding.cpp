#include <iostream>
#include "board.h"

int main()
{
    Board board;                     //ボードの宣言

    Point* start = new Point(8, 8);  //開始地点の設定。
    Point* goal = new Point(1, 1);   //終了地点の設定。
    board.find(*start, *goal);       //開始地点から終了地点のルートを見つける。

    board.show();                    //ボード上の効果や行動表示

    system("PAUSE");         //正常にコマンドが行われると０になる？
}

#include <iostream>
#include "board.h"

int main()
{
    //ボードのインスタンス生成
    Board board;

    //スタートとゴールを指定
    Point* start = new Point(8, 8);
    Point* goal = new Point(1, 1);

    //マスを探索してマスにステータスをつけていく
    board.find(*start, *goal);

    //findでつけたステータスを見ながらマップを描画する
    board.show();

    //キーが押されるまで一時停止
    system("PAUSE");
}

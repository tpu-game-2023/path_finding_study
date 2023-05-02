#include "board.h"

//経路探索をしてステータスを設定する
bool Board::find(const Point& start, const Point& goal)
{
	//スタートとゴールにステータスを設定
	mass_[start.y()][start.x()].setStatus(Mass::START);
	mass_[goal.y()][goal.x()].setStatus(Mass::GOAL);

	//調べるポイントpをスタート地点に設定
	Point p = start;

	//pがゴールに到達するまで処理する
	while (p != goal)
	{
		//通った地点にステータスWAYPOINT(〇印)を付与。スタート地点には付与しない。
		if (p != start) mass_[p.x()][p.y()].setStatus(Mass::WAYPOINT);

		//pの座標がゴール地点の座標と等しくなるまでx,yを増減させる
		if (p.x() < goal.x()) { p.setX(p.x() + 1); continue; }
		if (goal.x() < p.x()) { p.setX(p.x() - 1); continue; }
		if (p.y() < goal.y()) { p.setY(p.y() + 1); continue; }
		if (goal.y() < p.y()) { p.setY(p.y() - 1); continue; }
	}

	return false;
}

//マップ描画、変更不要
void Board::show() const 
{
	std::cout << std::endl;

	for (int y = 0; y < BOARD_SIZE; y++) {
		std::cout << " ";
		for (int x = 0; x < BOARD_SIZE; x++) {
			std::cout << "+-";
		}
		std::cout << "+" << std::endl;

		std::cout << " ";
		for (int x = 0; x < BOARD_SIZE; x++) {
			std::cout << "|";
			switch (mass_[y][x].getStatus()) {
			case Mass::BLANK:
				std::cout << " ";
				break;
			case Mass::START:
				std::cout << "S";
				break;
			case Mass::GOAL:
				std::cout << "G";
				break;
			case Mass::WAYPOINT:
				std::cout << "o";
				break;
			case Mass::WALL:
				std::cout << "#";
				break;
			case Mass::WATER:
				std::cout << "~";
				break;
			case Mass::ROAD:
				std::cout << "$";
				break;
			}
		}
		std::cout << "|" << std::endl;
	}

	std::cout << " ";
	for (int x = 0; x < BOARD_SIZE; x++) {
		std::cout << "+-";
	}
	std::cout << "+" << std::endl;

}

#include <algorithm>
#include "board.h"

//ソート用の比較関数
bool asc(const Mass* o1, const Mass* o2) {
	return o1->getCost() < o2->getCost();  //o1のマス目のコストとo2のマス目のコストを比較
}

//壁があるか調べる
bool Board::isValidated(const Point &p)
{
	if (getMass(p).getStatus() == Mass::WALL) {  //pのマス目が壁かどうか
		return false;
	}
	return true;
}

//行ける場所の探索
bool Board::find(const Point &start, const Point &goal)
{
	Mass &mass_start = getMass(start);  //開始地点のマス目の場所を設定
	Mass &mass_goal = getMass(goal);    //終了地点のマス目の場所を設定

	mass_start.setStatus(Mass::START);    //開始地点のマス目にSTARTを設置
	mass_goal.setStatus(Mass::GOAL);      //終了地点のマス目にGOALを設置

	//オープンリストに開始ノードを追加する
	open_list_.clear();                      //リストの初期化
	open_list_.push_back(&mass_start);  //開始地点をリストの最後に追加する。

	while (!open_list_.empty())  //オープンリストが空ならTRUE、空でなければFALSEを返し、それがFALSEで無ければ繰り返す
	{
		//現在のノード
		std::sort(open_list_.begin(), open_list_.end(), asc);  //先頭要素を指すイテレータと最後尾要素のイテレータを比較して数の大小で並び替えていく
		auto it = open_list_.begin();  //itにリストの先頭要素を代入
		Mass* current = *it; //Massクラスに先頭要素を代入
		if (current->getStatus() == Mass::GOAL) {  //マス目の値がゴールだったら
			//目的地なら経路の完成
			Mass* p = current;  //経路ステータスをMass::WAYPOINTにする
			while (p){
				if (p->getStatus() == Mass::BLANK){
					p->setStatus(Mass::WAYPOINT);
					p = p->getParent();
				}
			}
			return true;
		}
		else {  //マス目の値がゴールでなければ
			//現在のノードをクローズリストに移す
			open_list_.erase(it);   //itに入っている数のリストの場所を削除する。
			current->setListed(Mass::CLOSE);  
			//現在のノードの隣接する各ノードを調べる
			const Point &pos = current->getPos();
			Point next[4] = { pos.getRight(),pos.getLeft(),pos.getUp(),pos.getDown() };
			for (auto &c : next) { //隣接ノード
				//マップ外ならスキップ
				if (c.x() < 0 || BOARD_SIZE <= c.x()) continue;  
				if (c.y() < 0 || BOARD_SIZE <= c.y())continue;
				Mass& m = getMass(c);
				if (!m.isListed(Mass::OPEN) &&   //オープンリストに含まれていない
					!m.isListed(Mass::CLOSE) &&  //クローズリストに含まれていない
					m.getStatus() != Mass::WALL) {  //障害物でない
					//オープンリストに移してコストを計算する。
					open_list_.push_back(&m);
					m.setParent(current, goal);
					m.setListed(Mass::OPEN);
				}
			}
		}
	}
	return false;
}

//ボードを表示
void Board::show() const 
{
	std::cout << std::endl;

	for (int y = 0; y < BOARD_SIZE; y++) {
		std::cout << " ";
		for (int x = 0; x < BOARD_SIZE; x++) {
			std::cout << "+---";
		}
		std::cout << "+" << std::endl;

		//マス目の特徴を表示
		std::cout << " ";
		for (int x = 0; x < BOARD_SIZE; x++) {
			std::cout << "| ";
			switch (mass_[y][x].getStatus()) {
			case Mass::BLANK:
				std::cout << "  ";
				break;
			case Mass::START:
				std::cout << "S ";
				break;
			case Mass::GOAL:
				std::cout << "G ";
				break;
			case Mass::WAYPOINT:
				std::cout << "o ";
				break;
			case Mass::WALL:
				std::cout << "# ";
				break;
			case Mass::WATER:
				std::cout << "~ ";
				break;
			case Mass::ROAD:
				std::cout << "$ ";
				break;
			}
		}
		std::cout << "|" << std::endl;
	}

	std::cout << " ";
	for (int x = 0; x < BOARD_SIZE; x++) {
		std::cout << "+---";
	}
	std::cout << "+" << std::endl;

}


//mass_[start.y()][start.x()].setStatus(Mass::START);
//mass_[goal.y()][goal.x()].setStatus(Mass::GOAL);

//Point p = start;
//while (p != goal) {
//	if (p != start) mass_[p.x()][p.y()].setStatus(Mass::WAYPOINT);

//	int load_x = abs(p.x() - goal.x());
//	int load_y = abs(p.y() - goal.y());
//	if (load_y < load_x) {
//		if (p.x() < goal.x()) { p.setX(p.x() + 1); continue; }
//		if (goal.x() < p.x()) { p.setX(p.x() - 1); continue; }
//	}
//	if (p.y() < goal.y()) { p.setY(p.y() + 1); continue; }
//	if (goal.y() < p.y()) { p.setY(p.y() - 1); continue; }

//	//if (p.x() < goal.x()) { p.setX(p.x() + 1); continue; }
//	//if (goal.x() < p.x()) { p.setX(p.x() - 1); continue; }
//	//if (p.y() < goal.y()) { p.setY(p.y() + 1); continue; }
//	//if (goal.y() < p.y()) { p.setY(p.y() - 1); continue; }
//}


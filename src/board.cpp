#include "board.h"

//ソート用関数
//ここに書かないとリンカエラーが取れなかった
bool asc(const Mass* o1, const Mass* o2)
{
	return o1->getCost() < o2->getCost();
}

//経路探索をしてステータスを設定する
bool Board::find(const Point& start, const Point& goal)
{
	Mass& mass_start = getMass(start);
	Mass& mass_goal = getMass(goal);

	mass_start.setStatus(Mass::START);
	mass_goal.setStatus(Mass::GOAL);

	//オープンリストに開始ノード追加
	open_list_.clear();
	open_list_.push_back(&mass_start);

	while (!open_list_.empty()) //オープンリストが空でない
	{
		//現在のノード＝最も安価なやつ
		std::sort(open_list_.begin(), open_list_.end(), asc);
		auto it = open_list_.begin();
		Mass* current = *it;
		if (current->getStatus() == Mass::GOAL)//目的地なら完成
		{
			Mass* p = current;//経路のステータスをMass::WAYPOINTにする
			while (p)
			{
				if (p->getStatus() == Mass::BLANK)
				{
					p->setStatus(Mass::WAYPOINT);
				}
					p = p->getParent();
			}
			return true;
		}
		else
		{
			//現在のノードをクローズリストに移動
			open_list_.erase(it);
			current->setListed(Mass::CLOSE);
			//現在のノードに隣接するノードをすべて調べる
			const Point& pos = current->getPos();
			Point next[4] = { pos.getRight(), pos.getLeft(), pos.getUp(), pos.getDown() };
			for (auto& c : next) //隣接ノード
			{
				if (c.x() < 0 || BOARD_SIZE <= c.x()) continue; //マップ外ならスキップ
				if (c.x() < 0 || BOARD_SIZE <= c.y()) continue; //マップ外ならスキップ

				Mass& m = getMass(c);
				if (!m.isListed(Mass::OPEN) && //オープンリストに含まれていない
					!m.isListed(Mass::CLOSE) && //クローズドリストに含まれていない
					 m.getStatus() != Mass::WALL) //壁ではない
				{
					//オープンリストに移してコスト計算
					open_list_.push_back(&m);
					m.setParent(current, goal);
					m.setListed(Mass::OPEN);
				}
			}
		}
	}

	//ゴールに到達しなかった
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

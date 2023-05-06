#pragma once
#include <iostream>
#include <vector>
#include <algorithm>


//ポイントクラス
class Point
{
	//メンバ変数で座標を保持
	int x_ = -1,
		y_ = -1;

public:
	//わからない
	Point() {}
	Point(int x, int y) :x_(x), y_(y) {}

	//_xや_yを返却する関数。getX,getYみたいな感じ？
	int x() const
	{
		return x_;
	}
	int y() const
	{
		return y_;
	}
	//x_やy_に引数の値を代入する
	void setX(int x)
	{
		x_ = x;
	}
	void setY(int y)
	{
		y_ = y;
	}
	void set(int x, int y)
	{
		x_ = x;
		y_ = y;
	}

	//わからない
	bool operator == (const Point& p) const
	{
		return p.x() == x_ && p.y() == y_;
	}
	bool operator != (const Point& p) const
	{
		return !(p == *this);
	}

	//上下左右のPointを得る
	Point getRight() const { return Point(x_ + 1, y_); }
	Point getLeft() const { return Point(x_ - 1, y_); }
	Point getUp() const { return Point(x_, y_ - 1); }
	Point getDown() const { return Point(x_, y_ + 1); }

	//二次元的な直線距離
	static double distance(const Point p1, const Point p2)
	{
		//p1とp2の座標差分
		double dx = (double)p2.x() - (double)p1.x();
		double dy = (double)p2.y() - (double)p1.y();
		//三平方の定理
		return sqrt(dx * dx + dy * dy);
	}
};

//マスクラス
class Mass
{
public:
	//ステータスを列挙型で宣言
	enum status
	{
		BLANK,
		GOAL,
		START,
		WAYPOINT,
		WALL, // 通れない
		WATER,// 進むのが1/3に遅くなる
		ROAD,//進むのが3倍速い
	};
	//どのリストに含まれているか、あるいはどのリストにも含まれていないか
	enum listed
	{
		NONE,
		OPEN,
		CLOSE,
	};

private:
	//自分のステータスを格納
	status s_ = BLANK;
	listed listed_ = NONE;
	Point pos_;
	Mass* pParent_ = nullptr;
	double steps_ = 0;
	double estimate_ = 0.0;

	//地形コストの計算
	double getWalkCost(Mass& m)
	{
		status s = m.getStatus();

		switch (s)
		{
		case Mass::WATER:
			return 3;
			break;
		case Mass::ROAD:
			return 1 / 3;
			break;
		default:
			return 1;
			break;
		}
	}

	//コストの計算
	void calcCost(const Point target)
	{
		//親がいたらそのsteps_に、いなければ0に、プラス地形効果（基準値は1）
		steps_ = (pParent_ ? pParent_->steps_ : 0) + getWalkCost(*this);
		//ゴールまでの距離
		estimate_ = Point::distance(pos_, target);
	}

public:
	//引数として受け取ったステータスを自分に設定
	void setStatus(status s)
	{
		s_ = s;
	}
	//自分のステータスを返却する
	status getStatus() const 
	{
		return s_;
	}
	//pos_に値を代入
	void setPos(int x, int y)
	{
		pos_.set(x, y);
	}
	
	const Point &getPos() const
	{
		return pos_;
	}
	int x()
	{
		return pos_.x();
	}
	int y()
	{
		return pos_.y();
	}

	//親を設定
	void setParent(Mass* pParent, const Point& goal)
	{
		pParent_ = pParent;
		calcCost(goal);
	}

	Mass* getParent()
	{
		return pParent_;
	}

	void setListed(listed t)
	{
		listed_ = t;
	}
	bool isListed(listed t) const
	{
		return listed_ == t;
	}

	double getCost() const
	{
		//歩数とヒューリスティックコストを足してコストにする。
		return steps_ + estimate_;
	}
};

//cpp側に書いたらリンカエラーが消えた
/*
//ソート用関数
bool asc(const Mass* o1, const Mass* o2)
{
	return o1->getCost() < o2->getCost();
}
*/

class Board 
{
private:
	//ボードの大きさ決定
	enum
	{
		BOARD_SIZE = 10,
	};

	//ボードのインスタンス生成、2次元配列にする
	Mass mass_[BOARD_SIZE][BOARD_SIZE];
	//Pointを元にmass_配列のなかから任意のMassインスタンスを取ってくる
	Mass& getMass(const Point p)
	{
		return mass_[p.y()][p.x()];
	}

	std::vector<Mass*> open_list_;

public:
	//地形の指定
	Board()
	{
		for (int y = 0; y < BOARD_SIZE; y++)
		{

			//まず全てを「BLANK＝空」に設定
			for (int x = 0; x < BOARD_SIZE; x++)
			{
				mass_[y][x].setStatus(Mass::BLANK);
				mass_[y][x].setPos(x, y);
			}
		}
		// 壁を設定
		mass_[4][6].setStatus(Mass::WALL);
		mass_[4][5].setStatus(Mass::WALL);
		mass_[5][5].setStatus(Mass::WALL);
		mass_[6][5].setStatus(Mass::WALL);

		// 水を設定
		for (int y = 4; y <= 7; y++)
		{
			for (int x = 1; x <= 4; x++)
			{
				mass_[y][x].setStatus(Mass::WATER);
			}
		}
		// 道を設定
		mass_[4][1].setStatus(Mass::ROAD);
		mass_[5][1].setStatus(Mass::ROAD);
		mass_[5][2].setStatus(Mass::ROAD);
		mass_[5][3].setStatus(Mass::ROAD);
		mass_[6][1].setStatus(Mass::ROAD);
		mass_[6][2].setStatus(Mass::ROAD);
		mass_[6][3].setStatus(Mass::ROAD);

	}
	//デストラクタ？
	~Board() {}


	//そこが壁かどうか調べる
	bool isValidated(const Point& p)
	{
		if (getMass(p).getStatus() == Mass::WALL);
		{
			return false;
		}
		return true;
	}



	//関数宣言？
	bool find(const Point& start, const Point& goal);
		
	void show() const;
};

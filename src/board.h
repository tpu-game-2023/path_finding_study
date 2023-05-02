#pragma once
#include <iostream>

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

private:
	//自分のステータスを格納
	status s_ = BLANK;

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
};

//ポイントクラス
class Point
{
	//メンバ変数で座標を保持
	int x_ = -1,
		y_ = -1;

	//わからない
public:
	Point(int x, int y) :x_(x), y_(y) {}
	int x() const 
	{ 
		return x_;
	}
	int y() const 
	{
		return y_; 
	}
	void setX(int x) 
	{
		x_ = x;
	}
	void setY(int y) 
	{
		y_ = y;
	}

	bool operator == (const Point& p) const
	{
		return p.x() == x_ && p.y() == y_;
	}
	bool operator != (const Point& p) const
	{
		return !(p == *this);
	}
};

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

	//関数宣言？
	bool find(const Point& start, const Point& goal);

	void show() const;
};

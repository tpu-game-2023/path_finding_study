#pragma once
#include <iostream>
#include <vector>

class Point {
	int x_ = -1,
		y_ = -1;
public:
	Point() {}
	Point(int x, int y) :x_(x), y_(y) {}  //初期化
	int x() const { return x_; }  //x_の定数にする？
	int y() const { return y_; }  //y_の定数にする？
	void setX(int x) { x_ = x; }  //x_に値を代入する関数
	void setY(int y) { y_ = y; }  //y_に値を代入する関数
	void set(int x, int y) { x_ = x; y_ = y; }  //x_とy_に値を代入する関数

	bool operator == (const Point& p) const { return p.x() == x_ && p.y() == y_; }  //xの元の値と今の値、yの元の値を今の値がどちらも同じだったらtrue
	bool operator != (const Point& p) const { return !(p == *this); }   //pointクラスのpのx、yの値と今のx,yの値が同じではない時true

	Point getRight() const { return Point(x_ + 1, y_); }  //xの値から１を追加
	Point getLeft() const { return Point(x_ - 1, y_); }   //xの値から１を引く
	Point getUp() const { return Point(x_, y_ - 1); }     //yの値から１を引く
	Point getDown() const { return Point(x_, y_ + 1); }   //yの値から１を追加

	static double distance(const Point p1, const Point p2) {  //２点間の距離を求める。
		double dx = (double)p2.x() - (double)p1.x();
		double dy = (double)p2.y() - (double)p1.y();
		return sqrt(dx * dx + dy * dy);  //「sqrt」平方根を求める。
	}
};
//「const」をint x() constと付けると付加された変数または関数が定数であることを意味する。（const修飾子）
//「:」初期化子リスト。初期化を行うもの。書き方は変数名（初期値）。

class Mass {
public:
	enum status {
		BLANK,   //空いている場所
		GOAL,    //ゴール
		START,   //開始地点
		WAYPOINT,//歩く場所
		WALL,    // 通れない
		WATER,   // 進むのが1/3に遅くなる
		ROAD,    //進むのが3倍速い
	};
	enum listed {
		NONE,   //空
		OPEN,   //オープン
		CLOSE,  //クローズ
	};
private:
	status s_ = BLANK;  //初期化？
	listed listed_ = NONE;  //リストの初期化
	Point pos_;  //Pointを宣言
	Mass *pParent_ = nullptr;  //「nullptr」nullポインタ。nullとしての０ではなくポインタとして扱ってくれるnull。
	int steps_ = 0;
	double estimate_ = 0.0;

	static int getWalkCost(Mass& m) { status s = m.getStatus(); return (s == WATER) ? 3 : 1; }

	void calcCost(const Point target) {
		steps_ = (pParent_ ? pParent_->steps_ : 0) + getWalkCost(*this);  //pParentがtrueの時はsteps_に１加算したものをsteps_に代入。falseの時は０に１を加算したものを代入.
		estimate_ = Point::distance(pos_, target);  //pos_とtargetの距離を求めたものをestimate_に代入
	}
public:
	void setStatus(status s) { s_ = s; }     //列挙型の変数に値を代入する?
	status getStatus() const { return s_; }  //列挙型の変数から値を取り出す?

	void setPos(int x, int y) { pos_.set(x, y); }
	const Point &getPos() const { return pos_; }
	int x() { return pos_.x(); }
	int y() { return pos_.y(); }

	void setParent(Mass *pParent, const Point &goal) { pParent_ = pParent; calcCost(goal); }
	Mass* getParent() { return pParent_; }

	void setListed(listed t) { listed_ = t; }
	bool isListed(listed t)const { return listed_ == t; }

	double getCost()const { return (double)steps_ + estimate_; }
};

class Board {
private:
	enum {
		BOARD_SIZE = 10,  //マス目のサイズ
	};
	Mass mass_[BOARD_SIZE][BOARD_SIZE];  //マス目の定義
	Mass &getMass(const Point p) { return mass_[p.y()][p.x()]; }

	std::vector<Mass*> open_list_;
public:
	Board() {
		for (int y = 0; y < BOARD_SIZE; y++) {
			for (int x = 0; x < BOARD_SIZE; x++) {
				mass_[y][x].setStatus(Mass::BLANK);  //全部のマス目を初期化して空いている状態にする。
			}
		}

		// 壁
		mass_[4][6].setStatus(Mass::WALL);  //マス目の縦４横６番目に壁を設置
		mass_[4][5].setStatus(Mass::WALL);  //マス目の縦４横５番目に壁を設置
		mass_[5][5].setStatus(Mass::WALL);  //マス目の縦５横５番目に壁を設置
		mass_[6][5].setStatus(Mass::WALL);  //マス目の縦６横５番目に壁を設置

		// 水
		for (int y = 4; y <= 7; y++) {
			for (int x = 1; x <= 4; x++) {
				mass_[y][x].setStatus(Mass::WATER);  //マス目の縦４～７横１～４番目に水を設置。
			}
		}

		// 道
		mass_[4][1].setStatus(Mass::ROAD);  //マス目の縦４横１番目に道を設置
		mass_[5][1].setStatus(Mass::ROAD);  //マス目の縦５横１番目に道を設置
		mass_[5][2].setStatus(Mass::ROAD);  //マス目の縦５横２番目に道を設置
		mass_[5][3].setStatus(Mass::ROAD);  //マス目の縦５横３番目に道を設置
		mass_[6][1].setStatus(Mass::ROAD);  //マス目の縦６横１番目に道を設置
		mass_[6][2].setStatus(Mass::ROAD);  //マス目の縦６横２番目に道を設置
		mass_[6][3].setStatus(Mass::ROAD);  //マス目の縦６横３番目に道を設置
	}
	~Board() {}

	//「~」デストラクタ。オブジェクトを破棄する際に呼び出す関数のこと。

	void addWall(const Point &p) { getMass(p).setStatus(Mass::WALL); }

	bool isValidated(const Point &p);

	bool find(const Point &start, const Point &goal);  //進める場所を探す。

	void show() const;  //ボードを表示
};

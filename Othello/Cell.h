#pragma once

enum ColorFlag
{
	EMPTY,
	BLACK,
	WHITE,
	HOLE,
	BIG_E,
	BIG_B,
	BIG_W
};

enum BigCell
{
	LT,
	RT,
	LB,
	RB,
	NONE = -1
};

class Cell
{
public: //メンバ変数
	ColorFlag colorFlag = ColorFlag::EMPTY;
private:
	ColorFlag init = colorFlag;
	BigCell bigCell = BigCell::NONE;

public: //メンバ関数
	// 初期化
	void Init(ColorFlag colorFlag, BigCell bigCell = BigCell::NONE);
	// リセット
	void Reset() { colorFlag = init; }

	ColorFlag GetInitColor() const { return init; }
	BigCell GetBigCell() const { return bigCell; }
};
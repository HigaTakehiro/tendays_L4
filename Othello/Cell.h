#pragma once

enum ColorFlag
{
	EMPTY, //空白
	BLACK, //黒
	WHITE, //白
	HOLE,  //穴
	BIG_E, //空白（デカマス）
	BIG_B, //黒（デカマス）
	BIG_W  //白（デカマス）
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
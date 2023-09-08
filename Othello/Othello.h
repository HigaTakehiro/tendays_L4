#pragma once
#include "Cell.h"
#include <vector>
#include <string>

class Othello
{
private: //サブクラス
	enum Direction
	{
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

public: //定数
	static const int circleSize = 90;

private: //メンバ変数
	std::vector<Cell> cell;
	int width;  //幅
	int height; //高さ
	bool startColor; //開始時の色

public: //メンバ関数
	Othello();
	~Othello() {}

	// 初期化
	void Init();
	// 描画
	void Draw(int offsetX = 0, int offsetY = 0);
	// リセット
	void Reset();

	// 石を置く
	int Put(int x, int y, ColorFlag color);
	// デカマスに石を置く
	int BigPut(int index, ColorFlag color);
	// デカマスの反転
	int BigFlip(int index);

	// ステージ読み込み
	int Load(const std::string& filePath);
	// スキップ判定
	bool IsSkip(ColorFlag color);
	// 特定の方向に進んだ時に挟むことが出来るかどうか
	int IsPinchedDirection(int index, Direction dir, ColorFlag startColor, bool isStart = true);

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	size_t GetSize() const { return cell.size(); }
	ColorFlag GetCell(int index) const;
	ColorFlag GetStartColor() const;
};
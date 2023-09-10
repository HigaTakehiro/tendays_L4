#pragma once
#include "Cell.h"
#include <vector>
#include <string>

class Othello
{
public: //サブクラス
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
	// スキップ判定
	bool IsSkip(ColorFlag color);

	// デカマスの置き換え
	int BigChange(int index, ColorFlag color);
	// ステージ読み込み
	int Load(const std::string& filePath);

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	size_t GetSize() const { return cell.size(); }
	ColorFlag GetCell(int index) const;
	ColorFlag GetStartColor() const;
};
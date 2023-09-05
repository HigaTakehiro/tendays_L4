#pragma once
#include <vector>

enum Color
{
	EMPTY,
	BLACK,
	WHITE,
	HOLE
};

class Othello
{
private:
	template<class T>
	using vector = std::vector<T>;

public:
	static const int circleSize = 90;

private: //メンバ変数
	vector<Color> cell;
	int width;  //幅
	int height; //高さ

public: //メンバ関数
	Othello();
	~Othello() {}

	// 初期化
	void Init();
	// 描画
	void Draw(int offsetX = 0, int offsetY = 0);

	// 石を置く
	int Put(int x, int y, Color color);

	int GetWidth() { return width; }
	int GetHeight() { return height; }
	Color GetCell(const size_t& index) { return cell[index]; }
};
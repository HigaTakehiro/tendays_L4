#pragma once
#include "BaseScene.h"
#include "../Othello/Othello.h"

class GameScene : public BaseScene
{
private: //メンバ変数
	Othello othello;

	int x;
	int y;
	Color colorFlag;

	int bCount; //黒の個数
	int wCount; //白の個数
	bool isTie;
	bool isSkip;
	bool isFin;

public: //メンバ関数
	GameScene(SceneManager* sceneMgr);
	~GameScene() override;

	void Init() override;
	int Update(const KeyInput& input) override;
	void Draw() override;
};

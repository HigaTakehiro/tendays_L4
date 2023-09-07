#pragma once
#include "BaseScene.h"

class SelectScene : public BaseScene
{
private: //メンバ変数

public: //メンバ関数
	SelectScene(SceneManager* sceneMgr);
	~SelectScene() override;

	void Init() override;
	int Update(const KeyInput& input) override;
	void Draw() override;
};

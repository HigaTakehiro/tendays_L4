#pragma once
#include "BaseScene.h"

class TitleScene : public BaseScene
{
private: //メンバ変数

public: //メンバ関数
	TitleScene(SceneManager* sceneMgr);
	~TitleScene() override;

	void Init() override;
	int Update(const KeyInput& input) override;
	void Draw() override;
};

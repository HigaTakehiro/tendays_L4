#pragma once
#include "SceneManager.h"
#include "Scene.h"
#include "SceneChangeAnimation.h"
#include "../Input/KeyInput.h"

class BaseScene
{
protected: //静的メンバ変数
	static SceneChangeAnimation changeAnimation;

public: //静的メンバ関数
	static void ChangeAnimationUpdate() { changeAnimation.Update(); }
	static void ChangeAnimationDraw() { changeAnimation.Draw(); }

protected: //メンバ変数
	SceneManager* sceneMgr;

	bool isSceneDest; //シーンを破棄するかどうか
	Scene nextScene;  //次のシーンの遷移先

public: //メンバ関数
	BaseScene() = delete;
	BaseScene(SceneManager* sceneMgr);
	virtual ~BaseScene();

	virtual void Init() = 0;
	virtual int Update(const KeyInput& input) = 0;
	virtual void Draw() = 0;
};

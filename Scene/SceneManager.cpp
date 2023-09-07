#include "SceneManager.h"
#include <DxLib.h>

#include "TitleScene.h"
#include "GameScene.h"
#include "SelectScene.h"

SceneManager::SceneManager()
{
	sceneStack.push(std::make_shared<TitleScene>(this));
}

void SceneManager::Loop(const KeyInput& input) const
{
	if (sceneStack.top()->Update(input) != 0)
	{
		sceneStack.top()->Init();
	}
	BaseScene::ChangeAnimationUpdate();

	// 画面クリア
	ClearDrawScreen();

	sceneStack.top()->Draw();
	BaseScene::ChangeAnimationDraw();

	// (ダブルバッファ)裏面
	ScreenFlip();

	// 20ミリ秒待機(疑似60FPS)
	WaitTimer(20);
}

void SceneManager::SceneChange(Scene scene, bool stackClear)
{
	using namespace std;

	if (stackClear)
	{
		while (sceneStack.empty() == false)
		{
			sceneStack.pop();
		}
	}

	switch (scene)
	{
	case Scene::Title:
		sceneStack.push(make_shared<TitleScene>(this));
		break;
	case Scene::Game:
		sceneStack.push(make_shared<GameScene>(this));
		break;
	case Scene::StageSelect:
		sceneStack.push(make_shared<SelectScene>(this));
		break;
	default:
		OutputDebugStringA("存在しないシーンが呼ばれました\n");
		break;
	}
}

int SceneManager::PopScene()
{
	if (sceneStack.size() <= 1)
	{
		return -1;
	}

	sceneStack.pop();

	return 0;
}

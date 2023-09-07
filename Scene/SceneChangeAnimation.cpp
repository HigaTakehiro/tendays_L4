#include "SceneChangeAnimation.h"
#include <DxLib.h>

SceneChangeAnimation::SceneChangeAnimation() :
	winW(0),
	winH(0),
	isAnimation(false),
	isChange(false),
	radius(1)
{
	Init();
	Load();
}

SceneChangeAnimation::~SceneChangeAnimation()
{
	Release();
}

void SceneChangeAnimation::Init()
{
	RECT rect;
	GetClientRect(GetMainWindowHandle(), &rect);
	winW = rect.right - rect.left;
	winH = rect.bottom - rect.top;

	isAnimation = false;
	isChange = false;
	radius = 1;
}

void SceneChangeAnimation::Update()
{
	if (isAnimation == false)
	{
		return;
	}

	const int maxSize = winW + 150;
	const int changeSpeed = 50;

	if (isChange)
	{
		radius -= changeSpeed;
		if (radius < 0)
		{
			isAnimation = false;
		}
	}
	else
	{
		radius += changeSpeed;
		if (radius > maxSize)
		{
			isChange = true;
		}
	}
}

void SceneChangeAnimation::Draw()
{
	if (isAnimation)
	{
		if (radius > 0)
		{
			DrawBox(winW / 2 - radius / 2, winH / 2 - radius / 2,
					winW / 2 + radius / 2, winH / 2 + radius / 2, GetColor(0, 0, 0), true);
		}
	}
}

void SceneChangeAnimation::Start()
{
	isAnimation = true;
	isChange = false;
	radius = 1;
}

void SceneChangeAnimation::Load()
{
	ScreenInit();
}

void SceneChangeAnimation::Release()
{
}

void SceneChangeAnimation::ScreenInit()
{
}

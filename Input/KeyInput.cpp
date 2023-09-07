#include "KeyInput.h"
#include <DxLib.h>

void KeyInput::Update()
{
	for (int i = 0; i < 256; i++)
	{
		oldkeys[i] = keys[i];
	}
	GetHitKeyStateAll(keys);
}

bool KeyInput::IsKey(int KeyCode) const
{
	return keys[KeyCode];
}

bool KeyInput::IsKeyTrigger(int KeyCode) const
{
	return keys[KeyCode] && !oldkeys[KeyCode];
}

bool KeyInput::IsKeyReturn(int KeyCode) const
{
	return !keys[KeyCode] && oldkeys[KeyCode];
}

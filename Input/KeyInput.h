#pragma once

class KeyInput
{
private: //メンバ変数
	char keys[256] = {};
	char oldkeys[256] = {};

public: //メンバ関数
	KeyInput() = default;
	~KeyInput() = default;

	// 更新処理
	void Update();
	// キーを押しているかどうかの判定
	bool IsKey(int KeyCode);
	// キーを押した瞬間かどうかの判定
	bool IsKeyTrigger(int KeyCode);
	// キーを離した瞬間かどうかの判定
	bool IsKeyReturn(int KeyCode);
};

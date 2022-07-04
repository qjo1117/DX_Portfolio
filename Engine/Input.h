#pragma once

/*-----------
	Input
------------*/
class Input
{
private:
	DECLARE_SINGLE(Input)

public:
	/* ----- External Function ----- */
	void Init(HWND hWnd);
	void Update();

	/* ----- Helper Function ----- */
	bool GetButton(KEY_TYPE key) { return GetState(key) == KEY_STATE::PRESS; }
	bool GetButtonDown(KEY_TYPE key) { return GetState(key) == KEY_STATE::DOWN; }
	bool GetButtonUp(KEY_TYPE key) { return GetState(key) == KEY_STATE::UP; }

	bool IsAnyKey() { return _isAnyKey; }

	const POINT& GetMousePos() { return _mousePos; }

private:
	/* ----- Mapping Function ----- */
	inline KEY_STATE GetState(KEY_TYPE key) { return _states[static_cast<uint8>(key)]; }

private:
	/* ----- Input Variable ----- */
	HWND				_hWnd;

	bool				_isAnyKey = false;

	vector<KEY_STATE>	_states;
	BYTE				_ascilKeys[KEY_TYPE_COUNT] = {};
	POINT				_mousePos = {};
};


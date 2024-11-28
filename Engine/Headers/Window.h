#pragma once

#include<Windows.h>
#include<iostream>
#include "GMath.h"
#include "Driver.h"

class Inputs
{
	bool keys[256]; // keys input
	bool mouseButtons[3]; // mouse buttons 0 = left, 1 = middle, 2 = right
	Vec2 mousePos; // mouse position
	Vec2 mouseDelta; // mouse delta in single frame
	RECT screen;

	bool lockCursor; // lock cursor to the center
	bool mouseDirty;
public:

	void Init(HWND& _hwnd);
	// reset all inputs
	void Reset();
	// reset mouse pos to center of the window
	void ResetCursor();
	// set cursor lock state
	void SetCursorLock(bool _state);
	// update necessary inputs
	void Update();
	// update mouse position
	void UpdateMouse(int _x, int _y);
	// return W,A,S,D axis
	Vec2 GetAxis();
	// return UP,DOWN,LEFT,RIGHT axis
	Vec2 GetAxis2();
	// check if key is pressed
	bool KeyPressed(int key) const;
	// returns mouse position
	Vec2 MousePos() const;
	// return mouse delta for this frame
	Vec2 MouseDelta() const;

	friend LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

class Window
{
	HWND hwnd;
	HINSTANCE hinstance;
	DWORD style;

	// canvas properties
	std::string name;
	int width;
	int height;


	// DX Driver 
	DXCore dxDriver;

public:
	// inputs
	Inputs inputs;

	// creates a window
	void Create(unsigned int, unsigned int, std::string, bool = false, unsigned int = 0, unsigned int = 0);
	// processes inouts
	void Update();
	// returns current device
	DXCore& GetDevice();
	// clear window
	void Clear();
	// present window
	void Present();
};

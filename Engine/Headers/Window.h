#pragma once

#include<Windows.h>
#include<iostream>
#include "GMath.h"
#include "Driver.h"

class Inputs
{
	// inputs
	bool keys[256];
	bool mouseButtons[3];
	Vec2 mousePos;
	Vec2 mouseDelta;

public:
	void Reset()
	{
		memset(keys, 0, 256 * sizeof(bool));
		memset(mouseButtons, 0, 3 * sizeof(bool));
		mousePos = mouseDelta = Vec2(0, 0);
	}

	void Update()
	{
		mouseDelta = Vec2(0, 0);
	}

	void UpdateMouse(int _x, int _y)
	{
		Vec2 newPos(_x, _y);
		mouseDelta = newPos - mousePos;
		mousePos = newPos;
	}

	Vec2 GetAxis()
	{
		Vec2 axis;

		if (KeyPressed('W'))
			axis.y = 1;
		else if (KeyPressed('S'))
			axis.y = -1;

		if (KeyPressed('D'))
			axis.x = 1;
		else if (KeyPressed('A'))
			axis.x = -1;

		return axis.Normalize();
	}

	Vec2 GetAxis2()
	{
		Vec2 axis;

		if (KeyPressed(VK_UP))
			axis.y = 1;
		else if (KeyPressed(VK_DOWN))
			axis.y = -1;

		if (KeyPressed(VK_RIGHT))
			axis.x = 1;
		else if (KeyPressed(VK_LEFT))
			axis.x = -1;

		return axis.Normalize();
	}

	bool KeyPressed(int key) const
	{
		return keys[key];
	}

	Vec2 MousePos() const
	{
		return mousePos;
	}

	Vec2 MouseDelta() const
	{
		return mouseDelta;
	}

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

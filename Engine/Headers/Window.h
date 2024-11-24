#pragma once

#include<Windows.h>
#include<iostream>
#include "Drivers.h"

class Window
{
	HWND hwnd;
	HINSTANCE hinstance;
	DWORD style;

	// canvas properties
	std::string name;
	int width;
	int height;

	// inputs
	bool keys[256];
	bool mouseButtons[3];
	int mousex;
	int mousey;

	// DX Driver 
	DXCore dxDriver;

	// set mouse position
	void UpdateMouse(int _x, int _y);
public:
	// creates a window
	void Create(unsigned int, unsigned int, std::string, bool = false, unsigned int = 0, unsigned int = 0);
	// processes inouts
	void ProcessMessages();
	// returns current device
	DXCore& GetDevice();
	// clear window
	void Clear();
	// present window
	void Present();

	friend LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

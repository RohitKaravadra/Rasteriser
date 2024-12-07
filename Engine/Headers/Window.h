#pragma once

#include<Windows.h>
#include<iostream>
#include "GMath.h"
#include "Driver.h"

// The Timer class provides high-resolution timing functionality
class Timer
{
private:
	LARGE_INTEGER freq;   // Frequency of the performance counter
	LARGE_INTEGER start;  // Starting time

public:
	// Constructor that initializes the frequency
	Timer();
	// Resets the timer
	void reset();
	// Returns the elapsed time since the last reset in seconds. Note this should only be called once per frame as it resets the timer.
	float dt();
};

struct KeyState
{
	bool down;
	bool up;
};

class Inputs
{
	bool keys[256];
	KeyState keysState[256];

	bool mouseButtons[3]; // mouse buttons 0 = left, 1 = middle, 2 = right
	KeyState mouseButtonsState[3]; // mouse buttons 0 = left, 1 = middle, 2 = right

	bool isExit; // track if window is closed

	Vec2 mousePos; // mouse position
	Vec2 mouseDelta; // mouse delta in single frame
	RECT screen;

	int mouseWheel; // scroll value

	bool cursorLock; // lock cursor to the center
	bool mouseDirty;

	// reset mouse pos to center of the window
	void ResetCursor();
	// update key state
	void UpdateKey(unsigned int _key, bool _val);
	// update key mouse button state
	void UpdateMouseButton(unsigned int _button, bool _val);
	// update mouse position
	void UpdateMouse(int _x, int _y);
public:

	void Init(HWND& _hwnd);
	// reset all inputs
	void Reset();
	// set cursor lock state
	void SetCursorLock(bool _state);
	// toggle cursor lock
	void ToggleCursorLock();
	// get cursor lock state
	bool GetCursorLock() const;
	// update necessary inputs
	void Update();
	// return W,A,S,D axis
	Vec2 GetAxis();
	// return UP,DOWN,LEFT,RIGHT axis
	Vec2 GetAxis2();
	// check if key is pressed
	bool KeyPressed(int _key) const;
	// check if key is down
	bool KeyDown(int _key) const;
	// check if key is up
	bool KeyUp(int _key) const;
	// check if key is pressed
	bool ButtonPressed(int _button) const;
	// check if key is down
	bool ButtonDown(int _button) const;
	// check if key is up
	bool ButtonUp(int _button) const;
	// returns mouse position
	// returns mouse position
	Vec2 MousePos() const;
	// return mouse delta for this frame
	Vec2 MouseDelta() const;
	// returns mouse scroll delta
	int MouseWheel() const;
	// return if window is closed
	bool Exit() const;

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

	static Window* instance;
public:
	// inputs
	Inputs inputs;

	// constructor creates a window
	Window(unsigned int, unsigned int, std::string, bool = false, unsigned int = 0, unsigned int = 0);
	static Window* GetInstance();
	// processes inouts
	void Update();
	// returns current device
	DXCore& GetDevice();
	~Window();
};

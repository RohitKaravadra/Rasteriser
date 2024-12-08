#include "Window.h"
#include <memory>
#include<sstream>

Window* window;
#define WINDOW_GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define WINDOW_GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		window->inputs.isExit = true;
		return 0;
	}
	case WM_KEYDOWN:
	{
		window->inputs.UpdateKey((unsigned int)wParam, true);
		return 0;
	}
	case WM_KEYUP:
	{
		window->inputs.UpdateKey((unsigned int)wParam, false);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		window->inputs.UpdateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
		window->inputs.mouseWheel += GET_WHEEL_DELTA_WPARAM(wParam);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		window->inputs.UpdateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
		window->inputs.UpdateMouseButton(2, true);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		window->inputs.UpdateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
		window->inputs.UpdateMouseButton(2, false);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		window->inputs.UpdateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
		return 0;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}


Timer::Timer()
{
	QueryPerformanceFrequency(&freq);
	reset();
}

void Timer::reset()
{
	QueryPerformanceCounter(&start);
}

float Timer::dt()
{
	LARGE_INTEGER cur;
	QueryPerformanceCounter(&cur);
	float value = static_cast<float>(cur.QuadPart - start.QuadPart) / freq.QuadPart;
	return value;
}

#pragma region Inputs

void Inputs::Init(HWND& _hwnd)
{
	GetWindowRect(_hwnd, &screen);
	mouseDirty = true;
	Reset();

	ResetCursor();
}

void Inputs::Reset()
{
	memset(keys, 0, 256);
	memset(mouseButtons, 0, 3);

	mousePos = mouseDelta = Vec2(0, 0);
	mouseWheel = 0;
}

void Inputs::ResetCursor()
{
	Vec2 center(screen.left + (screen.right - screen.left) / 2, screen.top + (screen.bottom - screen.top) / 2);
	mousePos = center;
	SetCursorPos(center.x, center.y);
}

void Inputs::SetCursorLock(bool _state)
{
	ShowCursor(!_state);
	cursorLock = _state;
	mouseDelta = Vec2(0, 0);
}

void Inputs::ToggleCursorLock() { SetCursorLock(!cursorLock); }

bool Inputs::GetCursorLock() const { return cursorLock; }

void Inputs::Update()
{
	isExit = false;
	mouseDelta = Vec2(0, 0);
	mouseWheel = 0;
	memset(keysState, 0, 512);
	memset(mouseButtonsState, 0, 6);
}

void Inputs::UpdateKey(unsigned int _key, bool _val)
{
	keysState[_key].down = !keys[_key] && _val;
	keysState[_key].up = keys[_key] && !_val;

	keys[_key] = _val;
}

void Inputs::UpdateMouseButton(unsigned int _button, bool _val)
{
	mouseButtonsState[_button].down = !mouseButtons[_button] && _val;
	mouseButtonsState[_button].up = mouseButtons[_button] && !_val;

	mouseButtons[_button] = _val;
}

void Inputs::UpdateMouse(int _x, int _y)
{
	POINT point;
	GetCursorPos(&point);
	Vec2 curPos(point.x, point.y);

	mouseDelta = curPos - mousePos;
	mousePos = curPos;

	if (cursorLock)
		ResetCursor();
}

Vec2 Inputs::GetAxis()
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

Vec2 Inputs::GetAxis2()
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

bool Inputs::KeyPressed(int _key) const { return keys[_key]; }
bool Inputs::KeyDown(int _key) const { return keysState[_key].down; }
bool Inputs::KeyUp(int _key) const { return keysState[_key].up; }

bool Inputs::ButtonPressed(int _button) const { return mouseButtons[_button]; }
bool Inputs::ButtonDown(int _button) const { return mouseButtonsState[_button].down; }
bool Inputs::ButtonUp(int _button) const { return mouseButtonsState[_button].up; }

Vec2 Inputs::MousePos() const { return mousePos; }
Vec2 Inputs::MouseDelta() const { return mouseDelta; }
int Inputs::MouseWheel() const { return mouseWheel; }
bool Inputs::Exit() const { return isExit; }

#pragma endregion

#pragma region Window

Window* Window::instance = nullptr;

Window::Window(unsigned int _width, unsigned int _height, std::string _name, bool _fullScreen, unsigned int _x, unsigned int _y)
	:name(_name), width(_width), height(_height)
{
	if (instance != nullptr)
		return;

	style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	hinstance = GetModuleHandle(NULL);

	WNDCLASSEX wc{ 0 };
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc; // pointer to the input processing callback
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance;

	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = NULL; // wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	// only use HBRUSH if you are not drawing windows using DirectX (saves performance)
	wc.hbrBackground = NULL; // (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	std::wstring wname = std::wstring(name.begin(), name.end());
	wc.lpszClassName = wname.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);

	// create window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW, wname.c_str(), wname.c_str(), style, _x, _y, width, height, NULL, NULL, hinstance, this);

	// set default values for inputs
	inputs.Reset();

	// set window pointer
	window = this;

	// initialize DriectX driver
	dxDriver.Init(width, height, hwnd, _fullScreen);

	// set inputs
	inputs.Init(hwnd);

	instance = this;
}

Window* Window::GetInstance() { return instance; }

void Window::Update() {

	inputs.Update();

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

DXCore& Window::GetDevice() { return dxDriver; }

Window::~Window() { if (instance == this) instance = nullptr; }

#pragma endregion

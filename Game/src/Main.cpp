#include <Windows.h>

void GameScene();
void TestScene();

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	CreateDirectory(L"Cache", NULL); // create a cache directory if not present to store compiled data

	GameScene();
	//TestScene();

	return 0;
}
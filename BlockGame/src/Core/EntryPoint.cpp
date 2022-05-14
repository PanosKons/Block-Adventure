#include "GameManager.h"
int main()
{
	GameManager::Start();
	return 0;
}
#include <windows.h>
#undef CreateWindow
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PWSTR pCmdLine, int nCmdShow)
{
	GameManager::Start();
	return 0;
}
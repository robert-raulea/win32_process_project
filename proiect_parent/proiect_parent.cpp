#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

using namespace std;

#define ID_SEND 1
#define ID_CANCEL 2

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void AddControls(HWND);

COPYDATASTRUCT cs;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{

	WNDCLASSW wc = { 0 };

	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"myClass";
	wc.lpfnWndProc = WindowProcedure;

	if (!RegisterClassW(&wc))
		return -1;

	CreateWindowW(L"myClass", L"Proces parinte", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 500, 200,
		350, 200, NULL, NULL, NULL, NULL);


	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	HANDLE hProcess;
	HWND hwndReceive;
	HANDLE hThread;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	BOOL bProcess = NULL;
	DWORD lpExitCode;
	char numar[100];
	char numere[100];

	switch (msg)
	{
	case WM_COMMAND:

		switch (wp)
		{
		case ID_CANCEL:
			DestroyWindow(hWnd);
			break;

		case ID_SEND:
			hwndReceive = FindWindowA("Receive", "Proces fiu");

			ifstream File("C:\\proiect.txt");

			while (File >> numar) {
				cs.cbData = strlen(numar);
				cs.lpData = numar;
				SendMessage(hwndReceive, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&cs);
				Sleep(3000);
			}
			WaitForSingleObject(pi.hProcess, INFINITE);
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CREATE:
		AddControls(hWnd);
		bProcess = CreateProcess(L"C:\\Users\\raule\\source\\repos\\proiect_child\\Debug\\proiect_child.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		break;

	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
};

void AddControls(HWND hWnd)
{
	CreateWindowA("button", "Trimite", WS_CHILD | WS_VISIBLE, 50, 50, 100, 50, hWnd, (HMENU)ID_SEND, NULL, NULL);
	CreateWindowA("button", "Cancel", WS_CHILD | WS_VISIBLE, 170, 50, 120, 50, hWnd, (HMENU)ID_CANCEL, NULL, NULL);
}

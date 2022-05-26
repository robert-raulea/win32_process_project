#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#define ID_CANCEL 1
#define NR_LINII 2
#define SUMA 3
#define EC_MULTI 4
#define ID_AFISARE 5

int suma = 0;
int nr_linii = 0;
char szBufferFinalList[200];

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void AddControls(HWND);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{

	WNDCLASSW wc = { 0 };

	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"Receive";
	wc.lpfnWndProc = WindowProcedure;

	if (!RegisterClassW(&wc))
		return -1;

	CreateWindowW(L"Receive", L"Proces fiu", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 700, 300,
		720, 520, NULL, NULL, NULL, NULL);


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
	static char szBufferReceive[100],szBufferFinal[100];

	PCOPYDATASTRUCT pcs;

	switch (msg)
	{
	case WM_COPYDATA:
		pcs = (PCOPYDATASTRUCT)lp;
		strncpy_s(szBufferReceive, (char*)pcs->lpData, pcs->cbData);
		szBufferReceive[pcs->cbData] = '\0';
		strcpy_s(szBufferFinal, szBufferReceive);
		suma += atoi(szBufferFinal);
		nr_linii++;
		strcat_s(szBufferFinalList,szBufferFinal);
		strcat_s(szBufferFinalList,"\r\n");
		SetDlgItemInt(hWnd,NR_LINII, nr_linii, FALSE);
		SetDlgItemInt(hWnd, SUMA, suma, TRUE);
		SetDlgItemTextA(hWnd, EC_MULTI, szBufferFinalList);
		break;
	case WM_COMMAND:

		switch (wp)
		{
		case ID_CANCEL:
			DestroyWindow(hWnd);
			break;
		case ID_AFISARE:
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CREATE:
		AddControls(hWnd);
		break;

	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
};

void AddControls(HWND hWnd)
{
	CreateWindowA("static","Numar de linii",WS_CHILD | WS_VISIBLE, 200, 30, 100, 20, hWnd, NULL,NULL,NULL);
	CreateWindowA("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 310, 30, 100, 20,hWnd,(HMENU)NR_LINII, NULL, NULL);

	CreateWindowA("static", "Suma numerelor", WS_CHILD | WS_VISIBLE, 180, 60, 120, 20, hWnd, NULL, NULL, NULL);
	CreateWindowA("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY, 310, 60, 100, 20, hWnd, (HMENU)SUMA, NULL, NULL);

	CreateWindowA("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, 190, 130, 225, 200, hWnd, (HMENU)EC_MULTI, NULL, NULL);

	CreateWindowA("button", "Cancel", WS_CHILD | WS_VISIBLE, 480, 200, 100, 80, hWnd, (HMENU)ID_CANCEL, NULL, NULL);

}

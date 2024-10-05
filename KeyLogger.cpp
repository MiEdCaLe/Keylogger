#include <Windows.h>
#include <iostream>
#define UNICODE

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK HandleKeyPress(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

wchar_t CurrentMessage[256] = L"";

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

	const wchar_t CLASS_NAME[] = TEXT("KeyLogger");
	WNDCLASS kl = {};
	kl.hInstance = hInstance;
	kl.lpfnWndProc = WindowProc;
	kl.lpszClassName = CLASS_NAME;
	
	RegisterClass(&kl);

	HWND hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		TEXT("KeyLogger"),
		WS_OVERLAPPEDWINDOW,

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, 200, 200,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd == NULL) {
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	// Message Loop
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
	
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_DESTROY:
	{
		PostQuitMessage(0); // Puts a WM_QUIT message on the message queue.
		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps{};
		HDC hdc = BeginPaint(hwnd, &ps);
		RECT rect;
		GetClientRect(hwnd, &rect);
		FillRect(hdc, &rect, (HBRUSH)COLOR_BACKGROUND + 1);
		DrawText(hdc, CurrentMessage, -1, &rect, DT_CENTER);
		ReleaseDC(hwnd, hdc);
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_KEYDOWN:
	{
		HandleKeyPress(hwnd, uMsg, wParam, lParam);
		return 0;
	}
	default: 
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

LRESULT CALLBACK HandleKeyPress(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	
	GetKeyNameTextW(lParam,CurrentMessage,sizeof(CurrentMessage)/sizeof(CurrentMessage[0]));
	InvalidateRect(hwnd, NULL, TRUE); //Request a repaint
	return 0;
}
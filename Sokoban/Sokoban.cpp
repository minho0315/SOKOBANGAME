// Sokoban.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Sokoban.h"
#include "resource.h"

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>


#define MAX_LOADSTRING 100

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define MAXSTAGE 3
//#define putchxy(x,y,c) {gotoxy(x,y);_putch(c);} // _putch() : 출력버퍼를 사용하지 않고 화면에 띠움

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

CHAR ns[18][21];
BOOL nStage;
BOOL nx, ny;
BOOL nMove;

CHAR arStage[MAXSTAGE][18][21] = {

	 {
	 "####################",
	 "####################",
	 "####################",
	 "#####   ############",
	 "#####O  ############",
	 "#####  O############",
	 "###  O O ###########",
	 "### # ## ###########",
	 "#   # ## #####  ..##",
	 "# O  O   @      ..##",
	 "##### ### # ##  ..##",
	 "#####     ##########",
	 "####################",
	 "####################",
	 "####################",
	 "####################",
	 "####################",
	 "####################"
	 },

	 {
	 "####################",
	 "####################",
	 "####################",
	 "####################",
	 "####..  #     ######",
	 "####..  # O  O  ####",
	 "####..  #O####  ####",
	 "####..    @ ##  ####",
	 "####..  # #  O #####",
	 "######### ##O O ####",
	 "###### O  O O O ####",
	 "######    #     ####",
	 "####################",
	 "####################",
	 "####################",
	 "####################",
	 "####################",
	 "####################"
	 },

	 {
	 "####################",
	 "####################",
	 "####################",
	 "####################",
	 "##########     @####",
	 "########## O#O #####",
	 "########## O  O#####",
	 "###########O O #####",
	 "########## O # #####",
	 "##....  ## O  O  ###",
	 "###...    O  O   ###",
	 "##....  ############",
	 "####################",
	 "####################",
	 "####################",
	 "####################",
	 "####################",
	 "####################"
	 },

};

//void gotoxy(int x, int y)
//{
//	COORD Pos = { x, y };
//	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos); //콘솔의 출력핸들을 주고, 위치를 정해주면 커서가 이동하는 것
//}

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_SOKOBAN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SOKOBAN));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOKOBAN));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 1000, 650, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//PAINTSTRUCT ps;
	switch (message)
	{
	case WM_PAINT:
	{
		// 화면 그리기

		// 왼쪽 글자 생성
		HDC hdc = GetDC(hWnd);
		SetTextAlign(hdc, TA_TOP);
		TextOut(hdc, 700, 0, _T("SOKOBAN"), 7);
		TextOut(hdc, 700, 20, _T("Q:종료, R:다시 시작"), 13);
		TextOut(hdc, 700, 40, _T("N:종료, P:이전"), 10);

		TCHAR str[128];
		int Score = 85;
		wsprintf(str, _T("스테이지:%d"), Score);
		TextOut(hdc, 700, 60, str, lstrlen(str));

		wsprintf(str, _T("이동 회수:%d"), Score);
		TextOut(hdc, 700, 80, str, lstrlen(str));


		//화면 DC와 호환(동일색상)되는 메모리 DC를 얻음..
		HDC memDC = CreateCompatibleDC(hdc);
		HBITMAP hBitmap1 = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP_BOX));
		HBITMAP hBitmap2 = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP_HUMAN));
		HBITMAP hBitmap3 = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP_PATH));
		HBITMAP hBitmap4 = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP_POINT));
		HBITMAP hBitmap5 = LoadBitmap(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP_WALL));
		// GetModulHandle(0) ==> current module

		memcpy(ns, arStage[nStage], sizeof(ns));
		int x, y;
		HBITMAP iBitmap = hBitmap1;
		for (y = 0; y < 18; y++) {
			for (x = 0; x < 20; x++) {
				switch (ns[y][x])
				{
				case 'O':
					iBitmap = hBitmap1;
					break;
				case '@':
					iBitmap = hBitmap2;
					break;
				case ' ':
					iBitmap = hBitmap3;
					break;
				case '.':
					iBitmap = hBitmap4;
					break;
				case '#':
					iBitmap = hBitmap5;
					break;

				}
				BITMAP bm;
				// bitmap 정보 추출
				GetObject(iBitmap, sizeof(bm), &bm);
				// 메모리 DC에 비트맵 선택
				SelectObject(memDC, iBitmap);
				// 메모리 DC --> 화면 DC //
				POINTS pt = MAKEPOINTS(lParam);
				BitBlt(hdc, pt.x + 33 * x, pt.y + 33 * y, bm.bmWidth, bm.bmHeight, memDC, 0, 0, SRCCOPY);
			}
		}

		//메모리 제거
		DeleteDC(memDC);
		ReleaseDC(hWnd, hdc);
		DeleteObject(hBitmap1);
		DeleteObject(hBitmap2);
		DeleteObject(hBitmap3);
		DeleteObject(hBitmap4);
		DeleteObject(hBitmap5);
		DeleteObject(iBitmap);


	}
	break;
	case WM_KEYDOWN:
	{

	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hbit)
{

}
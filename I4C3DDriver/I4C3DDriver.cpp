// I4C3DDriver.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "I4C3DDriver.h"
#include "I4C3DModules.h"
#include "F710Modules.h"
#include <ShellAPI.h>

#include <cstdlib>	// 必要

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#define new  ::new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif


#define MAX_LOADSTRING	100
#define MY_NOTIFYICON	(WM_APP+1)
#define TIMER_ID		1

// グローバル変数:
HINSTANCE hInst;								// 現在のインターフェイス
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

static bool g_bUseDirectInputPlugin = false;
static int timer_interval = 50;

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK	DlgProc(HWND, UINT, WPARAM, LPARAM);

namespace {
	HICON g_hMiniIcon = NULL;
	typedef enum { EDIT_MENU, RELOAD_MENU, EXIT_MENU } MENU_ITEMS;

	typedef struct {
		int targetID;
		int keyID;
		double tumbleRate;
		double trackRate;
		double dollyRate;
	} Target3DInfo;
	Target3DInfo g_targetInfo = {0};

	TCHAR *g_szTargetName[] = {
		_T("RTT"),
		_T("Maya"),
		_T("Alias"),
		_T("Showcase"),
	};
	TCHAR *g_szKeyHandling[] = {
		_T("Ctrl"),
		_T("Alt"),
		_T("Shift"),
		_T("Ctrl+Alt"),
		_T("Ctrl+Shift"),
		_T("Alt+Shift"),
		_T("Ctrl+Alt+Shift"),
	};
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#if DEBUG || _DEBUG
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

 	// TODO: ここにコードを挿入してください。
	MSG msg;
	HACCEL hAccelTable;

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_I4C3DDRIVER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	int argc = 0;
	LPTSTR *argv = NULL;
	argv = CommandLineToArgvW(GetCommandLine(), &argc);
	if (argc != 2) {
		MessageBox(NULL, _T("[ERROR] 引数が足りません[例: I4C3DDriver.exe ON]。<I4C3DDriver>"), szTitle, MB_OK | MB_ICONERROR);
		LocalFree(argv);
		return EXIT_FAILURE;
	}
	g_bUseDirectInputPlugin = _tcsicmp(argv[1], _T("on")) == 0;
	LocalFree(argv);

	// アプリケーションの初期化を実行します:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_I4C3DDRIVER));

	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

#if DEBUG || _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return (int) msg.wParam;
}

//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
//  コメント:
//
//    この関数および使い方は、'RegisterClassEx' 関数が追加された
//    Windows 95 より前の Win32 システムと互換させる場合にのみ必要です。
//    アプリケーションが、関連付けられた
//    正しい形式の小さいアイコンを取得できるようにするには、
//    この関数を呼び出してください。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_I4C3DDRIVER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_I4C3DDRIVER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 300, 200, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   //ShowWindow(hWnd, nCmdShow);
   //UpdateWindow(hWnd);

   return TRUE;
}

inline void SetTaskTrayIcon(HWND hWnd) {
	HMENU hMenu = NULL;
	MENUITEMINFO menuItem_edit, menuItem_reload, menuItem_exit;
	POINT pos;
	ZeroMemory(&menuItem_edit, sizeof(menuItem_edit));
	ZeroMemory(&menuItem_reload, sizeof(menuItem_reload));
	ZeroMemory(&menuItem_exit, sizeof(menuItem_exit));

	menuItem_edit.cbSize		= sizeof(menuItem_edit);
	menuItem_edit.fMask			= MIIM_STRING | MIIM_ID;
	menuItem_edit.wID			= EDIT_MENU;
	menuItem_edit.dwTypeData	= _T("設定ファイルを編集");
	menuItem_edit.cch			= _tcslen(menuItem_edit.dwTypeData);

	menuItem_reload.cbSize		= sizeof(menuItem_reload);
	menuItem_reload.fMask		= MIIM_STRING | MIIM_ID;
	menuItem_reload.wID			= RELOAD_MENU;
	menuItem_reload.dwTypeData	= _T("リロード");
	menuItem_reload.cch			= _tcslen(menuItem_reload.dwTypeData);

	menuItem_exit.cbSize		= sizeof(menuItem_exit);
	menuItem_exit.fMask			= MIIM_STRING | MIIM_ID;
	menuItem_exit.wID			= EXIT_MENU;
	menuItem_exit.dwTypeData	= _T("終了");
	menuItem_exit.cch			= _tcslen(menuItem_exit.dwTypeData);

	GetCursorPos(&pos);
	SetForegroundWindow(hWnd);
	hMenu = CreatePopupMenu();
	InsertMenuItem(hMenu, EDIT_MENU, TRUE, &menuItem_edit);
	//InsertMenuItem(hMenu, RELOAD_MENU, TRUE, &menuItem_reload);
	InsertMenuItem(hMenu, EXIT_MENU, TRUE, &menuItem_exit);
	TrackPopupMenuEx(hMenu, TPM_LEFTALIGN, pos.x, pos.y, hWnd, 0);
	DestroyMenu(hMenu);
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:  メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	TCHAR szFileName[MAX_PATH] = {0};
	static NOTIFYICONDATA nIcon = {0};
	static int sw = 1;

	switch (message)
	{
	case WM_CREATE:
		I4C3DStart(_T("I4C3D.xml"));
		if (g_bUseDirectInputPlugin) {
			if (!F710Start(_T("I4C3D.xml"), hInst, hWnd)) {
				MessageBox(hWnd, _T("[ERROR] DIrectInputの初期化に失敗しました。終了します。"), szTitle, MB_OK | MB_ICONERROR);
				PostMessage(hWnd, WM_CLOSE, 0, 0);
				return 0;
			}
		}

		if (!GetModuleFileName(NULL, szFileName, _countof(szFileName))) {
			MessageBox(hWnd, _T("[ERROR] 実行モジュール名の取得に失敗しました。終了します。"), szTitle, MB_OK | MB_ICONERROR);
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			return 0;
		}

		ExtractIconEx(szFileName, 0, NULL, &g_hMiniIcon, 1);
		nIcon.cbSize = sizeof(g_hMiniIcon);
		nIcon.uID = 1;
		nIcon.hWnd = hWnd;
		nIcon.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		nIcon.uCallbackMessage = MY_NOTIFYICON;
		nIcon.hIcon = g_hMiniIcon;
		_tcscpy_s(nIcon.szTip, szTitle);
		Shell_NotifyIcon(NIM_ADD, &nIcon);

		// DirectInputのために追記
		SetTimer(hWnd, TIMER_ID, timer_interval, NULL);
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case EDIT_MENU:
			ShellExecute(NULL, _T("open"), _T("notepad.exe"), _T("I4C3D.xml"), NULL, SW_SHOW);
			break;
		case RELOAD_MENU:
			if (g_bUseDirectInputPlugin) {
				F710Stop();
			}
			I4C3DStop();
			I4C3DStart(_T("I4C3D.xml"));
			if (g_bUseDirectInputPlugin) {
				F710Start(_T("I4C3D.xml"), hInst, hWnd);
			}
			break;
		case EXIT_MENU:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case MY_NOTIFYICON:
		switch (lParam) {
		case WM_LBUTTONDBLCLK:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
			break;

		case WM_RBUTTONDOWN:
			SetTaskTrayIcon(hWnd);
			break;
		}
		break;

	case WM_TIMER:
		if (g_bUseDirectInputPlugin) {
			F710CheckInput();
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 描画コードをここに追加してください...
		EndPaint(hWnd, &ps);
		break;

	case WM_CLOSE:
	case WM_DESTROY:
		I4C3DStop();
		if (g_bUseDirectInputPlugin) {
			F710Stop();
		}
		if (g_hMiniIcon != NULL) {
			Shell_NotifyIcon(NIM_DELETE, &nIcon);
			DestroyIcon(g_hMiniIcon);
		}
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

inline void InitTargetComboBox(HWND hTargetComboBox, int select) {
	int targetCount = _countof(g_szTargetName);
	if (select < 0 || targetCount <= select) {
		select = 0;
	}
	for (int i = 0; i < targetCount; ++i) {
		SendMessage(hTargetComboBox, CB_ADDSTRING, 0, (LPARAM)g_szTargetName[i]);
	}
	// ウィンドウ生成時に表示するデータ
	WPARAM index = SendMessage(hTargetComboBox, CB_FINDSTRINGEXACT, -1, (LPARAM)g_szTargetName[select]);
	SendMessage(hTargetComboBox, CB_SETCURSEL, index, 0);
}

inline void InitKeyComboBox(HWND hKeyComboBox, int select) {
	int targetCount = _countof(g_szKeyHandling);
	if (select < 0 || targetCount <= select) {
		select = 0;
	}
	for (int i = 0; i < targetCount; ++i) {
		SendMessage(hKeyComboBox, CB_ADDSTRING, 0, (LPARAM)g_szKeyHandling[i]);
	}
	// ウィンドウ生成時に表示するデータ
	WPARAM index = SendMessage(hKeyComboBox, CB_FINDSTRINGEXACT, -1, (LPARAM)g_szKeyHandling[select]);
	SendMessage(hKeyComboBox, CB_SETCURSEL, index, 0);
}

LRESULT CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	static HWND hTargetComboBox = NULL;
	static HWND hKeyComboBox = NULL;
	TCHAR szBuffer[MAX_LOADSTRING] = {0};

	switch (message)
	{
	case WM_INITDIALOG:
		hTargetComboBox = GetDlgItem(hDlg, IDC_COMBO_TARGET);
		hKeyComboBox = GetDlgItem(hDlg, IDC_COMBO_KEY);

		InitTargetComboBox(hTargetComboBox, 0);
		InitKeyComboBox(hKeyComboBox, 0);
		SetDlgItemText(hDlg, IDC_EDIT_TUMBLE, _T("1.0"));
		SetDlgItemText(hDlg, IDC_EDIT_TRACK, _T("1.0"));
		SetDlgItemText(hDlg, IDC_EDIT_DOLLY, _T("1.0"));
		return (LRESULT)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			// タスクトレイへ

			// ターゲット情報取得
			if (hTargetComboBox != NULL) {
				g_targetInfo.targetID = (INT)SendMessage(hTargetComboBox, CB_GETCURSEL, 0, 0);
				g_targetInfo.keyID = (INT)SendMessage(hKeyComboBox, CB_GETCURSEL, 0, 0);
				GetDlgItemText(hDlg, IDC_EDIT_TUMBLE, szBuffer, _countof(szBuffer));
				g_targetInfo.tumbleRate = _tstof(szBuffer);
				GetDlgItemText(hDlg, IDC_EDIT_TRACK, szBuffer, _countof(szBuffer));
				g_targetInfo.trackRate = _tstof(szBuffer);
				GetDlgItemText(hDlg, IDC_EDIT_DOLLY, szBuffer, _countof(szBuffer));
				g_targetInfo.dollyRate = _tstof(szBuffer);
			}
			
			break;

		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		}
		return (LRESULT)TRUE;
	}
	return (LRESULT)FALSE;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

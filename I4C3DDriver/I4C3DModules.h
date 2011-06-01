#ifndef _I4C3DModules_
#define _I4C3DModules_

#pragma comment(linker, "/export:I4C3DStart=_I4C3DStart@8")
#pragma comment(linker, "/export:I4C3DStop=_I4C3DStop@0")

#include <Windows.h>

#ifdef I4C3DMODULES_EXPORTS
#define DLL_EXPORT		__declspec(dllexport)
#else
#define DLL_EXPORT		__declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

	// szXMLUriには設定ファイルのパスを、szTargetWindowTitleにはターゲットの3Dソフトウェアのウィンドウタイトル(の一部)を指定してください。
	// szTargetWindowTitleがNULLの場合は設定ファイルに記載されているwindow_titleタグの値が含まれるウィンドウを検索します。
	DLL_EXPORT BOOL WINAPI I4C3DStart(PCTSTR szXMLUri, PCTSTR szTargetWindowTitle);
	DLL_EXPORT void WINAPI I4C3DStop(void);

#ifdef __cplusplus
}
#endif

#endif /* _I4C3DModules_ */

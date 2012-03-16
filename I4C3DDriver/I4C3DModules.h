#ifndef _I4C3DModules_
#define _I4C3DModules_

#pragma comment(linker, "/export:I4C3DStart=_I4C3DStart@4")
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

	// szXMLUri�ɂ͐ݒ�t�@�C���̃p�X���w�肵�Ă��������B
	DLL_EXPORT BOOL WINAPI I4C3DStart(PCTSTR szXMLUri);
	// �v���O�����̏I���ɕK���Ă�ł��������i���������[�N�ɂȂ���܂��j�B
	DLL_EXPORT void WINAPI I4C3DStop(void);

#ifdef __cplusplus
}
#endif

#endif /* _I4C3DModules_ */

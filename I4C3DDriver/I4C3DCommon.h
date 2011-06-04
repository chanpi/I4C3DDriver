#ifndef _I4C3DCOMMON_H_
#define _I4C3DCOMMON_H_

#define MY_I4C3DDESTROY		(WM_APP+1)
#define MY_I4C3DREBOOT		(WM_APP+2)
#define MY_WINSOCKSELECT	(WM_APP+3)

#define I4C3D_RECEIVE_LENGTH	1024

// init [修飾キー] [Tumble Rate] [Track Rate] [Dolly Rate] [終端文字]\0
// init Alt+Shift 1.0 1.0 0.5 ?\0
static const char* g_initCommandFormat = "%s %s %f %f %f %c";

typedef struct {
	unsigned char hwnd[4];
	char szCommand[I4C3D_RECEIVE_LENGTH];
} I4C3DUDPPacket;

#endif	/* _I4C3DCOMMON_H_ */
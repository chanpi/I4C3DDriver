#ifndef _I4C3DCOMMON_H_
#define _I4C3DCOMMON_H_

#define MY_I4C3DDESTROY			(WM_APP+1)
#define MY_I4C3DREBOOT			(WM_APP+2)
#define MY_WINSOCKSELECT		(WM_APP+3)

#define I4C3D_RECEIVE_LENGTH	1024

#define SHARED_LOG_FILE_NAME		"Hamster"			// 拡張子は日付(ファイル名)とともにdllが自動的に付与します
#define SHARED_LOGINFO_FILE_NAME	"HamsterInfo"		// 拡張子は日付(ファイル名)とともにdllが自動的に付与します
#define SHARED_XML_FILE				_T("Hamster.xml")

// init [修飾キー] [Tumble Rate] [Track Rate] [Dolly Rate] [終端文字]\0
// init Alt+Shift 1.0 1.0 0.5 ?\0
static const char* g_initCommandFormat = "%s %s %lf %lf %lf %c";

// registermacro [マクロ名] [キーの組み合わせ] [終端文字]\0
// registermacro Macro1 1?\0
// registermacro Macro1 "ANIM anim1 START"?\0
static const char* g_registerMacroFormat = "%s %s %s %c";

typedef struct {
	unsigned char hwnd[4];
	char szCommand[I4C3D_RECEIVE_LENGTH];
} I4C3DUDPPacket;

const int MAX_MACROS = 32;

const int I4C3DAlpha = 1;
const int cancelKeyDownMillisec = 1000;
const int waitModkeyDownCount	= 100;

// コアとプラグイン間のの通知フォーマット
typedef struct {
	unsigned char bodyHeight[4];
	// データが増える場合にはここに追加
} NotifyDataFormat;

#endif	/* _I4C3DCOMMON_H_ */

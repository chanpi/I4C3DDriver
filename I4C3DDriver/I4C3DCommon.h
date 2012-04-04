#ifndef _I4C3DCOMMON_H_
#define _I4C3DCOMMON_H_

#define MY_I4C3DDESTROY			(WM_APP+1)
#define MY_I4C3DREBOOT			(WM_APP+2)
#define MY_WINSOCKSELECT		(WM_APP+3)

#define I4C3D_RECEIVE_LENGTH	1024

#define SHARED_LOG_FILE_NAME		"Hamster"			// �g���q�͓��t(�t�@�C����)�ƂƂ���dll�������I�ɕt�^���܂�
#define SHARED_LOGINFO_FILE_NAME	"HamsterInfo"		// �g���q�͓��t(�t�@�C����)�ƂƂ���dll�������I�ɕt�^���܂�
#define SHARED_XML_FILE				_T("Hamster.xml")

// init [�C���L�[] [Tumble Rate] [Track Rate] [Dolly Rate] [�I�[����]\0
// init Alt+Shift 1.0 1.0 0.5 ?\0
static const char* g_initCommandFormat = "%s %s %lf %lf %lf %c";

// registermacro [�}�N����] [�L�[�̑g�ݍ��킹] [�I�[����]\0
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

// �R�A�ƃv���O�C���Ԃ̂̒ʒm�t�H�[�}�b�g
typedef struct {
	unsigned char bodyHeight[4];
	// �f�[�^��������ꍇ�ɂ͂����ɒǉ�
} NotifyDataFormat;

#endif	/* _I4C3DCOMMON_H_ */

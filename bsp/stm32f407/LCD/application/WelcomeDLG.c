#include <stdio.h>
#include "DIALOG.h"
#include "bitmap.h"
#include "skin\skin.h"
#include "time.h"
#include "cmd_a.h"

#define P1_SkinENA	(1)
extern WM_HWIN hPage[4];

#define ID_WINDOW_0		(GUI_ID_USER + 0x00)
#define ID_TEXT_0		(GUI_ID_USER + 0x01)
#define ID_EDIT_0		(GUI_ID_USER + 0x02)
#define ID_DROPDOWN_0	(GUI_ID_USER + 0x03)
#define ID_BUTTON_0		(GUI_ID_USER + 0x04)
#define ID_BUTTON_1		(GUI_ID_USER + 0x05)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
	{ WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 800, 600, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Text", ID_TEXT_0, 0, 0, 320, 12, 0, 0x0, 0 },
	{ EDIT_CreateIndirect, "Edit", ID_EDIT_0, 380, 318, 150, 22, 0, 0x64, 0 },
	{ DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_0, 380, 279, 150, 18, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_0, 280, 380, 100, 31, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_1, 430, 380, 100, 31, 0, 0x0, 0 },
};

static void GUI_renew_gui_memory(WM_HWIN hParent, int textID)
{
	char buff[48];
	sprintf(buff, "Memory used: %dKBytes  free: %dKBytes", GUI_ALLOC_GetNumUsedBytes()/1024, GUI_ALLOC_GetNumFreeBytes()/1024);
	//TEXT_SetTextColor(WM_GetDialogItem(hParent, textID), (0x008000FF));
	TEXT_SetText(WM_GetDialogItem(hParent, textID), buff);
}

extern GUI_CONST_STORAGE GUI_FONT GBK_16m16;	//�ֿ�����
extern GUI_CONST_STORAGE GUI_FONT GBK_64m64;	//�ֿ�����

#include "glcd.h"
static void _cbDialog(WM_MESSAGE * pMsg) {		//����ص�����
	WM_HWIN hItem;
	int NCode;
	int Id;
	static int movCNT = 0, prgCNT[2] = {0, 0};
	static unsigned char buttonS = 1;

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG://��ʼ����Ϣ,��������/�ؼ�ʱ��Ч,��������������һЩ�ؼ��ĳ�ʼ����

		//'Edit'
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
	  EDIT_SetBkColor(hItem, 1,(0x00E3E4E6));
		EDIT_SetText(hItem, "Password");
		EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);

			//'Dropdown'
		hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
		DROPDOWN_SetListHeight(hItem, 40);
		DROPDOWN_SetTextHeight(hItem, 18);
		DROPDOWN_SetTextAlign(hItem, GUI_TA_VCENTER);
		DROPDOWN_SetFont(hItem, &GBK_16m16);	
		DROPDOWN_AddString(hItem, "����Ա");	
		DROPDOWN_AddString(hItem, "����Ա");
//		if(P1_SkinENA) DROPDOWN_SetSkin(hItem, SKIN_dropdown2C);

		//'Button'
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);	//��ȡID_BUTTON_0�ľ��
		//BUTTON_SetTextColor(hItem, 0, GUI_WHITE);			//�޸�ID_BUTTON_0Ϊ��ɫ����
		BUTTON_SetFont(hItem, &GBK_16m16);					//�޸�ID_BUTTON_0���ֿ�
//		BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED, GUI_BLUE);
		BUTTON_SetText(hItem, "�� ¼");
//		if(P1_SkinENA) BUTTON_SetSkin(hItem, SKIN_button2C);

		//'Button'
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
		BUTTON_SetFont(hItem, &GBK_16m16);
//		BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED, GUI_BLUE);
		BUTTON_SetText(hItem, "�� ��");
//		if(P1_SkinENA) BUTTON_SetSkin(hItem, SKIN_button2C);

		GUI_renew_gui_memory(pMsg->hWin, ID_TEXT_0);//�����ڴ�ʹ�����,ע:���ڴ�仯�ĵط�Ҳ�����.
		break;
	case WM_PAINT://�����ػ���Ϣ,����Ƚ���˵����,������Framewin ��Window ����֮������һ�����ÿؼ�,���Ҫ��Framewin ��Window ����֮����ʾ���ֻ����ֱ�ߡ����Ρ�Բ��������ʵ��
		GUI_DrawBitmap(&bmwelcome_n, 0, 0);	//�ػ汳��ͼƬ
		break;
	case WM_TIMER://��ʱ����Ϣ,��ʱ��ʱ��Ч
		WM_RestartTimer(pMsg->Data.v, 100);
	case WM_NOTIFY_PARENT://����������Ϣ����(������Ļ������ܵ�����),����������������༭��(�κεĲ���)�ȵ�......
		Id		= WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) {
		case ID_EDIT_0:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				break;
			}
			break;
	case ID_DROPDOWN_0:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				break;
			case WM_NOTIFICATION_SEL_CHANGED:
				break;
			}
			break;

		case ID_BUTTON_0:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:	//���¶�����Ϣ
//					CMDA_backlight_setting(6);
				break;
			case WM_NOTIFICATION_RELEASED:	//��������Ϣ
				WM_HideWindow(hPage[0]);	//����hPage[0]
				WM_ShowWindow(hPage[1]);	//��ʾhPage[1]
				WM_SetFocus(hPage[1]);		//emWin��Ѱ�Ҳ��۽���hPage[1]����ĵ�1���ܾ۽���С�ؼ�
				CMDA_buzzer_beep(60);
				break;
			}
			break;
		case ID_BUTTON_1:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
				EDIT_SetText(hItem, "");
				CMDA_buzzer_beep(60);		
				break;
			}
			break;

			}
			break;
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}


WM_HWIN CreateWelcomeDLG(void);
WM_HWIN CreateWelcomeDLG(void) {
	WM_HWIN hWin;
	static WIN_BITMAP BMP_WelcomeDLG; WIN_BITMAP *pBMP_WelcomeDLG = &BMP_WelcomeDLG;

	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	WM_CreateTimer(hWin, 1, 100, 0);//����һ�������ʱ��

	if(!P1_SkinENA) return hWin;
	//����ͬ״̬��ҳ��BMPλͼ��ֵ��"ҳ��ͼƬ�ṹ��"(���г�Ա��Ҫ��ֵ)
	BMP_WelcomeDLG.normal = &bmwelcome_n;
	BMP_WelcomeDLG.mark = &bmwelcome_m;
	BMP_WelcomeDLG.focus = &bmwelcome_n;
	BMP_WelcomeDLG.disable = &bmwelcome_m;
	BMP_WelcomeDLG.thumbN = &bmwelcome_n;
	BMP_WelcomeDLG.thumbM = &bmwelcome_m;
	WM_SetUserData(hWin, &pBMP_WelcomeDLG, sizeof(pBMP_WelcomeDLG));//��ָ��д���û�������
	return hWin;
}


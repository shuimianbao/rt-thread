#include "DIALOG.h"
#include "bitmap.h"
#include "skin\skin.h"
#include "cmd_a.h"

#define SKB_SkinENA		(1)
extern WM_HWIN hPage[5];

#define ID_WINDOW_0  (GUI_ID_USER + 0x00)
#define ID_BUTTON_0  (GUI_ID_USER + 0x01)
#define ID_BUTTON_1  (GUI_ID_USER + 0x02)
#define ID_BUTTON_2  (GUI_ID_USER + 0x03)
#define ID_BUTTON_3  (GUI_ID_USER + 0x04)
#define ID_BUTTON_4  (GUI_ID_USER + 0x05)
#define ID_BUTTON_5  (GUI_ID_USER + 0x06)
#define ID_BUTTON_6  (GUI_ID_USER + 0x07)
#define ID_BUTTON_7  (GUI_ID_USER + 0x08)
#define ID_BUTTON_8  (GUI_ID_USER + 0x09)
#define ID_BUTTON_9  (GUI_ID_USER + 0x0A)
#define ID_BUTTON_10  (GUI_ID_USER + 0x0B)
#define ID_BUTTON_11  (GUI_ID_USER + 0x0C)
#define ID_BUTTON_12  (GUI_ID_USER + 0x0D)
//#define ID_EDIT_0  (GUI_ID_USER + 0x15)
//#define ID_TEXT_0  (GUI_ID_USER + 0x16)
//#define ID_CHECKBOX_0  (GUI_ID_USER + 0x17)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 445, 240, 303, 238, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_0, 10, 10, 85, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_1, 110, 10, 85, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_2, 210, 10, 85, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_3, 10, 67, 85, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_4, 110, 67, 85, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_5, 210, 67, 85, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_6, 10, 122, 85, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_7, 110, 122, 85, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_8, 210, 122, 85, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_9, 10, 176, 85, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_10, 110, 176, 85, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_11, 200, 176, 45, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "", ID_BUTTON_12, 250, 176, 45, 50, 0, 0x0, 0 },
};

static void _cbDialog(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;
	int NCode;
	int Id;
	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
		for(Id=0; Id<=12; Id++) {
			hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0+Id);
			if(SKB_SkinENA) BUTTON_SetText(hItem, "");
			if(SKB_SkinENA) BUTTON_SetSkin(hItem, SKIN_button2C); //设置BUTTON控件的位图皮肤(Skinning方式)
		}
		break;
	case WM_PAINT:
		GUI_DrawBitmap(&bmskb_n, 0, 0);
		break;
	case WM_NOTIFY_PARENT:
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) {
		case ID_BUTTON_0:
		case ID_BUTTON_1:
		case ID_BUTTON_2:
		case ID_BUTTON_3:
		case ID_BUTTON_4:
		case ID_BUTTON_5:
		case ID_BUTTON_6:
		case ID_BUTTON_7:
		case ID_BUTTON_8:
		case ID_BUTTON_9:
		case ID_BUTTON_10:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
				case WM_NOTIFICATION_RELEASED:
	//				EDIT_SetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_0), Id-ID_BUTTON_0);
				break;
				}
			break;
		case ID_BUTTON_11:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
				case WM_NOTIFICATION_RELEASED:
	//				EDIT_SetValue(WM_GetDialogItem(pMsg->hWin, ID_EDIT_0), Id-ID_BUTTON_0);
				break;
				}
			break;
		case ID_BUTTON_12:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
			break;
			case WM_NOTIFICATION_RELEASED:
				WM_HideWindow(hPage[4]);
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

WM_HWIN CreateWindowSKB(WM_HWIN hParent);
WM_HWIN CreateWindowSKB(WM_HWIN hParent) {
	WM_HWIN hWin;
	static WIN_BITMAP BMP_WindowSKB; WIN_BITMAP *pBMP_WindowSKB = &BMP_WindowSKB;

	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hParent, 0, 0);

	if(!SKB_SkinENA) return hWin;
	//将不同状态的页面BMP位图赋值给"页面图片结构体"(所有成员都要赋值)
	BMP_WindowSKB.normal = &bmskb_n;
	BMP_WindowSKB.mark = &bmskb_m;
	BMP_WindowSKB.focus = &bmskb_n;
	BMP_WindowSKB.disable = &bmskb_n;
	BMP_WindowSKB.thumbN = &bmskb_n;
	BMP_WindowSKB.thumbM = &bmskb_n;
	WM_SetUserData(hWin, &pBMP_WindowSKB, sizeof(pBMP_WindowSKB));//将指针写到用户数据区

	return hWin;
}


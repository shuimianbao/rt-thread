#include "DIALOG.h"
#include "cmd_a.h"

#define D1_TransENA	(0)

extern WM_HWIN hPage[4];
extern char x[];

#define ID_WINDOW_0  (GUI_ID_USER + 0x00)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
	{ WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 800, 600, 0, 0x0, 0 },
};

static void _cbWindowSTB(WM_MESSAGE * pMsg)
{
	switch(pMsg->MsgId)
	{
	case WM_PAINT:
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetColor(GUI_RED);
		GUI_DispStringAt("stand-by screen", 600/2-8*6, 24);
		break;
	default:
		WINDOW_Callback(pMsg);
		break;
	}
}

static void _cbDialog(WM_MESSAGE * pMsg) {
	static int TimeD = 0;
	GUI_PID_STATE statePID;
	GUI_PID_GetState(&statePID);
	if(statePID.Pressed) TimeD = 0;

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG:
		WINDOW_SetBkColor(pMsg->hWin, (0x00C0C0C0));
		//WM_SetHasTrans(pMsg->hWin);
		if(D1_TransENA) WM_SetCallback(pMsg->hWin, _cbWindowSTB);
		break;
	case WM_PAINT:
		GUI_SetTextMode(GUI_TM_TRANS);
		GUI_SetColor(GUI_BLUE);
		GUI_DispStringAt(x, 600/2-8*6, 24);
		break;
	case WM_TOUCH:
		WM_HideWindow(hPage[3]);
		WM_ShowWindow(hPage[1]);
		CMDA_backlight_setting(12);
		break;
	case WM_TIMER://定时器消息,到时间时有效
		WM_RestartTimer(pMsg->Data.v, 100);
		if(TimeD<=30000) TimeD++;
		if(TimeD!=30000) break;

		//stand-by active code here
		WM_HideWindow(hPage[1]);
		WM_ShowWindow(hPage[3]);
	  CMDA_backlight_setting(5);
		break;
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}

WM_HWIN CreateStandbyDLG(void);
WM_HWIN CreateStandbyDLG(void) {
	WM_HWIN hWin;
	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	WM_CreateTimer(hWin, 0, 100, 0); //创建一个软件定时器
	return hWin;
}


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

extern GUI_CONST_STORAGE GUI_FONT GBK_16m16;	//字库声明
extern GUI_CONST_STORAGE GUI_FONT GBK_64m64;	//字库声明

#include "glcd.h"
static void _cbDialog(WM_MESSAGE * pMsg) {		//窗体回调函数
	WM_HWIN hItem;
	int NCode;
	int Id;
	static int movCNT = 0, prgCNT[2] = {0, 0};
	static unsigned char buttonS = 1;

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG://初始化消息,创建窗口/控件时有效,比如在这里设置一些控件的初始参数

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
		DROPDOWN_AddString(hItem, "操作员");	
		DROPDOWN_AddString(hItem, "管理员");
//		if(P1_SkinENA) DROPDOWN_SetSkin(hItem, SKIN_dropdown2C);

		//'Button'
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);	//获取ID_BUTTON_0的句柄
		//BUTTON_SetTextColor(hItem, 0, GUI_WHITE);			//修改ID_BUTTON_0为白色文字
		BUTTON_SetFont(hItem, &GBK_16m16);					//修改ID_BUTTON_0的字库
//		BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED, GUI_BLUE);
		BUTTON_SetText(hItem, "登 录");
//		if(P1_SkinENA) BUTTON_SetSkin(hItem, SKIN_button2C);

		//'Button'
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
		BUTTON_SetFont(hItem, &GBK_16m16);
//		BUTTON_SetTextColor(hItem, BUTTON_CI_UNPRESSED, GUI_BLUE);
		BUTTON_SetText(hItem, "重 置");
//		if(P1_SkinENA) BUTTON_SetSkin(hItem, SKIN_button2C);

		GUI_renew_gui_memory(pMsg->hWin, ID_TEXT_0);//更新内存使用情况,注:在内存变化的地方也需调用.
		break;
	case WM_PAINT://窗口重绘消息,这个比较难说明白,反正在Framewin 或Window 窗体之中我们一般是用控件,如果要在Framewin 或Window 窗体之中显示文字或绘制直线、矩形、圆等在这里实现
		GUI_DrawBitmap(&bmwelcome_n, 0, 0);	//重绘背景图片
		break;
	case WM_TIMER://定时器消息,到时间时有效
		WM_RestartTimer(pMsg->Data.v, 100);
	case WM_NOTIFY_PARENT://操作触发消息处理(操作屏幕程序会跑到这里),比如点击按键、点击编辑框(任何的操作)等等......
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
			case WM_NOTIFICATION_CLICKED:	//按下动作消息
//					CMDA_backlight_setting(6);
				break;
			case WM_NOTIFICATION_RELEASED:	//弹起动作消息
				WM_HideWindow(hPage[0]);	//隐藏hPage[0]
				WM_ShowWindow(hPage[1]);	//显示hPage[1]
				WM_SetFocus(hPage[1]);		//emWin会寻找并聚焦到hPage[1]窗体的第1个能聚焦的小控件
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
	WM_CreateTimer(hWin, 1, 100, 0);//创建一个软件定时器

	if(!P1_SkinENA) return hWin;
	//将不同状态的页面BMP位图赋值给"页面图片结构体"(所有成员都要赋值)
	BMP_WelcomeDLG.normal = &bmwelcome_n;
	BMP_WelcomeDLG.mark = &bmwelcome_m;
	BMP_WelcomeDLG.focus = &bmwelcome_n;
	BMP_WelcomeDLG.disable = &bmwelcome_m;
	BMP_WelcomeDLG.thumbN = &bmwelcome_n;
	BMP_WelcomeDLG.thumbM = &bmwelcome_m;
	WM_SetUserData(hWin, &pBMP_WelcomeDLG, sizeof(pBMP_WelcomeDLG));//将指针写到用户数据区
	return hWin;
}


#include <stdio.h>
#include "DIALOG.h"
#include "bitmap.h"
#include "skin\skin.h"
#include "cmd_a.h"

#define P2_SkinENA		(1)
extern WM_HWIN hPage[5];

#define ID_WINDOW_0		 (GUI_ID_USER + 0x00)
#define ID_TEXT_0		   (GUI_ID_USER + 0x01)
#define ID_MULTIPAGE_0 (GUI_ID_USER + 0x02)
#define ID_BUTTON_0		 (GUI_ID_USER + 0x04)
#define ID_SCROLLBAR_0 (GUI_ID_USER + 0x05)
#define ID_SLIDER_0		 (GUI_ID_USER + 0x06)
#define ID_EDIT_1		   (GUI_ID_USER + 0x07)
#define ID_EDIT_2		   (GUI_ID_USER + 0x08)
#define ID_EDIT_3		   (GUI_ID_USER + 0x09)
#define ID_EDIT_4		   (GUI_ID_USER + 0x10)
#define ID_EDIT_5		   (GUI_ID_USER + 0x11)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
	{ WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 800, 600, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Text", ID_TEXT_0, 0, 0, 320, 12, 0, 0x0, 0 },
	{ MULTIPAGE_CreateIndirect, "Multipage", ID_MULTIPAGE_0, 14, 30, 448, 146, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_0, 40, 520, 70, 70, 0, 0x0, 0 },
	{ SLIDER_CreateIndirect, "Slider", ID_SLIDER_0, 100, 410, 350, 25, 0, 0x0, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_1, 214, 132, 92, 35, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_2, 566, 134, 92, 35, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_3, 214, 182, 92, 35, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_4, 566, 183, 92, 35, 0, 0x64, 0 },
	{ EDIT_CreateIndirect, "", ID_EDIT_5, 212, 236, 92, 35, 0, 0x64, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aPage0[] = {
	{ WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 448, 146, 0, 0x0, 0 },
};
static const GUI_WIDGET_CREATE_INFO _aPage1[] = {
	{ WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 448, 146, 0, 0x0, 0 },
};
static const GUI_WIDGET_CREATE_INFO _aPage2[] = {
	{ WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 448, 146, 0, 0x0, 0 },
};
static const GUI_WIDGET_CREATE_INFO _aPage3[] = {
	{ WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 448, 146, 0, 0x0, 0 },
};
static void _cbMultipage(WM_MESSAGE * pMsg)	//Multipage控件的PAGE回调函数
{
	switch (pMsg->MsgId) {
	case WM_PAINT:
		if(P2_SkinENA) SKIN_multipage_paint4C(pMsg->hWin);
		break;
	default: WM_DefaultProc(pMsg); break;
	}
}

static void GUI_renew_gui_memory(WM_HWIN hParent, int textID)
{
	char buff[48];
	sprintf(buff, "Memory used: %dKBytes  free: %dKBytes", GUI_ALLOC_GetNumUsedBytes()/1024, GUI_ALLOC_GetNumFreeBytes()/1024);
	//TEXT_SetTextColor(WM_GetDialogItem(hParent, textID), (0x008000FF));
	TEXT_SetText(WM_GetDialogItem(hParent, textID), buff);
}

#include "glcd.h"
static void _cbDialog(WM_MESSAGE * pMsg)
{
	WM_HWIN hItem, hWin;
	int 		NCode;
	int 		Id;
	static int movCNT = 0, prgCNT = 0;
	static unsigned char buttonS[2] = {0, 0};
  int lightValue;
	char editvalue[20];
	
	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG://初始化消息,创建窗口/控件时有效

		//'Button'
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		if(P2_SkinENA) BUTTON_SetSkin(hItem, SKIN_button2C);

		//'Multipage'
		hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE_0);
		MULTIPAGE_AddEmptyPage(hItem, 0, "");
		MULTIPAGE_AddEmptyPage(hItem, 0, "");
		MULTIPAGE_AddEmptyPage(hItem, 0, "");
		MULTIPAGE_AddEmptyPage(hItem, 0, "");
		MULTIPAGE_SetTabHeight(hItem, 27);		//not supported for v5.24
		MULTIPAGE_SetTabWidth(hItem, 71, 0);	//not supported for v5.24
		MULTIPAGE_SetTabWidth(hItem, 71, 1);	//not supported for v5.24
		MULTIPAGE_SetTabWidth(hItem, 71, 2);	//not supported for v5.24
		MULTIPAGE_SetTabWidth(hItem, 71, 3);	//not supported for v5.24
		if(P2_SkinENA) MULTIPAGE_SetSkin(hItem, SKIN_multipage2C);

		hWin = GUI_CreateDialogBox(_aPage0, GUI_COUNTOF(_aPage0), _cbMultipage, WM_GetClientWindow(hItem), 0, 0);
		MULTIPAGE_AttachWindow(hItem, 0, hWin);

		hWin = GUI_CreateDialogBox(_aPage1, GUI_COUNTOF(_aPage1), _cbMultipage, WM_GetClientWindow(hItem), 0, 0);
		MULTIPAGE_AttachWindow(hItem, 1, hWin);

		hWin = GUI_CreateDialogBox(_aPage2, GUI_COUNTOF(_aPage2), _cbMultipage, WM_GetClientWindow(hItem), 0, 0);
		MULTIPAGE_AttachWindow(hItem, 2, hWin);

		hWin = GUI_CreateDialogBox(_aPage3, GUI_COUNTOF(_aPage3), _cbMultipage, WM_GetClientWindow(hItem), 0, 0);
		MULTIPAGE_AttachWindow(hItem, 3, hWin);

		GUI_renew_gui_memory(pMsg->hWin, ID_TEXT_0);//更新内存使用情况,注:在内存变化的地方也需调用.
		
		//'Slider'
		hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0);
		SLIDER_SetWidth(hItem, 14);
		SLIDER_SetBarColor(hItem, GUI_BLUE);
		//if(P2_SkinENA) SLIDER_SetSkin(hItem, SKIN_slider2C);		
    
		//Edit
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);
	  //EDIT_SetBkColor(hItem, 1, (0xFF));
		EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		EDIT_SetFont(hItem, GUI_FONT_32_ASCII);
		EDIT_SetText(hItem, "0.5");
		EDIT_SetBkColor(hItem, 1, GUI_LIGHTBLUE);
		EDIT_SetTextColor(hItem, 1, GUI_GREEN);
		EDIT_EnableBlink(hItem, 300, 1);
		EDIT_GetText(hItem, editvalue, 20);
		//WM_SetFocus(hItem);

		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_2);
	  //EDIT_SetBkColor(hItem, 1, (0x00E3E4E6));
		EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		EDIT_SetFont(hItem, GUI_FONT_32_ASCII);
		EDIT_SetText(hItem, "");
		EDIT_SetBkColor(hItem, 1, GUI_LIGHTBLUE);
		EDIT_SetTextColor(hItem, 1, GUI_GREEN);
		EDIT_EnableBlink(hItem, 300, 1);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_3);
	  //EDIT_SetBkColor(hItem, 1, (0x00E3E4E6));
		EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		EDIT_SetFont(hItem, GUI_FONT_32_ASCII);
		EDIT_SetText(hItem, "");
		EDIT_SetBkColor(hItem, 1, GUI_LIGHTBLUE);
		EDIT_SetTextColor(hItem, 1, GUI_GREEN);
		EDIT_EnableBlink(hItem, 300, 1);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_4);
	  //EDIT_SetBkColor(hItem, 1, (0x00E3E4E6));
		EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		EDIT_SetFont(hItem, GUI_FONT_32_ASCII);
		EDIT_SetText(hItem, "");
		EDIT_SetBkColor(hItem, 1, GUI_LIGHTBLUE);
		EDIT_SetTextColor(hItem, 1, GUI_GREEN);
		EDIT_EnableBlink(hItem, 300, 1);

		hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_5);
	  //EDIT_SetBkColor(hItem, 1, (0x00E3E4E6));
		EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
		EDIT_SetFont(hItem, GUI_FONT_32_ASCII);
		EDIT_SetText(hItem, "");
		EDIT_SetBkColor(hItem, 1, GUI_LIGHTBLUE);
		EDIT_SetTextColor(hItem, 1, GUI_GREEN);
		EDIT_EnableBlink(hItem, 300, 1);

		break;
		
	case WM_PAINT://窗口重绘消息,这个比较难说明白,反正在Framewin 或Window 窗体之中我们一般是用控件,如果要在Framewin 或Window 窗体之中显示文字或绘制直线、矩形、圆等在这里实现
		GUI_DrawBitmap(&bmcranescfg_n, 0, 0);	//重绘背景图片
		break;
	case WM_TIMER://定时器消息,到时间时有效
		WM_RestartTimer(pMsg->Data.v, 100);
		//while(GLCD_RSTA_CBUSY());//GLCD查忙,动画时GLCD最好加上这句,否则快速切换2个页面反应变慢,去掉试试...

		break;
	case WM_NOTIFY_PARENT://操作触发消息处理(操作屏幕程序会跑到这里),比如点击按键、点击编辑框(任何的操作)等等......
		Id 	 = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) {
		case ID_MULTIPAGE_0:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				break;
			case WM_NOTIFICATION_MOVED_OUT:
				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				break;
			}
			break;
			
	case ID_EDIT_1:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
			  CMDA_buzzer_beep(60);
				WM_ShowWindow(hPage[4]);
				break;
			case WM_NOTIFICATION_RELEASED:
				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				break;
			}
			break;
			
		case ID_EDIT_2:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
			  CMDA_buzzer_beep(60);
				WM_ShowWindow(hPage[4]);
				break;
			case WM_NOTIFICATION_RELEASED:
				break;
			case WM_NOTIFICATION_VALUE_CHANGED:

				break;
			}
			break;
			
		case ID_EDIT_3:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				CMDA_buzzer_beep(60);
				WM_ShowWindow(hPage[4]);
				break;
			case WM_NOTIFICATION_RELEASED:
				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				break;
			}
			break;
			
		case ID_EDIT_4:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
			  CMDA_buzzer_beep(60);
				WM_ShowWindow(hPage[4]);				
				break;
			case WM_NOTIFICATION_RELEASED:
				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				break;
			}
			break;
			
		case ID_EDIT_5:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
			  CMDA_buzzer_beep(60);
				WM_ShowWindow(hPage[4]);				
				break;
			case WM_NOTIFICATION_RELEASED:
				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				break;
			}
			break;
	 case ID_SLIDER_0:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				break;
			case WM_NOTIFICATION_VALUE_CHANGED:
				hItem = WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0);
		    SLIDER_SetWidth(hItem, 14);
			  SLIDER_SetRange(hItem, 4, 15);
			  lightValue = SLIDER_GetValue(hItem);
			  CMDA_backlight_setting(lightValue);	
			break;
		}
			break;
		
		case ID_BUTTON_0:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				WM_HideWindow(hPage[2]);
				WM_ShowWindow(hPage[1]);
				WM_SetFocus(hPage[1]);
				CMDA_buzzer_beep(60);
				break;
			}
			break;
			case WM_NOTIFICATION_MOVED_OUT:
				BUTTON_SetPressed(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0), buttonS[0] = !buttonS[0]);
				break;
			}
			break;

	default:
		WM_DefaultProc(pMsg);
		break;
	}
}

WM_HWIN CreateWindowDLG2(void);
WM_HWIN CreateWindowDLG2(void)
{
	WM_HWIN hWin;
	static WIN_BITMAP BMP_WindowDLG2; WIN_BITMAP *pBMP_WindowDLG2 = &BMP_WindowDLG2;

	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	WM_CreateTimer(hWin, 1, 100, 0); //创建一个软件定时器

	if(!P2_SkinENA) return hWin;
	//将不同状态的页面BMP位图赋值给"页面图片结构体"(所有成员都要赋值)
	BMP_WindowDLG2.normal = &bmcranescfg_n;
	BMP_WindowDLG2.mark = &bmcranescfg_m;
	BMP_WindowDLG2.focus = &bmcranescfg_n;
	BMP_WindowDLG2.disable = &bmcranescfg_n;
	BMP_WindowDLG2.thumbN = &bmcranescfg_n;
	BMP_WindowDLG2.thumbM = &bmcranescfg_n;
	WM_SetUserData(hWin, &pBMP_WindowDLG2, sizeof(pBMP_WindowDLG2));//将指针写到用户数据区

	return hWin;
}




#include "DIALOG.h"

extern WM_HWIN hPage[4];

#define ID_WINDOW_0  (GUI_ID_USER + 0x00)
#define ID_CHECKBOX_0  (GUI_ID_USER + 0x01)
#define ID_GRAPH_0  (GUI_ID_USER + 0x02)
#define ID_BUTTON_0  (GUI_ID_USER + 0x03)
#define ID_BUTTON_1  (GUI_ID_USER + 0x04)
#define ID_BUTTON_2  (GUI_ID_USER + 0x05)
#define ID_BUTTON_3  (GUI_ID_USER + 0x06)


static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
	{ WINDOW_CreateIndirect, "Framewin", ID_WINDOW_0, 360, 120, 420, 270, 0, 0x64, 0 },
	{ GRAPH_CreateIndirect, "Graph", ID_GRAPH_0, 15, 25, 440, 170, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 15, 205, 90, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 130, 205, 90, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_2, 250, 205, 90, 30, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_3, 365, 205, 90, 30, 0, 0x0, 0 },
	{ CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_0, 195, 5, 100, 16, 0, 0x0, 0 },
};

#define MAX_VALUE	125
static GRAPH_DATA_Handle  pdataGRP;

static void _cbDialog(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;
	int     NCode;
	int     Id;

	short Val;
	GRAPH_SCALE_Handle hScaleV;
	GRAPH_SCALE_Handle hScaleH;

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG://初始化消息,创建窗口/控件时有效,比如在这里设置一些控件的初始参数
		//hItem = pMsg->hWin;
		//FRAMEWIN_SetText(hItem, "www.neqee.com");
		//FRAMEWIN_SetTextColor(hItem, (0x00FF0000));
		//FRAMEWIN_SetTitleHeight(hItem, 20);
		//FRAMEWIN_SetFont(hItem, GUI_FONT_20_1);

		hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
		CHECKBOX_SetText(hItem, "TIMER OFF");
		CHECKBOX_SetFont(hItem, GUI_FONT_13_1);
		CHECKBOX_SetTextColor(hItem, (0x008000FF));

		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
		BUTTON_SetFont(hItem, GUI_FONT_16_1);
		BUTTON_SetText(hItem, "Button0");

		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
		BUTTON_SetFont(hItem, GUI_FONT_16_1);
		BUTTON_SetText(hItem, "Button1");

		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
		BUTTON_SetFont(hItem, GUI_FONT_16_1);
		BUTTON_SetText(hItem, "Button2");

		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
		BUTTON_SetFont(hItem, GUI_FONT_16_1);
		BUTTON_SetText(hItem, "Button3");

		hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
		GRAPH_SetBorder(hItem, 40, 5, 5, 30);
		GRAPH_SetGridVis(hItem, 1);
		GRAPH_SetGridFixedX(hItem, 1);
		GRAPH_SetGridDistY(hItem, 25);
		GRAPH_SetGridDistX(hItem, 50);
		hScaleV = GRAPH_SCALE_Create(30, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, 25);//创建和增加垂直范围尺度标签
		GRAPH_SCALE_SetTextColor(hScaleV, GUI_RED);									//设置标签字体颜色
		GRAPH_AttachScale(hItem, hScaleV);											//将标签添加到垂直方向
		hScaleH = GRAPH_SCALE_Create(150, GUI_TA_HCENTER, GRAPH_SCALE_CF_HORIZONTAL, 50);	//创建和增加水平范围尺度标签
		GRAPH_SCALE_SetTextColor(hScaleH, GUI_DARKGREEN);							//设置字体颜色
		GRAPH_AttachScale(hItem, hScaleH);											//添加到水平方向
		pdataGRP = GRAPH_DATA_YT_Create(GUI_GREEN, 500/*最大数据个数*/, 0, 0);		//创建一个数据曲线,可创建多个曲线
		GRAPH_AttachData(hItem, pdataGRP);											//为绘图控件添加数据对象
		break;
	case WM_KEY://键盘按键消息(操作实体键盘程序跑到这里)
		Val = (short)(((WM_KEY_INFO*)(pMsg->Data.p))->Key);	//获取键值
		GRAPH_DATA_YT_AddValue(pdataGRP, (Val>MAX_VALUE) ? MAX_VALUE : Val);//赋值到曲线
		break;
	case WM_TIMER://定时器消息(定时到时程序跑到这里)
		WM_RestartTimer(pMsg->Data.v, 300);
		if(WM_IsCompletelyCovered(pMsg->hWin)) break;		//当切换到其他页面什么都不做

		//可以在这里获取ADC值
		if(!CHECKBOX_IsChecked(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0)))
			GRAPH_DATA_YT_AddValue(pdataGRP, (I16)50);		//赋值到曲线
		break;
	case WM_NOTIFY_PARENT://操作触发消息处理(操作屏幕程序会跑到这里),比如点击按键、点击编辑框(任何的操作)等等......
		Id    = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) {
		case ID_CHECKBOX_0:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
			case WM_NOTIFICATION_RELEASED:
			case WM_NOTIFICATION_VALUE_CHANGED:
				WM_SetFocus(WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0));	//聚焦到ID_BUTTON_0控件
				break;
			}
			break;
		case ID_BUTTON_0:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED: break;
			case WM_NOTIFICATION_RELEASED:
				GRAPH_DATA_YT_AddValue(pdataGRP, (I16)0);	//赋值到曲线
				break;
			}
			break;
		case ID_BUTTON_1:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED: break;
			case WM_NOTIFICATION_RELEASED:
				GRAPH_DATA_YT_AddValue(pdataGRP, (I16)25);	//赋值到曲线
				break;
			}
			break;
		case ID_BUTTON_2:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED: break;
			case WM_NOTIFICATION_RELEASED:
				GRAPH_DATA_YT_AddValue(pdataGRP, (I16)75);	//赋值到曲线
				break;
			}
			break;
		case ID_BUTTON_3:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED: break;
			case WM_NOTIFICATION_RELEASED:
				GRAPH_DATA_YT_AddValue(pdataGRP, (I16)100);	//赋值到曲线
				WM_HideWindow(hPage[3]);
				WM_ShowWindow(hPage[1]);
				WM_SetFocus(hPage[1]);
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

WM_HWIN CreateWindGraphDLG(void);
WM_HWIN CreateWindGraphDLG(void) {
  WM_HWIN hWin;
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  WM_CreateTimer(WM_GetClientWindow(hWin), 1, 300, 0); //创建一个软件定时器
  return hWin;
}

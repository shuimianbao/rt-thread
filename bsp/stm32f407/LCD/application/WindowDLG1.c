#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "stm32f4xx.h"
#include "DIALOG.h"
#include "bitmap.h"
#include "skin\skin.h"
#include "time.h"
#include "cmd_a.h"

#define P1_SkinENA	(1)
extern WM_HWIN hPage[5];
char x[] = "This is a stand-by screen";

#define ID_WINDOW_0		(GUI_ID_USER + 0x00)
#define ID_TEXT_0		  (GUI_ID_USER + 0x01)
#define ID_TEXT_01	  (GUI_ID_USER + 0x02)
#define ID_TEXT_02	  (GUI_ID_USER + 0x03)
#define ID_TEXT_03	  (GUI_ID_USER + 0x04)
#define ID_IMAGE_0    (GUI_ID_USER + 0x05)
#define ID_IMAGE_1    (GUI_ID_USER + 0x06)
#define ID_IMAGE_2    (GUI_ID_USER + 0x07)
#define ID_TEXT_1		  (GUI_ID_USER + 0x08)
#define ID_TEXT_2		  (GUI_ID_USER + 0x09)
#define ID_TEXT_3		  (GUI_ID_USER + 0x10)
#define ID_TEXT_4		  (GUI_ID_USER + 0x11)
#define ID_TEXT_5		  (GUI_ID_USER + 0x12)
#define ID_IMAGE_01   (GUI_ID_USER + 0x13)
#define ID_IMAGE_02   (GUI_ID_USER + 0x14)
#define ID_IMAGE_03   (GUI_ID_USER + 0x15)
#define ID_IMAGE_04   (GUI_ID_USER + 0x16)
#define ID_IMAGE_05   (GUI_ID_USER + 0x17)
#define ID_BUTTON_0		(GUI_ID_USER + 0x18)
#define ID_BUTTON_1		(GUI_ID_USER + 0x19)
#define ID_BUTTON_2		(GUI_ID_USER + 0x20)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
	{ WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 800, 600, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "Text", ID_TEXT_0, 0, 0, 320, 12, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "DATE", ID_TEXT_01, 330, 15, 320, 40, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "TIME", ID_TEXT_02, 610, 15, 80, 40, 0, 0x0, 0 },	
	{ TEXT_CreateIndirect, "ID", ID_TEXT_03, 508, 92, 250, 40, 0, 0x0, 0 },
	{ IMAGE_CreateIndirect, "", ID_IMAGE_0, 700, 5, 40, 40, 0, 0x0, 0 },
	{ IMAGE_CreateIndirect, "", ID_IMAGE_1, 750, 5, 40, 40, 0, 0x0, 0 },	
	{ IMAGE_CreateIndirect, "", ID_IMAGE_2, 680, 490, 120, 40, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_1, 200, 105, 80, 40, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_2, 200, 150, 80, 40, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_3, 200, 195, 80, 40, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_4, 200, 240, 80, 40, 0, 0x0, 0 },
	{ TEXT_CreateIndirect, "", ID_TEXT_5, 200, 285, 80, 40, 0, 0x0, 0 },
	{ IMAGE_CreateIndirect, "", ID_IMAGE_01, 320, 105, 40, 40, 0, 0x0, 0 },
	{ IMAGE_CreateIndirect, "", ID_IMAGE_02, 320, 150, 40, 40, 0, 0x0, 0 },
	{ IMAGE_CreateIndirect, "", ID_IMAGE_03, 320, 195, 40, 40, 0, 0x0, 0 },
	{ IMAGE_CreateIndirect, "", ID_IMAGE_04, 320, 240, 40, 40, 0, 0x0, 0 },
	{ IMAGE_CreateIndirect, "", ID_IMAGE_05, 320, 285, 40, 40, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_0, 235, 520, 70, 70, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "", ID_BUTTON_1, 378, 520, 70, 70, 0, 0x0, 0 },	
	{ BUTTON_CreateIndirect, "", ID_BUTTON_2, 516, 520, 70, 70, 0, 0x0, 0 },
};

static void GUI_renew_gui_memory(WM_HWIN hParent, int textID)
{
	char buff[48];
	sprintf(buff, "Memory used: %dKBytes  free: %dKBytes", GUI_ALLOC_GetNumUsedBytes()/1024, GUI_ALLOC_GetNumFreeBytes()/1024);
	//TEXT_SetTextColor(WM_GetDialogItem(hParent, textID), (0x008000FF));
	TEXT_SetText(WM_GetDialogItem(hParent, textID), buff);
}

static const GUI_BITMAP *movieTAB0[9] =
{
	&bmmov0_00,&bmmov0_01,&bmmov0_02,&bmmov0_03,&bmmov0_04,&bmmov0_05,&bmmov0_06,&bmmov0_07,&bmmov0_08,
};
extern GUI_CONST_STORAGE GUI_FONT GBK_16m16;	//字库声明
extern GUI_CONST_STORAGE GUI_FONT GBK_32m32;
extern GUI_CONST_STORAGE GUI_FONT GBK_64m64;
extern GUI_CONST_STORAGE GUI_FONT UNI_16m16;
#include "glcd.h"


static void _cbDialog(WM_MESSAGE * pMsg) {		//窗体回调函数
	WM_HWIN hItem;
	int NCode;
	int Id;
	static int movCNT = 0, prgCNT[2] = {0, 0};
	static unsigned char buttonS = 1;
	
  //RTC_TimeTypeDef   RTC_TimeStructure;
  //RTC_DateTypeDef   RTC_DateStructure;
	
	double txt1 = rand()%100*0.1f;
	int txt2 = rand()%1000;
	int txt3 = rand()%100;
	int txt4 = rand()%100;
	int txt5 = rand()%1000;
	char str[20];
	
	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG://初始化消息,创建窗口/控件时有效,比如在这里设置一些控件的初始参数
		WM_RestartTimer(pMsg->Data.v, 500);
	
		//'Text'
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_01); 
	  TEXT_SetFont(hItem, &GBK_32m32);;
		TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
		TEXT_SetTextColor(hItem, GUI_RED);
		TEXT_SetText(hItem, "2019年 1月15日");

			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_02); 
	  TEXT_SetFont(hItem, &GBK_32m32);;
		TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
		TEXT_SetTextColor(hItem, GUI_RED);
	  TEXT_SetText(hItem, "12:30");
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_03); 
	  TEXT_SetFont(hItem, GUI_FONT_24_ASCII);
		TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
		TEXT_SetTextColor(hItem, GUI_MAGENTA);
		TEXT_SetText(hItem, "G205258218");
	
	  //'Image'
		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_0);
	  IMAGE_SetBitmap(hItem, &bmgps1);
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_1);
	  IMAGE_SetBitmap(hItem, &bmsignal5);	
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_01);
	  IMAGE_SetBitmap(hItem, &bmsafelight);	
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_02);
	  IMAGE_SetBitmap(hItem, &bmsafelight);	
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_03);
	  IMAGE_SetBitmap(hItem, &bmsafelight);	
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_04);
	  IMAGE_SetBitmap(hItem, &bmsafelight);	
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_05);
	  IMAGE_SetBitmap(hItem, &bmsafelight);	
		
	  //'Text'
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1); 
	  TEXT_SetFont(hItem, GUI_FONT_32_ASCII);
		TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
		TEXT_SetTextColor(hItem, GUI_GREEN);
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2); 
	  TEXT_SetFont(hItem, GUI_FONT_32_ASCII);
		TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
		TEXT_SetTextColor(hItem, GUI_GREEN);
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3); 
	  TEXT_SetFont(hItem, GUI_FONT_32_ASCII);
		TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
		TEXT_SetTextColor(hItem, GUI_GREEN);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4); 
	  TEXT_SetFont(hItem, GUI_FONT_32_ASCII);
		TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
		TEXT_SetTextColor(hItem, GUI_GREEN);
		
		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5); 
	  TEXT_SetFont(hItem, GUI_FONT_32_ASCII);
		TEXT_SetTextAlign(hItem, TEXT_CF_HCENTER | TEXT_CF_VCENTER);
		TEXT_SetTextColor(hItem, GUI_GREEN);
	
		//'Button'	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);	//获取ID_BUTTON_0的句柄
		//BUTTON_SetTextColor(hItem, 0, GUI_WHITE);			//修改ID_BUTTON_0为白色文字
		BUTTON_SetFont(hItem, &GBK_16m16);					//修改ID_BUTTON_0的字库
		BUTTON_SetText(hItem, "");
		if(P1_SkinENA) BUTTON_SetSkin(hItem, SKIN_button2C);
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);	//获取ID_BUTTON_0的句柄
		BUTTON_SetFont(hItem, &GBK_16m16);
		BUTTON_SetText(hItem, "");
		if(P1_SkinENA) BUTTON_SetSkin(hItem, SKIN_button2C);
	
		hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
		BUTTON_SetFont(hItem, &GBK_16m16);
		BUTTON_SetText(hItem, "");
		if(P1_SkinENA) BUTTON_SetSkin(hItem, SKIN_button2C);
		
		GUI_renew_gui_memory(pMsg->hWin, ID_TEXT_0);//更新内存使用情况,注:在内存变化的地方也需调用.
		break;
	case WM_PAINT://窗口重绘消息,这个比较难说明白,反正在Framewin 或Window 窗体之中我们一般是用控件,如果要在Framewin 或Window 窗体之中显示文字或绘制直线、矩形、圆等在这里实现
		GUI_DrawBitmap(&bmcranes_n, 0, 0);	//重绘背景图片
	  GUI_SetFont(&GBK_32m32);
		//GUI_SetBkColor(GUI_TRANSPARENT);
		//GUI_SetColor(GUI_RED);
	  //GUI_DispStringAt("时间", 400, 20);
		break;
	case WM_TIMER://定时器消息,到时间时有效
		WM_RestartTimer(pMsg->Data.v, 500);

		if(WM_IsCompletelyCovered(WM_GetDialogItem(pMsg->hWin, ID_IMAGE_2))) break;//do nothing if chancing to other page
		while(GLCD_RSTA_CBUSY());//GLCD查忙,动画时GLCD最好加上这句,否则快速切换2个页面反应变慢,去掉试试...

		if(!movieTAB0[movCNT]) break;
		IMAGE_SetBitmap(WM_GetDialogItem(pMsg->hWin, ID_IMAGE_2), movieTAB0[movCNT]);
		movCNT = (movCNT<8) ? (movCNT+1) : 0;
	
			//RTC_GetTime(RTC_Format_BIN, &rtc_time);
			//txt1 = RTC_GetTime();
			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);	
			sprintf(str, "%1.1f", txt1);
			TEXT_SetText(hItem, str);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);	
			sprintf(str, "%3d", txt2);
			TEXT_SetText(hItem, str);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);	
			sprintf(str, "%2d", txt3);
			TEXT_SetText(hItem, str);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);	
			sprintf(str, "%2d", txt4);
			TEXT_SetText(hItem, str);

			hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);	
			sprintf(str, "%3d", txt5);
			TEXT_SetText(hItem, str);	

			if (txt1 > 40)
			{
				hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_01);
				IMAGE_SetBitmap(hItem, &bmdangerlight);	
				//CMDA_buzzer_beep(120);
			}
			else
			{
				hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_01);
	      IMAGE_SetBitmap(hItem, &bmsafelight);	
			}			
			
			if (txt5 > 500)
			{
				hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_05);
				IMAGE_SetBitmap(hItem, &bmdangerlight);	
						if (WM_IsCompletelyCovered(pMsg->hWin)) 
						{
							//DO NOTHING;
						}
						else
						{
						//CMDA_buzzer_beep(120);
						}
			}
			else
			{
				hItem = WM_GetDialogItem(pMsg->hWin, ID_IMAGE_05);
	      IMAGE_SetBitmap(hItem, &bmsafelight);	
			}
			
	break;
	case WM_NOTIFY_PARENT://操作触发消息处理(操作屏幕程序会跑到这里),比如点击按键、点击编辑框(任何的操作)等等......
		Id		= WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) {
			case ID_BUTTON_0:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				//WM_RestartTimer(pMsg->Data.v, 500);
				WM_HideWindow(hPage[1]);
				WM_ShowWindow(hPage[0]);
				WM_SetFocus(hPage[0]);
				CMDA_buzzer_beep(60);

				break;
			}
			break;
		case ID_BUTTON_1:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				CMDA_buzzer_beep(60);

				break;
			}
			break;
		case ID_BUTTON_2:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED:
				break;
			case WM_NOTIFICATION_RELEASED:
				WM_HideWindow(hPage[1]);
				WM_ShowWindow(hPage[2]);
				WM_SetFocus(hPage[2]);
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


WM_HWIN CreateWindowDLG1(void);
WM_HWIN CreateWindowDLG1(void) {
	WM_HWIN hWin;
	static WIN_BITMAP BMP_WindowDLG1; WIN_BITMAP *pBMP_WindowDLG1 = &BMP_WindowDLG1;

	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	WM_CreateTimer(hWin, 1, 100, 0);//创建一个软件定时器

	if(!P1_SkinENA) return hWin;
	//将不同状态的页面BMP位图赋值给"页面图片结构体"(所有成员都要赋值)
	BMP_WindowDLG1.normal = &bmcranes_n;
	BMP_WindowDLG1.mark = &bmcranes_m;
	BMP_WindowDLG1.focus = &bmcranes_n;
	BMP_WindowDLG1.disable = &bmcranes_n;
	BMP_WindowDLG1.thumbN = &bmWindowDLG1_thumbN;
	BMP_WindowDLG1.thumbM = &bmWindowDLG1_thumbM;
	WM_SetUserData(hWin, &pBMP_WindowDLG1, sizeof(pBMP_WindowDLG1));//将指针写到用户数据区
	return hWin;
}


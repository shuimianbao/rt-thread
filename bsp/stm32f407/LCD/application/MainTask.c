#include "dialog.h"

WM_HWIN hPage[5] = {0, 0, 0, 0, 0};

void MainTask(void)
{
	GUI_Init();
	GUI_UC_SetEncodeGBK();

	if(!hPage[0]) hPage[0] = CreateWelcomeDLG(WM_HBKWIN);	
	if(!hPage[1]) hPage[1] = CreateWindowDLG1(WM_HBKWIN);	//����ǰ�жϾ���Ƿ�Ϊ0,��ɾ������ǵþ����0
	if(!hPage[2]) hPage[2] = CreateWindowDLG2(WM_HBKWIN);
	if(!hPage[3]) hPage[3] = CreateStandbyDLG(WM_HBKWIN);	
	if(!hPage[4]) hPage[4] = CreateWindowSKB(WM_HBKWIN);
	WM_HideWindow(hPage[3]);
	//if(!hPage[2]) hPage[2] = CreateWindowSKB(hPage[1]);		//Soft keyboard
	
	WM_HideWindow(hPage[1]);
	WM_HideWindow(hPage[2]);
	WM_HideWindow(hPage[3]);
	WM_HideWindow(hPage[4]);
	WM_SetFocus(hPage[0]);									//emWin��Ѱ�Ҳ��۽���hPage[0]����ĵ�1���ܾ۽���С�ؼ�

	if(GUI_ALLOC_GetNumFreeBytes() < 3200) GUI_ErrorOut("Not enough memory available.");

	while(1) {GUI_Delay(20);}
}

#include "GUI.h"

static GUI_CONST_STORAGE unsigned char Xsize002[] = {//�ֿ��2�ֶ��ַ��Ŀ�(����)
  64
};

static GUI_CONST_STORAGE unsigned char Xsize001[] = {//�ֿ��1�ֶ��ַ��Ŀ�(����)
  32
};

static GUI_CONST_STORAGE GUI_FONT_PROP_GL Prop002 = {//�ֿ��2�ֶ�
   0x0080//��ʼ�ַ�
  ,0xfefe//�����ַ�
  ,&Xsize002[0]
  ,0
};

static GUI_CONST_STORAGE GUI_FONT_PROP_GL Prop001 = {//�ֿ��1�ֶ�
   0x0020//��ʼ�ַ�
  ,0x007f//�����ַ�
  ,&Xsize001[0]
  ,&Prop002//ָ����1�ֶ�
}; 

GUI_CONST_STORAGE GUI_FONT_GL GBK_64m64 = {
   GUI_FONTTYPE_PROP_GL
  ,64        //�ַ��ĸ�(�����ַ�ֻ��1�ָ߶�)
  ,64        //�ַ���ʾʱY����ļ�࣬�����ַ���ʱ���о�Ϊ0
  ,1         //X����Ŵ�������֧���ַ��Ŵ���Զ��1
  ,1         //Y����Ŵ�������֧���ַ��Ŵ���Զ��1
  ,&Prop001  //ָ���ֿ��1�ֶ�
  ,0         //δ֪
  ,0         //δ֪
  ,0         //δ֪
  ,2         //�ֿ����� 1:gb2312 2:gbk 4:big5 8:unicode(monospaced) 16:unicode(non-monospaced)
  ,"a64:k64" //Nand-Flash�е��ֿ��ļ���(����ascii�ͷ�ascii�����ֿ�,����":"����)
};









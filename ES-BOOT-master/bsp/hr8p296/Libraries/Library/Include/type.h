/*********************************************************
 *Copyright (C), 2015, Shanghai Eastsoft Microelectronics Co., Ltd.
 *�ļ���:  type.h
 *��  ��:  ESM Application Team 
 *��  ��:  V1.01
 *��  ��:  2016/11/01
 *��  ��:  type define
 *��  ע:  ������HRSDK-GDB-8P296
 ���������ѧϰ����ʾʹ�ã����û�ֱ�����ô����������ķ��ջ������е��κη������Ρ�
 **********************************************************/
#ifndef __TYPE_H__
#define __TYPE_H__

typedef enum {DISABLE = 0, ENABLE = !DISABLE} TYPE_FUNCEN,FuncState,TYPE_PINTIE,TYPE_PINTMASK;
typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus, PinStatus;
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;


#endif

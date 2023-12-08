#include <windsp.h>
#include <string.h>
#include <bios.h>

extern   uchar buf_tx_bom[_C_SIZE_TX_BOM];


int idata current_win = 0;

void (*TxCallBack)(char* pcBuffer, int count);  //pointer to external UART transmiting function
void (*OnNotify) (int IDW, int ID, int param);  //window control event handler
bit (*TxReady)(void );



void CopyStr(char *d,char *s)
 {while (*d++=*s++);
 }

//-----------------------------DSPS additional functions---------------------------------------------
void SetCurrentWin(int IDW)
{current_win = IDW;
}
//***************************************************************************************************
void RxHandler(char* pcBuffer, int count)
{

}
//
//
void DSPS_Init(void (*TTxCallBack)(char* pcBuffer, int count),bit (*TTxReady)(void ))
{TxCallBack =TTxCallBack;
 TxReady=TTxReady;
}

//----------------------------------SYSTEM functions-------------------------------------------------
void sys_Sound(int index)
{while (!TxReady());
 buf_tx_bom[0]=COM_SYSTEM;
 buf_tx_bom[1]=SYS_SOUND;
 buf_tx_bom[2]=index;
 TxCallBack(buf_tx_bom,3);

}
//***************************************************************************************************
void sys_Reset()
{while (TxReady==0);
 buf_tx_bom[0]=COM_SYSTEM;
 buf_tx_bom[1]=SYS_RESET;
 TxCallBack(buf_tx_bom,2);
}

//***************************************************************************************************
void sys_Message(char *pcMessage)
{int idata temp;
 while (!TxReady());
 temp=strlen(pcMessage);
 buf_tx_bom[0]=COM_SYSTEM;
 buf_tx_bom[1]=SYS_MESSAGE;
 buf_tx_bom[2]=temp;
 CopyStr(buf_tx_bom+3,pcMessage);
 TxCallBack(buf_tx_bom,temp+3);
}



//***************************************************************************************************
void sys_NewTexWin(int IDW, CRect* Rect)
{while (!TxReady());
 buf_tx_bom[0]=COM_SYSTEM;
 buf_tx_bom[1]=SYS_NEW_TEXT_WIN;
 buf_tx_bom[2]=IDW; current_win = IDW;
 buf_tx_bom[3]=Rect->top >>8;buf_tx_bom[4]=Rect->top & 0xFF;
  buf_tx_bom[5]=Rect->left >>8;buf_tx_bom[6]=Rect->left & 0xFF;
   buf_tx_bom[7]=Rect->width >>8; buf_tx_bom[8]=Rect->width & 0xFF;
    buf_tx_bom[9]=Rect->height>>8; buf_tx_bom[10]=Rect->height & 0xFF;
 TxCallBack(buf_tx_bom,11);
}

//***************************************************************************************************
void sys_DeleteWin(int IDW)
{while (!TxReady());
 buf_tx_bom[0]=COM_SYSTEM;
 buf_tx_bom[1]=SYS_DELETE_WIN;
 buf_tx_bom[2]=IDW;
 TxCallBack(buf_tx_bom,3);
}

//------------------------------------TEXT window functions------------------------------------------
void text_SetX(int pos)
{while (!TxReady());
 buf_tx_bom[0]=COM_TEXT;
 buf_tx_bom[1]=current_win;
 buf_tx_bom[2]=TEXT_SETX;
 buf_tx_bom[3]=pos;
 TxCallBack(buf_tx_bom,4);
}

//***************************************************************************************************
void text_SetY(int pos)
{while (!TxReady());
 buf_tx_bom[0]=COM_TEXT;
 buf_tx_bom[1]=current_win;
 buf_tx_bom[2]=TEXT_SETY;
 buf_tx_bom[3]=pos;
 TxCallBack(buf_tx_bom,4);
}

//***************************************************************************************************
void text_Clear()
{while (!TxReady());
 buf_tx_bom[0]=COM_TEXT;
 buf_tx_bom[1]=current_win;
 buf_tx_bom[2]=TEXT_CLEAR;
 TxCallBack(buf_tx_bom,3);
}

//***************************************************************************************************
void text_FontSize(int size)
{while (!TxReady());
 buf_tx_bom[0]=COM_TEXT;
 buf_tx_bom[1]=current_win;
 buf_tx_bom[2]=TEXT_FONT_SIZE;
 buf_tx_bom[3]=size;
 TxCallBack(buf_tx_bom,4);
}


//***************************************************************************************************
void text_FontColor(int color)
{while (!TxReady());
 buf_tx_bom[0]=COM_TEXT;
 buf_tx_bom[1]=current_win;
 buf_tx_bom[2]=TEXT_FONT_COLOR;
 buf_tx_bom[3]=color;
 TxCallBack(buf_tx_bom,4);
}


//***************************************************************************************************
void text_ClearLine(int line)
{while (!TxReady());
 buf_tx_bom[0]=COM_TEXT;
 buf_tx_bom[1]=current_win;
 buf_tx_bom[2]=TEXT_CLEAR_LINE;
 buf_tx_bom[3]=line;
 TxCallBack(buf_tx_bom,4);
}

//***************************************************************************************************
void text_AddText(char* Text)
{int temp;
 while (!TxReady());
 buf_tx_bom[0]=COM_TEXT;
 buf_tx_bom[1]=current_win;
 buf_tx_bom[2]=TEXT_ADD_TEXT;
 temp=strlen(Text);
 buf_tx_bom[3]=temp;
 CopyStr(buf_tx_bom+4,Text);
 TxCallBack(buf_tx_bom,temp+4);
}



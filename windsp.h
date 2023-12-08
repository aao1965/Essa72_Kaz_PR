
#ifndef __WINDSP
#define __WINDSP

//Command types
#define COM_SYSTEM                      0X01
#define COM_TEXT                        0X02
#define COM_CONTROLS                    0X03
#define COM_CHART                       0X04

//System commands
#define SYS_SOUND                       1
#define SYS_RESET                       2
#define SYS_MESSAGE                     3
#define SYS_NEW_CTRL_WIN                4
#define SYS_NEW_CHART_WIN               5
#define SYS_NEW_TEXT_WIN                6
#define SYS_DELETE_WIN                  7

//Text_Screen commands
#define TEXT_SETX                       1
#define TEXT_SETY                       2
#define TEXT_CLEAR                      3
#define TEXT_FONT_SIZE                  4
#define TEXT_FONT_COLOR                 5
#define TEXT_NEW_LINE                   6
#define TEXT_SCROLL                     7
#define TEXT_CLEAR_LINE                 8
#define TEXT_SET_LINE_WIDTH             9
#define TEXT_SET_WORD_WRAP              10
#define TEXT_ADD_TEXT                   11

//Controls_Screen commands
#define CTRL_NEW_BUTTON                 1
#define CTRL_NEW_SLIDER                 2
#define CTRL_NEW_PROGRESS               3
#define CTRL_NEW_CHECKBOX               4
#define CTRL_NEW_LABEL                  5
#define CTRL_SET_PARAM                  6
#define CTRL_CLEAR                      7
#define CTRL_DELETE_CTRL                8

//Chart_Screen commands
#define CHART_CLEAR                     1
#define CHART_NEW_CHART                 2
#define CHART_ADD_SAMPLES               3
#define CHART_DELETE_CHART              4

typedef struct
{
        int top;
        int left;
        int width;
        int height;
} CRect;

//DSPS additional functions
void RxHandler(char* pcBuffer, int count);              //user software have to call this function to process received data
void SetCurrentWin(int IDW);                            //set current PC window to process commands


//init DSPS library
void DSPS_Init(void (*TTxCallBack)(char* pcBuffer, int count),bit (*TTxReady)(void));

//SYSTEM functions
void sys_Sound(int index);
void sys_Reset();
void sys_Message(char* pcMessage);
void sys_NewCtrlWin(int IDW, CRect* Rect);
void sys_NewChartWin(int IDW, CRect* Rect);
void sys_NewTexWin(int IDW, CRect* Rect);
void sys_DeleteWin(int IDW);

//TEXT window functions
void text_SetX(int pos);
void text_SetY(int pos);
void text_Clear();
void text_FontSize(int size);
void text_FontFormat(int format);
void text_FontColor(int color);
void text_NewLine();
void text_Scroll(int value);
void text_ClearLine(int line);
void text_SetLineWidth(int width);
void text_SetWordWrap(bit value);
void text_AddText(char* Text);

//CONTROLS window functions
void ctrl_NewButton(int ID, char* pcCaption, CRect* Rect);
void ctrl_NewSlider(int ID, CRect* Rect, int min, int max);
void ctrl_NewProgress(int ID, CRect* Rect,int min, int max);
void ctrl_NewCheckBox(int ID, char* pcCaption, int top, int left);
void ctrl_NewLabel(int ID, char* pcCaption, int top, int left);
void ctrl_SetParam(int ID, int param);
void ctrl_Clear();
void ctrl_DeleteCtrl(int ID);

//CHART window functions
void chart_Clear();
void chart_NewChart(int ID, char* pcCaption, int top, int size, int color, int style);
void chart_AddSamples(int ID, int* piBuffer, int count);
void chart_DeleteChart(int ID);

#endif


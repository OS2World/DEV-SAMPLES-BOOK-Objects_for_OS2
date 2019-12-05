/************/
/* CLIENT.C */
/************/

#define Client_Class_Source
#include <client.ih>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <graphic.h>

FNWP vwClientProc;

/* I inherit somInit from SOMObject */

SOM_Scope void  SOMLINK somInit(Client somSelf)
{
    ClientData *somThis = ClientGetData(somSelf);
    Environment *ev = somGetGlobalEnvironment();
    short  i;
    static ULONG flFrameFlags = FCF_TITLEBAR    |  FCF_SYSMENU  |
                                FCF_SIZEBORDER  |  FCF_MINMAX   |
                                FCF_SHELLPOSITION | FCF_TASKLIST;
    HWND hwndTitleBar, hwndFrame, hwndClient;
    HAB hab;
    static BOOL firsttimein=TRUE;
    string className="Client App";

    Client_parent_Window_somInit(somSelf);

    hab = __get_hab(somSelf, ev);
    _gr = GraphicNew();

    if(firsttimein) {
        WinRegisterClass(
            hab,    /* anchor block handle */
            className,  /* Window Client Class name */
            vwClientProc,  /* EMClientProc  Client procedure for class */
            CS_SIZEREDRAW, /*   Class style */
            4L);       /*   Extra bytes to reserve for data like HPS */
        firsttimein = FALSE;
    }
    hwndFrame = WinCreateStdWindow(
            HWND_DESKTOP,
            WS_VISIBLE,
            &flFrameFlags,
            className,
            NULL,
            0L,
            (HMODULE)0,
            0L,    /* resource ID */
            &hwndClient);
    if((hwndTitleBar = WinWindowFromID (hwndFrame, FID_TITLEBAR)) != 0L)
        WinSetWindowText (hwndTitleBar, className);

    WinSetWindowPtr(hwndClient, 0L, somSelf);

    __set_grWin(_gr, ev, somSelf);  /* the graphic object needs to know its associated window */

    __set_hwndWindow(somSelf, ev, hwndClient);
    __set_hwndFrame(somSelf, ev, hwndFrame);
}

SOM_Scope void  SOMLINK somUninit(Client somSelf)
{
    ClientData *somThis = ClientGetData(somSelf);
    Environment *ev = somGetGlobalEnvironment();
    Client_parent_Window_somUninit(somSelf);
}

SOM_Scope void  SOMLINK CharPress(Client somSelf, Environment *ev,
                char key,
                short kbmState)
{ }

SOM_Scope void  SOMLINK VirtCharPress(Client somSelf, Environment *ev,
                short vkey, short kbmState)
{ }

/*
 *  x,y when pressed
 */

SOM_Scope void  SOMLINK Button1Press(Client somSelf, Environment *ev,
                short x, short y)
{ }

/*
 *  x,y when pressed
 */

SOM_Scope void  SOMLINK Button1Click(Client somSelf, Environment *ev,
                short x, short y)
{ }

/*
 *  x,y when pressed
 */

SOM_Scope void  SOMLINK Button1DblClick(Client somSelf, Environment *ev,
                short x, short y)
{ }

SOM_Scope void  SOMLINK Button1DragBegin(Client somSelf,  Environment *ev,
                                         short x, short y)
{ }


SOM_Scope void  SOMLINK Button1Drag(Client somSelf, Environment *ev,
                short x, short y)
{ }

SOM_Scope void  SOMLINK Button1DragEnd(Client somSelf,  Environment *ev,
                                       short x, short y)
{ }

SOM_Scope void  SOMLINK PaintWindow(Client somSelf, Environment *ev,
                RECTL* rcl)
{ }

SOM_Scope void  SOMLINK ReSize(Client somSelf, Environment *ev,
                short w, short h)
{ }


MRESULT EXPENTRY vwClientProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    HPS hps;
    RECTL rcl;
    char key;
    short vkey, sFlag, kbmState;
    Environment *ev = somGetGlobalEnvironment();
    SOMObject somSelf;
    short x = MOUSEMSG(&msg)->x;
    short y = MOUSEMSG(&msg)->y;
    short w, h;
    SHORT src;
    Window somWin;
    SHORT winID;
    HWND winHwnd;
    static BOOL button1motion=FALSE;

    if(hwnd)
        somSelf = WinQueryWindowPtr(hwnd, 0);

    switch(msg) {
        case WM_CHAR:
            sFlag = CHARMSG(&msg)->fs;
            if(sFlag & KC_KEYUP)
                return FALSE;
            if(sFlag & KC_CHAR) {
                key = CHARMSG(&msg)->chr;
                _CharPress(somSelf, ev, key, 0);
            } else if(sFlag & KC_VIRTUALKEY) {
                vkey = CHARMSG(&msg)->vkey;
                _VirtCharPress(somSelf, ev, vkey, 0);
            }
            break;
        case WM_BUTTON1DOWN:
            _Button1Press(somSelf, ev, x, y);
            break;
        case WM_BUTTON1CLICK:
            _Button1Click(somSelf, ev, x, y);
            break;
        case WM_BUTTON1DBLCLK:
            _Button1DblClick(somSelf, ev, x, y); /* x,y when pressed */
            break;
        case WM_MOUSEMOVE:
            if(button1motion) {
                x = MOUSEMSG(&msg)->x;
                y = MOUSEMSG(&msg)->y;
                _Button1Drag(somSelf, ev, x, y);
            }
            break;
        case WM_BUTTON1MOTIONSTART:
            button1motion=TRUE;
            _Button1DragBegin(somSelf, ev, x, y); /* x,y when pressed */
            fprintf(stderr, "WM_BUTTON1MOTIONSTART\n");
            break;
        case WM_BUTTON1MOTIONEND:
            button1motion=FALSE;
            _Button1DragEnd(somSelf, ev, x, y); /* x,y when pressed */
            fprintf(stderr, "WM_BUTTON1MOTIONEND\n");
            break;
        case WM_SIZE:
            if(somSelf) {
                w = SHORT1FROMMP(mp2);
                h = SHORT2FROMMP(mp2);
                __set_w(somSelf, ev, w);
                __set_h(somSelf, ev, h);
                _ReSize(somSelf, ev, w, h);
            }
            break;
        case WM_PAINT:
            if (somSelf) {
                hps = WinBeginPaint(hwnd, NULLHANDLE, &rcl);
                _PaintWindow(somSelf, ev, &rcl);
                WinEndPaint(hps);
                return FALSE;
            }
            break;
    }
    return WinDefWindowProc(hwnd, msg, mp1, mp2);
}

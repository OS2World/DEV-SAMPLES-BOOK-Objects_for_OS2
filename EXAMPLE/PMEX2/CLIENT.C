
/************/
/* CLIENT.C */
/************/

#define Client_Class_Source
#include <client.ih>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FNWP vwClientProc;

/* I inherit somInit from SOMObject */

SOM_Scope void  SOMLINK somInit(Client somSelf)
{
/*    ClientData *somThis = ClientGetData(somSelf); */
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

    __set_hwndWindow(somSelf, ev, hwndClient);
    __set_hwndFrame(somSelf, ev, hwndFrame);
}

SOM_Scope void  SOMLINK somUninit(Client somSelf)
{
/*    ClientData *somThis = ClientGetData(somSelf); */
    Environment *ev = somGetGlobalEnvironment();
    Client_parent_Window_somUninit(somSelf);
}

/*
 * x,y when pressed
 */

SOM_Scope void  SOMLINK Button1Click(Client somSelf, Environment *ev,
                short x, short y)
{ }

/*
 *  x,y when clicked
 */


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
        case WM_BUTTON1CLICK:
            _Button1Click(somSelf, ev, x, y);
            break;
        case WM_PAINT:
            if (somSelf) {
                hps = WinBeginPaint(hwnd, NULLHANDLE, &rcl);
                WinDrawText(hps, 39, "SOM PM Objects! Click Mouse Button One!", &rcl, 0L, 0L,
                      DT_ERASERECT | DT_CENTER | DT_VCENTER | DT_TEXTATTRS);
                WinEndPaint(hps);
                return FALSE;
            }
            break;
    }
    return WinDefWindowProc(hwnd, msg, mp1, mp2);
}

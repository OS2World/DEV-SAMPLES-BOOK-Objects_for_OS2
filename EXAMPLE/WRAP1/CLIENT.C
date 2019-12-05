/************/
/* CLIENT.C */
/************/

#define Client_Class_Source
#include <client.ih>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOGRAPHICS     /* */

FNWP vwClientProc;

static char strBuf[100];    /* used by many of the routines to print messages */

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
    string className = "controlsApp";

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
    __set_hwndFrame (somSelf, ev, hwndFrame);

    __set_hwndParent(gBaseWindow,ev, hwndFrame);
    __set_hwndOwner (gBaseWindow,ev, hwndFrame);
    __set_hwndWindow(gBaseWindow,ev, hwndClient);
}

MRESULT EXPENTRY vwClientProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    HPS hps;
    RECTL rcl;
    char key;
    short vkey, sFlag, kbmState;
    Environment *ev = somGetGlobalEnvironment();
    SOMObject somSelf;
    short x, y, w, h;
    SHORT src;
    Window somWin;
    SHORT winID;
    HWND winHwnd;
    static BOOL button1motion=FALSE;

    if(hwnd)
        somSelf = WinQueryWindowPtr(hwnd, 0);

    switch(msg) {
        case WM_SIZE:
            if(somSelf) {
                w = SHORT1FROMMP(mp2);
                h = SHORT2FROMMP(mp2);
                _ReSize(somSelf, ev, w, h);
            }
            break;
#ifdef NOGRAPHICS
        case WM_ERASEBACKGROUND:
            return (MRESULT)TRUE;
#endif
        case WM_PAINT:
            if (somSelf) {
                hps = WinBeginPaint(hwnd, NULLHANDLE, &rcl);
                _PaintWindow(somSelf, ev, &rcl);
                WinEndPaint(hps);
                return FALSE;
            }
            break;
        case WM_COMMAND:
            src = SHORT1FROMMP(mp2);  /* source of WM_COMMAND, should be ..._PUSHBUTTON */
            winID = SHORT1FROMMP(mp1);
            winHwnd = WinWindowFromID(hwnd, winID);
            somWin = WinQueryWindowPtr(winHwnd, 0L);
            fprintf(stderr,"WM_COMMAND received somWin=%p winID=%d %lx %lx\n",somWin, winID, mp1, mp2);
            switch(src) {
                case CMDSRC_PUSHBUTTON:
                    _PushedButton(somSelf, ev, winID);
                    break;
                default:
                    fprintf(stderr,"  pushb.c ERROR: shouldn't have executed pbSubProc\n");
                    break;
            }
            return 0;
    }
    return WinDefWindowProc(hwnd, msg, mp1, mp2);
}

/* The following are stub methods (Virtual) designed to be overridden by applications */

SOM_Scope void  SOMLINK PaintWindow(Client somSelf, Environment *ev,
                RECTL* rcl)
{ }

SOM_Scope void  SOMLINK ReSize(Client somSelf, Environment *ev,
                short w, short h)
{ }

SOM_Scope void  SOMLINK PushedButton(Client somSelf, Environment *ev,
                short id)
{ }


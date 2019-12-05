
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
string className = "clientApp";

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

    Client_parent_Window_somInit(somSelf);

    hwndFrame = WinCreateStdWindow(
            HWND_DESKTOP,
            WS_VISIBLE,
            &flFrameFlags,
            className,
            "Wrapped Control",
            0L,
            (HMODULE)0,
            0L,    /* resource ID */
            &hwndClient);
    if((hwndTitleBar = WinWindowFromID (hwndFrame, FID_TITLEBAR)) != 0L)
        WinSetWindowText (hwndTitleBar, "Wrapped Controls");
    WinSetWindowPtr(hwndClient, 0L, somSelf);

    __set_hwndWindow(somSelf, ev, hwndClient);
    __set_hwndFrame (somSelf, ev, hwndFrame);

    __set_hwndParent(somSelf, ev, hwndFrame);
    __set_hwndOwner (somSelf, ev, hwndFrame);
}

MRESULT EXPENTRY vwClientProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    Environment *ev = somGetGlobalEnvironment();
    SOMObject somSelf;
    Window sourceWindow;
    HPS hps;
    RECTL rcl;
    char key;
    short vkey, sFlag, kbmState;
    short x, y, w, h;
    SHORT src;
    SHORT winID;
    HWND winHwnd;

    if(hwnd)
        somSelf = WinQueryWindowPtr(hwnd, 0);

    switch(msg) {
        case WM_CHAR:
            somSelf = WinQueryWindowPtr(hwnd, 0);
            if(somIsObj(somSelf)) {
                sFlag = CHARMSG(&msg)->fs;
                if(sFlag & KC_KEYUP)
                    return FALSE;
                if(sFlag & KC_CHAR) {
                    key = CHARMSG(&msg)->chr;
                    _CharPress(somSelf, ev, key, 0);
                }
            }
            break;
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
            sourceWindow = WinQueryWindowPtr(winHwnd, 0L);
            fprintf(stderr,"WM_COMMAND received sourceWindow=%p winID=%d %lx %lx\n",sourceWindow, winID, mp1, mp2);
            switch(src) {
                case CMDSRC_PUSHBUTTON:
                    _PushedButton(somSelf, ev, sourceWindow);
                    break;
                default:
                    fprintf(stderr,"  pushb.c ERROR: shouldn't have executed pbSubProc\n");
                    break;
            }
            return 0;
    }
    return WinDefWindowProc(hwnd, msg, mp1, mp2);
}

/* The following are stub methods (Virtual) designed to be overridden */

SOM_Scope void  SOMLINK CharPress(Client somSelf, Environment *ev,
                char key,
                short kbmState)
{ }

SOM_Scope void  SOMLINK PaintWindow(Client somSelf, Environment *ev,
                RECTL* rcl)
{ }

SOM_Scope void  SOMLINK ReSize(Client somSelf, Environment *ev,
                short w, short h)
{ }

/*
 * this method gets called when a user presses down and up on a pushbutton.  The
 * sourceWindow parameter indicates the particular SOMObject that was pressed.
 */

SOM_Scope void  SOMLINK PushedButton(Client somSelf, Environment *ev,
                Window id)
{ }


void SOMLINK clientInit(SOMClass *cls)
{
    Environment *ev = somGetGlobalEnvironment();
    HAB hab = __get_hab(_Window, ev);

    WinRegisterClass(
        hab,    /* anchor block handle */
        className,  /* Window Client Class name */
        vwClientProc,  /* EMClientProc  Client procedure for class */
        CS_SIZEREDRAW, /*   Class style */
        4L);       /*   Extra bytes to reserve for data like HPS */
}

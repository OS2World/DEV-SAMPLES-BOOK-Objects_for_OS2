/***************/
/*** pmapp.c ***/
/***************/

#define PMApp_Class_Source
#include <pmapp.ih>

FNWP vwClientProc;
/*
 *  override the method 'somInit' which I inherit from SOMObject
 */

SOM_Scope void  SOMLINK somInit(PMApp somSelf)
{
    PMAppData *somThis = PMAppGetData(somSelf);
    Environment *ev = somGetGlobalEnvironment();
    short  i;
    static ULONG flFrameFlags = FCF_TITLEBAR    |  FCF_SYSMENU  |
                                FCF_SIZEBORDER  |  FCF_MINMAX   |
                                FCF_SHELLPOSITION | FCF_TASKLIST;
    HWND hwndTitleBar;
    QMSG qmsg;
    static BOOL firsttimein=TRUE;

    _className = "Good Morning SOM";

    if(firsttimein) {
        _hab = WinInitialize(0);
        _hmq = WinCreateMsgQueue (_hab, 0);
        WinRegisterClass(
            _hab,    /* anchor block handle */
            _className,  /* Client Window Class name */
            vwClientProc,  /* EMClientProc  window procedure for class */
            CS_SIZEREDRAW, /*   Class style */
            4L);       /*   Extra bytes to reserve for data like HPS */

        firsttimein=FALSE;
    }
    _hwndFrame = WinCreateStdWindow(
        HWND_DESKTOP,
        WS_VISIBLE,
        &flFrameFlags,
        _className,
        NULL,
        0L,
        (HMODULE)0,
        0L,    /* resource ID */
        &_hwndClient);
    if((hwndTitleBar = WinWindowFromID (_hwndFrame, FID_TITLEBAR)) != 0L)
        WinSetWindowText (hwndTitleBar, _className);

    PMApp_parent_SOMObject_somInit(somSelf);

    while(WinGetMsg(_hab, &qmsg, NULLHANDLE, 0L, 0L))
        WinDispatchMsg(_hab, &qmsg);
}

SOM_Scope void  SOMLINK somUninit(PMApp somSelf)
{
    PMAppData *somThis = PMAppGetData(somSelf);

    WinDestroyWindow(_hwndFrame);
    WinDestroyMsgQueue(_hmq);
    WinTerminate (_hab);

    PMApp_parent_SOMObject_somUninit(somSelf);
}

MRESULT EXPENTRY vwClientProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    HPS hps;
    RECTL rcl;

    switch(msg) {
        case WM_PAINT:
            hps = WinBeginPaint(hwnd, NULLHANDLE, &rcl);
            WinDrawText(hps, (LONG)11, "SOM is cool", &rcl, 0L, 0L,
                DT_ERASERECT | DT_CENTER | DT_VCENTER | DT_TEXTATTRS);
            WinEndPaint(hps);
            return FALSE;
    }
    return WinDefWindowProc(hwnd, msg, mp1, mp2);
}


/************/
/* WINDOW.C */
/************/

#define Window_Class_Source
#include <window.ih>

#include <stdio.h>

Window gBaseWindow;

/*
 * Method from the IDL attribute statement:
 * "attribute string text"
 */

SOM_Scope string  SOMLINK _get_text(Window somSelf, Environment *ev)
{
    WindowData *somThis = WindowGetData(somSelf);
    string textStr;
    long   textLen;

    if(_hwndWindow) {
        textLen = WinQueryWindowTextLength(_hwndWindow)+1;
        textStr = (string)malloc(WinQueryWindowTextLength(_hwndWindow)+1);
        WinQueryWindowText(_hwndWindow, textLen, textStr);
    }

    return textStr;
}

/*
 * Method from the IDL attribute statement:
 * "attribute string text"
 */

SOM_Scope void  SOMLINK _set_text(Window somSelf, Environment *ev,
                string text)
{
    WindowData *somThis = WindowGetData(somSelf); 
    if(_hwndWindow) {
        WinSetWindowText(_hwndWindow, text);
    }
    if(somSelf==gBaseWindow && _hwndFrame)
        WinSetWindowText(_hwndFrame, text);
}

/*
 *   A primitive unique id generator
 */

SOM_Scope short  SOMLINK UniqueID(Window somSelf,  Environment *ev)
{
    static short id=0;
    return (++id);
}

/*
 * this method will return the Window pointer of the SOMobject identified
 * by ID if somSelf is the parent of that window 
 */

SOM_Scope Window  SOMLINK WindowFromID(Window somSelf,  Environment *ev, 
                                        short ID)
{
    WindowData *somThis = WindowGetData(somSelf);
    Window w = NULL;

    WindowMethodDebug("Window","WindowFromID");

    if(_hwndWindow) {
        HWND hwndChild;
        hwndChild = WinWindowFromID(_hwndWindow, ID);
        w = (Window )WinQueryWindowPtr(hwndChild, QWL_USER);
    }
    return w;
}

SOM_Scope void  SOMLINK ProcessEvents(Window somSelf, Environment *ev)
{
    WindowData *somThis = WindowGetData(somSelf);
    QMSG qmsg;
    while(WinGetMsg(_hab, &qmsg, NULLHANDLE, 0L, 0L))
        WinDispatchMsg(_hab, &qmsg);
}

SOM_Scope void  SOMLINK SetSize(Window somSelf,  Environment *ev,
                                long x, long y, long w, long h)
{
    WindowData *somThis = WindowGetData(somSelf);

    if(_hwndWindow) {
        fprintf(stderr,"Set Size: hwndWindow=%lx %ld,%ld %ldX%ld\n",_hwndWindow, x, y, w, h);
        WinSetWindowPos(_hwndWindow,
                    HWND_TOP,
                    x, y, w, h,
                    SWP_MOVE | SWP_SIZE | SWP_ZORDER);
    }
    _x = x;
    _y = y;
    _w = w;
    _h = h;

    fprintf(stderr,"Set Size: end\n");
}

SOM_Scope void  SOMLINK somInit(Window somSelf)
{
    WindowData *somThis = WindowGetData(somSelf);
    Environment *ev = somGetGlobalEnvironment();
    static BOOL firsttimein=TRUE;

    if(firsttimein) {
        _hab = WinInitialize(0);
        _hmq = WinCreateMsgQueue (_hab, 0);
        gBaseWindow = somSelf;
        firsttimein=FALSE;
    }
    _x = _y = _w = _h = 0L;

    Window_parent_SOMObject_somInit(somSelf);
}

SOM_Scope void  SOMLINK somUninit(Window somSelf)
{
    WindowData *somThis = WindowGetData(somSelf);

    WinDestroyWindow(_hwndFrame);
    WinDestroyMsgQueue(_hmq);
    WinTerminate (_hab);

    Window_parent_SOMObject_somUninit(somSelf);
}

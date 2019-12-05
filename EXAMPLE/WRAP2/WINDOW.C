
/************/
/* WINDOW.C */
/************/

#define Window_Class_Source
#define  INCL_GPI
#include <window.ih>

#include <stdio.h>

Window baseWindow;
HMQ    hmq;
HAB    hab;

/*
 * Method from the IDL attribute statement:
 * "attribute string text"
 */


SOM_Scope string  SOMLINK _get_text(Window somSelf,  Environment *ev)
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

SOM_Scope void  SOMLINK _set_text(Window somSelf,  Environment *ev, 
                                  string text)
{
    WindowData *somThis = WindowGetData(somSelf);
    if(_hwndWindow) {
        WinSetWindowText(_hwndWindow, text);
    }
    if(somSelf==baseWindow && _hwndFrame)
        WinSetWindowText(_hwndFrame, text);
}

/*
 *   A primitive unique id generator
 */


SOM_Scope short  SOMLINK UniqueID(Window somSelf,  Environment *ev)
{
    static short id=1;
    return (++id);
}

/*
 * This method will return the pointer to the Window SOMobject identified
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

SOM_Scope void  SOMLINK SetSize(Window somSelf,  Environment *ev, 
                                long x, long y, long w, long h)
{
    WindowData *somThis = WindowGetData(somSelf);

    if(_hwndWindow) {
        WinSetWindowPos(_hwndWindow,
                    HWND_TOP,
                    x, y, w, h,
                    SWP_MOVE | SWP_SIZE | SWP_ZORDER);
    } else
        fprintf(stderr,"Set Size hwndWindow was NULL\n");
    _x = x;
    _y = y;
    _w = w;
    _h = h;
}

/*
 * sets focus to the Window on which this method acts.
 */

SOM_Scope void  SOMLINK SetFocus(Window somSelf,  Environment *ev)
{
    WindowData *somThis = WindowGetData(somSelf);
    HWND hwnd = __get_hwndWindow(somSelf, ev);
    WindowMethodDebug("Window","SetFocus");
    if(hwnd)
        WinSetFocus(HWND_DESKTOP, hwnd);
    return;
}

SOM_Scope void  SOMLINK SetColorFG(Window somSelf,  Environment *ev, 
                                   BYTE r, BYTE g, BYTE b)
{
    WindowData *somThis = WindowGetData(somSelf);
    WindowMethodDebug("Window","SetColorFG");

    if(_hwndWindow) {
        RGB2       rgb;

        rgb.bRed      = r;
        rgb.bGreen    = g;
        rgb.bBlue     = b;
        rgb.fcOptions = 0;
        WinSetPresParam(_hwndWindow, PP_FOREGROUNDCOLOR, (ULONG)sizeof(RGB2), (PVOID)&rgb);
    }

}

SOM_Scope void  SOMLINK SetColorBG(Window somSelf,  Environment *ev, 
                                   BYTE r, BYTE g, BYTE b)
{
    WindowData *somThis = WindowGetData(somSelf);
    WindowMethodDebug("Window","SetColorBG");

    if(_hwndWindow) {
        RGB2       rgb;

        rgb.bRed      = r;
        rgb.bGreen    = g;
        rgb.bBlue     = b;
        rgb.fcOptions = 0;
        WinSetPresParam(_hwndWindow, PP_BACKGROUNDCOLOR, (ULONG)sizeof(RGB2), (PVOID)&rgb);
    }

}

SOM_Scope void  SOMLINK somInit(Window somSelf)
{
    WindowData *somThis = WindowGetData(somSelf);
    Environment *ev = somGetGlobalEnvironment();

    static BOOL firsttimein=TRUE;

    if(firsttimein) {
        baseWindow = somSelf;
        __set_hwndWindow(baseWindow, ev, HWND_DESKTOP);
        __set_hwndFrame (baseWindow, ev, NULLHANDLE);
        __set_hwndParent(baseWindow, ev, NULLHANDLE);
        __set_hwndOwner (baseWindow, ev, NULLHANDLE);
        firsttimein=FALSE;
    }

    _x = _y = _w = _h = 0L;

    Window_parent_SOMObject_somInit(somSelf);
}


SOM_Scope void  SOMLINK somUninit(Window somSelf)
{
    WindowData *somThis = WindowGetData(somSelf);

    if(somSelf==baseWindow) {
        WinDestroyWindow(_hwndFrame);
        WinDestroyMsgQueue(hmq);
        WinTerminate (hab);
    }

    Window_parent_SOMObject_somUninit(somSelf);
}

/*
 *Method from the IDL attribute statement:
 *"readonly attribute Window baseWindow"
 */

SOM_Scope Window  SOMLINK m__get_baseWindow(M_Window somSelf, 
                                             Environment *ev)
{
    /* M_WindowData *somThis = M_WindowGetData(somSelf); */
    M_WindowMethodDebug("M_Window","m__get_baseWindow");

    return baseWindow;
}

/*
 *Method from the IDL attribute statement:
 *"readonly attribute HAB hab"
 */

SOM_Scope HAB  SOMLINK m__get_hab(M_Window somSelf,  Environment *ev)
{
/*  M_WindowData *somThis = M_WindowGetData(somSelf); */
    M_WindowMethodDebug("M_Window","m__get_hab");

    return hab;
}

/*
 *Method from the IDL attribute statement:
 *"readonly attribute HMQ hmq"
 */

SOM_Scope HMQ  SOMLINK m__get_hmq(M_Window somSelf,  Environment *ev)
{
/*  M_WindowData *somThis = M_WindowGetData(somSelf); */
    M_WindowMethodDebug("M_Window","m__get_hmq");

    return hmq;
}

/*
 *Method from the IDL attribute statement:
 *"readonly attribute long screenWidth"
 */

SOM_Scope long  SOMLINK m__get_screenWidth(M_Window somSelf, 
                                            Environment *ev)
{
    /* M_WindowData *somThis = M_WindowGetData(somSelf); */
    M_WindowMethodDebug("M_Window","m__get_screenWidth");

    return ((long)WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN));
}

/*
 *Method from the IDL attribute statement:
 *"readonly attribute long screenHeight"
 */

SOM_Scope long  SOMLINK m__get_screenHeight(M_Window somSelf, 
                                             Environment *ev)
{
    /* M_WindowData *somThis = M_WindowGetData(somSelf); */
    M_WindowMethodDebug("M_Window","m__get_screenHeight");
    return ((long)WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN));
}

SOM_Scope Window  SOMLINK m_CreateWindow(M_Window somSelf,  Environment *ev, 
                                         Window parent, Window owner, 
                                         long style, string initText, 
                                         short x, short y, short w, 
                                         short h, somToken userStruct)
{ }

SOM_Scope void  SOMLINK m_ProcessEvents(M_Window somSelf,  Environment *ev)
{
/*    M_WindowData *somThis = M_WindowGetData(somSelf); */
    QMSG qmsg;
    M_WindowMethodDebug("M_Window","m_ProcessEvents");

    while(WinGetMsg(hab, &qmsg, NULLHANDLE, 0L, 0L))
        WinDispatchMsg(hab, &qmsg);
}

/*
 * this next method will be executed first before all instance methods and class methods that
 * are introduced at the Window level or by its subclasses.
 */

void SOMLINK m_windowInit(SOMClass *cls)
{
    hab = WinInitialize(0);
    hmq = WinCreateMsgQueue (hab, 0);
}

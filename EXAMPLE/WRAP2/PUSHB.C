
/***********/
/* PUSHB.C */
/***********/

#define PushButton_Class_Source
#include <pushb.ih>

#include <stdio.h>

/*
 * allow for programmatic press of push button
 */

SOM_Scope void  SOMLINK DoPress(PushButton somSelf,  Environment *ev)
{
    /* PushButtonData *somThis = PushButtonGetData(somSelf); */
    HWND hwnd = __get_hwndWindow(somSelf, ev);
    PushButtonMethodDebug("PushButton","DoPress");
    if(hwnd)
        WinSendMsg(hwnd, BM_CLICK, MPFROMSHORT(TRUE), NULL);
    return;
}

/*
 * allow for programmatic release of push button
 */

/*
 * allow for programmatic release push button
 */

SOM_Scope void  SOMLINK DoRelease(PushButton somSelf,  Environment *ev)
{
    /* PushButtonData *somThis = PushButtonGetData(somSelf); */
    HWND hwnd = __get_hwndWindow(somSelf, ev);
    PushButtonMethodDebug("PushButton","DoRelease");
    if(hwnd)
        WinSendMsg(hwnd, BM_CLICK, MPFROMSHORT(FALSE), NULL);
    return;
}



/*
 * SOM_Scope Window  SOMLINK m_CreateWindow(M_PushButton somSelf,
 *              Environment *ev, Window  parent, Window  owner, long style,
 *              string initText, short x, short y, short w, short h)
 */

/*
 * The prototype for m_CreateWindow was replaced by the following prototype:
 */
SOM_Scope Window  SOMLINK m_CreateWindow(M_PushButton somSelf, 
                                          Environment *ev, Window parent, 
                                         Window owner, long style, 
                                         string initText, short x, 
                                         short y, short w, short h, 
                                         somToken userStruct)
{
    /* M_PushButtonData *somThis = M_PushButtonGetData(somSelf); */
    PushButton pbNew = PushButtonNew();

    HWND hwndParent, hwndOwner, hwnd;
    USHORT idPushButton = _UniqueID(pbNew, ev);
    ERRORID errorid;
    USHORT i;

    M_PushButtonMethodDebug("M_PushButton","m_CreateWindow");
/* use the base window as the parent and owner */

    hwndParent = __get_hwndWindow(parent, ev);
    hwndOwner = __get_hwndWindow(owner, ev);

    __set_hwndParent(pbNew,ev, hwndParent);
    __set_hwndOwner(pbNew,ev, hwndOwner);

    hwnd =WinCreateWindow(
                   hwndParent,
                   WC_BUTTON,              /* window class */
                   initText,               /* window text */
                   BS_PUSHBUTTON | style,  /* window style */
                   (long)x, (long)y,       /* position */
                   (long)w, (long)h,       /* size */
                   hwndOwner,              /* owner window */
                   HWND_BOTTOM,            /* placement */
                   idPushButton,           /* child window id */
                   NULL,                   /* Control data */
                   NULL);                  /* pres. params */

    WinSetWindowPtr(hwnd, QWL_USER, pbNew);   /* store the object pointer */
                           /* in the PM window's word indexed by QWL_USER */
    __set_hwndWindow(pbNew, ev, hwnd);
    __set_hwndFrame(pbNew, ev, NULLHANDLE);

    return pbNew;
}

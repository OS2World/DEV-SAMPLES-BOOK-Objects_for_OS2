
/************/
/* STATIC.C */
/************/

#define StaticText_Class_Source
#define M_StaticText_Class_Source
#include <static.ih>


/*
 * SOM_Scope Window  SOMLINK m_CreateWindow(M_StaticText somSelf, 
 *                                           Environment *ev, Window parent, 
 *                                          Window owner, long style, 
 *                                          string initText, short x, 
 *                                          short y, short w, short h)
 */

/*
 * The prototype for m_CreateWindow was replaced by the following prototype:
 */
SOM_Scope Window  SOMLINK m_CreateWindow(M_StaticText somSelf, 
                                          Environment *ev, Window parent, 
                                         Window owner, long style, 
                                         string initText, short x, 
                                         short y, short w, short h, 
                                         somToken userStruct)
{
    /* M_StaticTextData *somThis = M_StaticTextGetData(somSelf); */
    StaticText stNew = StaticTextNew();

    HWND hwndParent, hwndOwner, hwnd;
    USHORT idStaticText = _UniqueID(stNew, ev);
    ERRORID errorid;
    USHORT i;

    M_StaticTextMethodDebug("M_StaticText","m_CreateWindow");

    M_StaticTextMethodDebug("M_StaticText","m_CreateWindow");

    if(!style)
            style = WS_VISIBLE | SS_TEXT | DT_CENTER | DT_VCENTER;

    hwndParent = __get_hwndWindow(parent, ev);
    hwndOwner = __get_hwndWindow(owner, ev);

    __set_hwndParent(stNew, ev, hwndParent);
    __set_hwndOwner(stNew, ev, hwndOwner);

    hwnd =WinCreateWindow(
                   hwndParent,
                   WC_STATIC,              /* window class */
                   initText,               /* window text */
                   style,                  /* window style */
                   (long)x, (long)y,       /* position */
                   (long)w, (long)h,       /* size */
                   hwndOwner,              /* owner window */
                   HWND_BOTTOM,            /* placement */
                   idStaticText,           /* child window id */
                   NULL,                   /* Control data */
                   NULL);                  /* pres. params */

    WinSetWindowPtr(hwnd, QWL_USER, stNew);   /* store the object pointer */
                           /* in the PM window's word indexed by QWL_USER */
    __set_hwndWindow(stNew, ev, hwnd);
    __set_hwndFrame(stNew, ev, NULLHANDLE);

    return stNew;
}


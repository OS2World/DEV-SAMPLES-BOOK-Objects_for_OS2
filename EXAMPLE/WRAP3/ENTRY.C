
#define EntryField_Class_Source
#define M_EntryField_Class_Source
#include <entry.ih>

SOM_Scope Window  SOMLINK m_CreateWindow(M_EntryField somSelf, 
                                          Environment *ev, Window parent, 
                                         Window owner, long style, 
                                         string initText, short x, 
                                         short y, short w, short h, 
                                         somToken userStruct)
{
    /* M_EntryFieldData *somThis = M_EntryFieldGetData(somSelf); */
    HWND hwndParent, hwndOwner, hwnd;
    EntryField efNew = EntryFieldNew();
    USHORT idEntryField = _UniqueID(efNew, ev);
    ERRORID errorid;
    USHORT i;

    M_EntryFieldMethodDebug("M_EntryField","m_CreateWindow");
  /* use the base window as the parent and owner */

    if(!style)  /* if style is 0 give the user a default style */
         style = WS_VISIBLE | ES_AUTOSCROLL | ES_MARGIN;

    hwndParent = __get_hwndWindow(parent, ev);
    hwndOwner = __get_hwndWindow(owner, ev);

    __set_hwndParent(efNew,ev, hwndParent);
    __set_hwndOwner(efNew,ev, hwndOwner);

    hwnd =WinCreateWindow(
                   hwndParent,
                   WC_ENTRYFIELD,          /* window class */
                   initText,               /* window text */
                   style,                  /* window style */
                   (long)x, (long)y,       /* position */
                   (long)w, (long)h,       /* size */
                   hwndOwner,              /* owner window */
                   HWND_BOTTOM,            /* placement */
                   idEntryField,           /* child window id */
                   NULL,                   /* Control data */
                   NULL);                  /* pres. params */

    WinSetWindowPtr(hwnd, QWL_USER, efNew);   /* store the object pointer */
                           /* in the PM window's word indexed by QWL_USER */

    __set_hwndWindow(efNew, ev, hwnd);
    __set_hwndFrame(efNew, ev, NULLHANDLE);

    return efNew;
}


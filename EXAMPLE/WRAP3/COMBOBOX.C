
#define ComboBox_Class_Source
#define M_ComboBox_Class_Source
#include <combobox.ih>

SOM_Scope Window  SOMLINK m_CreateWindow(M_ComboBox somSelf, 
                                          Environment *ev, Window parent, 
                                         Window owner, long style, 
                                         string initText, short x, 
                                         short y, short w, short h, 
                                         somToken userStruct)
{
    /* M_ComboBoxData *somThis = M_ComboBoxGetData(somSelf); */
    ComboBox comboNew = ComboBoxNew();
    HWND hwndParent, hwndOwner, hwnd;
    USHORT idComboBox = _UniqueID(comboNew, ev);
    ERRORID errorid;
    USHORT i;
    M_ComboBoxMethodDebug("M_ComboBox","m_CreateWindow");

    if(!style)    /* give a default */
        style = WS_VISIBLE | CBS_DROPDOWN;
    hwndParent = __get_hwndWindow(parent, ev);
    hwndOwner = __get_hwndWindow(owner, ev);

    __set_hwndParent(comboNew,ev, hwndParent);
    __set_hwndOwner(comboNew,ev, hwndOwner);

    hwnd =WinCreateWindow(
                   hwndParent,
                   WC_COMBOBOX,              /* window class */
                   initText,               /* window text */
                   style,  /* window style */
                   (long)x, (long)y,       /* position */
                   (long)w, (long)h,       /* size */
                   hwndOwner,              /* owner window */
                   HWND_BOTTOM,            /* placement */
                   idComboBox,           /* child window id */
                   NULL,                   /* Control data */
                   NULL);                  /* pres. params */

    WinSetWindowPtr(hwnd, QWL_USER, comboNew);   /* store the object pointer */
                           /* in the PM window's word indexed by QWL_USER */

    __set_hwndWindow(comboNew, ev, hwnd);
    __set_hwndFrame(comboNew, ev, NULLHANDLE);

    return comboNew;
}

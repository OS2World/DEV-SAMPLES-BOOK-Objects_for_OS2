
/*************/
/* LISTBOX.C */
/*************/

#define ListBox_Class_Source
#define M_ListBox_Class_Source
#include <listbox.ih>

/*
 * itemIndex can be the zero-based index indicating the
 * desired placement of the itemText, or it can be one
 * of the listbox constants: LIT_SORTASCENDING, LIT_SORTDESCENDING,
 *     LIT_END.
 */

SOM_Scope short  SOMLINK InsertItem(ListBox somSelf,  Environment *ev, 
                                    string itemText, short itemIndex)
{
    /* ListBoxData *somThis = ListBoxGetData(somSelf); */
    HWND hwnd = __get_hwndWindow(somSelf, ev);

    ListBoxMethodDebug("ListBox","InsertItem");

    if(hwnd) 
        return (short)WinSendMsg(hwnd, LM_INSERTITEM, MPFROMSHORT(itemIndex), MPFROMP(itemText));
    else
        return -1;
}

SOM_Scope void  SOMLINK ClearList(ListBox somSelf,  Environment *ev)
{
    /* ListBoxData *somThis = ListBoxGetData(somSelf); */
    HWND hwnd = __get_hwndWindow(somSelf, ev);

    ListBoxMethodDebug("ListBox","ClearList");
    if(hwnd)
        WinSendMsg(hwnd, LM_DELETEALL, 0, 0);
    return;

}


SOM_Scope string  SOMLINK GetItemSelected(ListBox somSelf,  Environment *ev)
{
    /* ListBoxData *somThis = ListBoxGetData(somSelf); */
    short itemIndex, itemLength;
    static string itemText=NULL;
    HWND hwnd = __get_hwndWindow(somSelf, ev);
    ListBoxMethodDebug("ListBox","GetItemSelected");

    itemIndex = (SHORT)WinSendMsg(hwnd, LM_QUERYSELECTION, NULL, NULL); 
              /* single selection style makes mp1 and mp2 ignored */
    if(itemIndex == LIT_NONE)
        return NULL;
    itemLength = (SHORT)WinSendMsg(hwnd, LM_QUERYITEMTEXTLENGTH, MPFROMSHORT(itemIndex),NULL);
    itemText = (string)malloc(itemLength+1);
    WinSendMsg(hwnd, LM_QUERYITEMTEXT, MPFROM2SHORT(itemIndex, itemLength+1), itemText);
    /* Return statement to be customized: */
    return itemText;
}



/*
 * SOM_Scope Window  SOMLINK m_CreateWindow(M_ListBox somSelf,  Environment *ev, 
 *                                          Window parent, Window owner, 
 *                                          long style, string initText, 
 *                                          short x, short y, short w, 
 *                                          short h)
 */

/*
 * The prototype for m_CreateWindow was replaced by the following prototype:
 */
SOM_Scope Window  SOMLINK m_CreateWindow(M_ListBox somSelf,  Environment *ev, 
                                         Window parent, Window owner, 
                                         long style, string initText, 
                                         short x, short y, short w, 
                                         short h, somToken userStruct)
{
    /* M_ListBoxData *somThis = M_ListBoxGetData(somSelf); */
    ListBox lbNew = ListBoxNew();
    HWND hwndParent, hwndOwner, hwnd;
    USHORT idListBox = _UniqueID(lbNew, ev);
    ERRORID errorid;
    USHORT i;

    M_ListBoxMethodDebug("M_ListBox","m_CreateWindow");

    if(!style)
        style = WS_VISIBLE | LS_NOADJUSTPOS;
    hwndParent = __get_hwndWindow(parent, ev);
    hwndOwner = __get_hwndWindow(owner, ev);

    __set_hwndParent(lbNew,ev, hwndParent);
    __set_hwndOwner(lbNew,ev, hwndOwner);

    hwnd =WinCreateWindow(
                   hwndParent,
                   WC_LISTBOX,              /* window class */
                   initText,               /* window text */
                   style,  /* window style */
                   (long)x, (long)y,       /* position */
                   (long)w, (long)h,       /* size */
                   hwndOwner,              /* owner window */
                   HWND_BOTTOM,            /* placement */
                   idListBox,           /* child window id */
                   NULL,                   /* Control data */
                   NULL);                  /* pres. params */

    WinSetWindowPtr(hwnd, QWL_USER, lbNew);   /* store the object pointer */
                           /* in the PM window's word indexed by QWL_USER */
    __set_hwndWindow(lbNew, ev, hwnd);
    __set_hwndFrame(lbNew, ev, NULLHANDLE);

    return lbNew;

}


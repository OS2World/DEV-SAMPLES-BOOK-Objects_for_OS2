/***********/
/* PUSHB.C */
/***********/

#define PushButton_Class_Source
#include <pushb.ih>

#include <stdio.h>

SOM_Scope void  SOMLINK somInit(PushButton somSelf)
{
    /* PushButtonData *somThis = PushButtonGetData(somSelf); */

    HWND hwndParent, hwndOwner, hwnd;
    Environment *ev = somGetGlobalEnvironment();
    USHORT idPushButton = 1;
    ERRORID errorid;
    USHORT i;

/* use the base window as the parent and owner this time */

    hwndParent = __get_hwndWindow(gBaseWindow,ev);
    hwndOwner = __get_hwndWindow(gBaseWindow,ev);

    __set_hwndParent(somSelf,ev, hwndParent);
    __set_hwndOwner(somSelf,ev, hwndOwner);

    hwnd =WinCreateWindow(
                   hwndParent,
                   WC_BUTTON,   /* window class */
                   "I'm a Push Button",           /* window text */
                   WS_VISIBLE | BS_PUSHBUTTON,  /* window style */
                   0L, 0L, 0L, 0L,         /* position and size */
                   hwndOwner,        /* owner window */
                   HWND_BOTTOM,    /* placement */
                   idPushButton,         /* child window id */
                   NULL,           /* Control data */
                   NULL);          /* pres. params */
    WinSetWindowPtr(hwnd, 0L, somSelf);
    __set_hwndWindow(somSelf, ev, hwnd);
    __set_hwndFrame(somSelf, ev, NULLHANDLE);
    fprintf(stderr,"push button created. hwnd=%lx\n",hwnd);
    PushButton_parent_Window_somInit(somSelf);
}

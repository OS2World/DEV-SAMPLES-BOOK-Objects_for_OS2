
/************/
/* CLIENT.C */
/************/

#define Client_Class_Source
#include <client.ih>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <listbox.h>
#include <combobox.h>
                        /* defing NOGRAPHICS means that: */
#define NOGRAPHICS      /* Only PM Controls at play here */
                        /* I don't do any of my own painting */
FNWP vwClientProc;

static char strBuf[100];    /* used by many of the routines to print messages */

MRESULT EXPENTRY vwClientProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    SHORT winID;
    HWND winHwnd, hwndFocus, hwndFocusParent;
    HPS hps;
    RECTL rcl;
    SHORT src;
    char key;
    short vkey, sFlag, kbmState;
    short x, y, w, h;
    static string itemText=NULL;
    short itemIndex;  /* index of item selected in list box or combo box */
    short itemLength;
    Environment *ev = somGetGlobalEnvironment();
    SOMObject somSelf;
    Window sourceWindow;

    if(hwnd)
        somSelf = WinQueryWindowPtr(hwnd, 0);

    switch(msg) {
        case WM_CHAR:
            sFlag = CHARMSG(&msg)->fs;
            if(sFlag & KC_KEYUP)
                return FALSE;       /* ignore key-up messages */
            if(sFlag & KC_CHAR) {
                key = CHARMSG(&msg)->chr;
                _CharPress(somSelf, ev, key, 0);
            }

            if(sFlag & KC_VIRTUALKEY) {
                vkey = CHARMSG(&msg)->vkey;
                _VirtCharPress(somSelf, ev, vkey, 0);
                switch(vkey) {   /* virtual key located in mp2 */
                    case VK_NEWLINE:
                    case VK_ENTER:
                        hwndFocus = WinQueryFocus(HWND_DESKTOP);
                        sourceWindow = WinQueryWindowPtr(hwndFocus, 0L);
                        if(somIsObj(sourceWindow) && !_somIsA(sourceWindow, ComboBoxNewClass(0,0)) )
                            _EnterKeyPressed(somSelf, ev, sourceWindow);
                        break;
                    default:
                        break;
                };
            }
            break;
        case WM_SIZE:
            if(somIsObj(somSelf)) {
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
            if (somIsObj(somSelf)) {
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
            switch(src) {
                case CMDSRC_PUSHBUTTON:
                    _PushedButton(somSelf, ev, sourceWindow);
                    break;
                default:
                    fprintf(stderr,"  pushb.c ERROR: shouldn't have executed pbSubProc\n");
                    break;
            }
            return 0;
        case WM_CONTROL:
            winID = SHORT1FROMMP(mp1);
            winHwnd = WinWindowFromID(hwnd, winID);
            sourceWindow = WinQueryWindowPtr(winHwnd, 0L);
            if(somIsObj(sourceWindow)) {
                switch(SHORT2FROMMP(mp1)) {

                    /* the set of control notification messages, for example LN_... for listbox
                     * and CBN_... for combobox, are not unique.  For example the following three
                     * notification messages for three different controls are all #defined as 4:
                     * EN_CHANGE, CBN_LBSELECT, and LN_SCROLL.  Therefore we must use the
                     * som utility, _somIsA, to help us ensure that source control is of a class
                     * that can be used for a particular method.
                     */
                    case LN_SELECT:
                    /* be careful: could also have been CBN_EFCHANGE, or EN_SETFOCUS (all #defined as 1) */
                        if(_somIsA(sourceWindow, ListBoxNewClass(0,0)) && !_somIsA(sourceWindow, ComboBoxNewClass(0,0)) ) {
                            _ListItemSelected(somSelf, ev, sourceWindow);
                        }
                        return 0;
                    case CBN_ENTER:
                        if( _somIsA(sourceWindow, ComboBoxNewClass(0,0)) ) {
                            _EnterKeyPressed(somSelf, ev, sourceWindow);
                        }
                        return 0;
                    case CBN_LBSELECT:
                    /* be careful: could also have been EN_CHANGE, or LN_SCROLL (all #defined as 4)  */
                        if(_somIsA(sourceWindow, ComboBoxNewClass(0,0))) {
                            _ListItemSelected(somSelf, ev, sourceWindow);
                        }
                        return 0;
                }  /* end of switch(sourceWindow) */
            }  /* end of if(somIsObj(sourceWindow)) */
        default:
            break;

    }
    return WinDefWindowProc(hwnd, msg, mp1, mp2);
}

/*
 *
 *  The following are stub methods (Virtual) designed to be overridden
 *
 */

SOM_Scope void  SOMLINK PaintWindow(Client somSelf,  Environment *ev,
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

/*
 * this is called when a user hits the Enter or NewLine key
 *    in an Entry Field or ComboBox.
 */

SOM_Scope void  SOMLINK EnterKeyPressed(Client somSelf,  Environment *ev,
                                        Window sourceWindow)
{ }


SOM_Scope void  SOMLINK CharPress(Client somSelf, Environment *ev,
                char key, short kbmState)
{ }

SOM_Scope void  SOMLINK VirtCharPress(Client somSelf, Environment *ev,
                short vkey, short kbmState)
{ }

/*
 * ListItemSelected is called when a user makes a selection
 *   in a ListBox or ComboBox.
 */

/*
 * this is called when a user makes a selection in a ListBox or ComboBox
 */

SOM_Scope void  SOMLINK ListItemSelected(Client somSelf,  Environment *ev,
                                         Window sourceWindow)
{ }


void SOMLINK clientInit(SOMClass *cls)
{
    HAB hab = WinQueryAnchorBlock(HWND_DESKTOP);
    string className = "Wrapped Controls";

    WinRegisterClass(
            hab,    /* anchor block handle */
            className,  /* Window Client Class name */
            vwClientProc,  /* EMClientProc  Client procedure for class */
            CS_SIZEREDRAW, /*   Class style */
            4L);       /*   Extra bytes to reserve for data like HPS */
}

SOM_Scope Window  SOMLINK m_CreateWindow(M_Client somSelf,  Environment *ev, 
                                         Window parent, Window owner, 
                                         long style, string initText, 
                                         short x, short y, short w, 
                                         short h, somToken userStruct)
{
    /* M_ClientData *somThis = M_ClientGetData(somSelf); */
    Client newClient = _somNew(somSelf);
    static ULONG flFrameFlags = FCF_TITLEBAR    |  FCF_SYSMENU  |
                                FCF_SIZEBORDER  |  FCF_MINMAX   |
                                FCF_SHELLPOSITION | FCF_TASKLIST;
    HWND hwndTitleBar, hwndFrame, hwndClient, hwndParent, hwndOwner;
    string className = "Wrapped Controls";
    M_ClientMethodDebug("M_Client","m_CreateWindow");
    if(parent) {
        hwndParent = __get_hwndWindow(parent, ev);
        hwndOwner = __get_hwndWindow(owner, ev);
    } else {
        hwndParent = hwndOwner = HWND_DESKTOP;
    }
    hwndFrame = WinCreateStdWindow(
            hwndParent,
            0,     /* I turned off the WS_VISIBLE flag here */
            &flFrameFlags,
            className,
            NULL,
            0L,
            (HMODULE)0,
            0L,    /* resource ID */
            &hwndClient);
    if((hwndTitleBar = WinWindowFromID (hwndFrame, FID_TITLEBAR)) != 0L)
        WinSetWindowText (hwndTitleBar, className);
    WinSetWindowPos(
            hwndFrame,
            HWND_TOP,
            x, y, w, h,
            SWP_ACTIVATE | SWP_SHOW | SWP_MOVE | SWP_SIZE | SWP_ZORDER);
/* store the newClient Window pointer in the PM window's user space */
/* for both the client and frame */
    WinSetWindowPtr(hwndClient, 0L, newClient);
    WinSetWindowPtr(hwndFrame, 0L, newClient);

/* further initialize the new Client object */
    __set_hwndWindow(newClient, ev, hwndClient);
    __set_hwndFrame (newClient, ev, hwndFrame);

    __set_hwndParent(newClient, ev, hwndFrame);
    __set_hwndOwner (newClient, ev, hwndFrame);


    return newClient;
}


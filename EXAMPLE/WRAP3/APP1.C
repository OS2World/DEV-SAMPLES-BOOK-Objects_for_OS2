
#define App1_Class_Source

#include <app1.ih>
#include <static.h>
#include <entry.h>
#include <listbox.h>
#include <combobox.h>
#include <pushb.h>

#include <stdio.h>
#include <string.h>

SOM_Scope void  SOMLINK PushedButton(App1 somSelf,  Environment *ev,
                                      Window sourceWindow)
{
    App1Data *somThis = App1GetData(somSelf);
    App1MethodDebug("App1","PushedButton");
    _ClearList(_lb, ev);
}

SOM_Scope void  SOMLINK EnterKeyPressed(App1 somSelf,  Environment *ev,
                                        Window sourceWindow)
{
    App1Data *somThis = App1GetData(somSelf);
    char strBuf[256];
    App1MethodDebug("App1","EnterKeyPressed");

    if(sourceWindow==_ef || sourceWindow==_cb) {    /* only interested in Entry Field and Combo Box */
        EntryField efTmp = sourceWindow;
        char *efText = __get_text(sourceWindow, ev);

        sprintf(strBuf, "EntryField Text: %s", efText);
        __set_text(_msgField, ev, strBuf);  /* set the text on the title bar */
        _InsertItem(_lb, ev, efText, LIT_SORTASCENDING);
        _InsertItem(_cb, ev, efText, LIT_SORTASCENDING);

        __set_text(_ef, ev, "");
        free(efText);
    }
    return;
}


SOM_Scope void  SOMLINK ListItemSelected(App1 somSelf,  Environment *ev,
                                         Window sourceWindow)
{
    App1Data *somThis = App1GetData(somSelf);
    char strBuf[256];
    string itemText;

    App1MethodDebug("App1","ListItemSelected");

    itemText = _GetItemSelected(sourceWindow, ev);
    if(!itemText)
        return;
    sprintf(strBuf, "Selection from List or Combo: %s", itemText);
    __set_text(_msgField, ev, strBuf);  /* set the text on the title bar */
    free(itemText);  /* method _GetSelectedItem allocates memory for my string that I must free */

}


SOM_Scope Window  SOMLINK m_CreateWindow(M_App1 somSelf,  Environment *ev, 
           Window parent, Window owner, long style, string initText, 
           short x, short y, short w, short h, somToken userStruct)
{
    /* M_App1Data *somThis = M_App1GetData(somSelf); */
    App1Data *somThat;
    ListBox lb;
    StaticText st;
    PushButton pb;
    App1 appOne;
    short i;
    BYTE fgRGB=255, bgRGB=0;

    M_App1MethodDebug("M_App1","m_CreateWindow");

    appOne = M_App1_parent_M_Client_CreateWindow(somSelf, ev,
          parent, owner, style, initText, x, y, w, h, userStruct);

    somThat = App1GetData(appOne);


    fprintf(stderr,"M_CreateWindow of app1.c: begin\n");

    EntryFieldNewClass(0,0);
    StaticTextNewClass(0,0);
    ListBoxNewClass(0,0);
    ComboBoxNewClass(0,0);

    PushButtonNewClass(0,0);
    somThat->ef = _CreateWindow(_EntryField, ev,
                appOne,      /* parent */
                appOne,      /* owner */
                0L, "",
                10, 60, 300, 40, (somToken)0);
    st = _CreateWindow(_StaticText, ev,
                appOne,      /* parent */
                appOne,      /* owner */
                WS_VISIBLE | SS_TEXT | DT_LEFT | DT_BOTTOM,
                "Enter text for next ListBox item:",
                10, 110, 300, 40, (somToken)0);
    pb = _CreateWindow(_PushButton, ev,
                appOne,      /* parent */
                appOne,      /* owner */
                WS_VISIBLE | BS_NOPOINTERFOCUS,  /* a PM style for buttons who won't take focus */
                "Clear List Box",
                10, 170, 300, 60, (somToken)0);
    somThat->msgField = _CreateWindow(_StaticText, ev,
                appOne,      /* parent */
                appOne,      /* owner */
                WS_VISIBLE | SS_TEXT | DT_LEFT | DT_VCENTER, "Messages:",
                10, 10, 560, 40, (somToken)0);
    somThat->lb = _CreateWindow(_ListBox, ev,
                appOne,      /* parent */
                appOne,      /* owner */
                0L, NULL,     /* no style indicated (I will get default) and no initial text */
                320, 60, 250, 230, (somToken)0);
    somThat->cb = _CreateWindow(_ComboBox, ev,
                appOne,      /* parent */
                appOne,      /* owner */
                0L, NULL,     /* no style indicated (I will get default) and no initial text */
                10, 60, 300, 200, (somToken)0);

    _SetColorFG(somThat->ef, ev, fgRGB, fgRGB, fgRGB);        /* black foreground */
    _SetColorBG(somThat->ef, ev, bgRGB, bgRGB, bgRGB);  /* on white background */
    _SetColorFG(somThat->lb, ev, 255, 255, 255);
    _SetColorBG(somThat->lb, ev, 255, 0, 0);
    _SetColorFG(somThat->msgField, ev, 255, 255, 255);
    _SetColorBG(somThat->msgField, ev, 0, 0, 255);
    _SetFocus(somThat->ef, ev);

}


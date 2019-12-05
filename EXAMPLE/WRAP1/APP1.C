/**********/
/* APP1.C */
/**********/

#define App1_Class_Source
#include <app1.ih>

#include <stdio.h>

#include <pushb.h>

SOM_Scope void  SOMLINK somInit(App1 somSelf)
{
    App1Data *somThis = App1GetData(somSelf);
    PushButton pb;
    Environment *ev;

    App1_parent_Client_somInit(somSelf);

    _pb = PushButtonNew();
    _SetSize(_pb, ev, 10L, 10L, 240L, 60L);
}

SOM_Scope void  SOMLINK PushedButton(App1 somSelf, Environment *ev,
                short id)
{
    App1Data *somThis = App1GetData(somSelf);
    char strBuf[20];

    _timesPressed++;
    sprintf(strBuf, "%d", _timesPressed);
    __set_text(_pb, ev, strBuf);
    sprintf(strBuf, "Pressed %d times", _timesPressed);
    __set_text(somSelf, ev, strBuf);
}


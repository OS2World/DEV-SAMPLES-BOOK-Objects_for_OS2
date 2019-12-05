
/**********/
/* APP1.C */
/**********/

#define App1_Class_Source

#include <app1.ih>

#include <stdio.h>

static char appString[30];

SOM_Scope void  SOMLINK Button1Click(App1 somSelf, Environment *ev,
                short x, short y)
{
    App1Data *somThis = App1GetData(somSelf);
    _timesClicked++;
    sprintf(appString,"times clicked=%d", _timesClicked);
    __set_text(somSelf, ev, appString);

}

SOM_Scope void  SOMLINK somInit(App1 somSelf)
{
    App1Data *somThis = App1GetData(somSelf);
    Environment *ev = somGetGlobalEnvironment();

    App1_parent_Client_somInit(somSelf);
    _timesClicked = 0;
    sprintf(appString,"Click Mouse Button 1.  Current times clicked is %d", _timesClicked);
    __set_text(somSelf, ev, appString);
}

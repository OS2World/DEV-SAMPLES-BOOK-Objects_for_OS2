
/*************/
/** eaview.c ***/
/*************/

#include <eafile.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    EAFile a;
    Environment *ev = somGetGlobalEnvironment();

    M_EAFile clsObj = EAFileNewClass(0,0);
    if(argc>1)
        a = _getFirstFile(clsObj, ev, argv[1]);
    else
        a = _getFirstFile(clsObj, ev, "*");

    while(a) {
        printf("%18s\n", __get_name(a, ev));
        _listEAs(a, ev);
        _somFree(a);
        a = _getNextFile(clsObj, ev);
    }

    return(0);
}

/***********/
/* TEST3.C */
/***********/

#include <boxdraw.h>
void main(int argc, char *argv[])
{
    Boxdraw obj = BoxdrawNew();
    _ProcessEvents(obj, somGetGlobalEnvironment());
    _somFree(obj);
}

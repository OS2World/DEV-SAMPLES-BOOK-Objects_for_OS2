/***********/
/* TEST3.C */
/***********/

#include <app1.h>
void main(int argc, char *argv[])
{
    App1 obj = App1New();
    _ProcessEvents(obj, somGetGlobalEnvironment());
    _somFree(obj);
}

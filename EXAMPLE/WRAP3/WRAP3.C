#include <app1.h>
void main(int argc, char *argv[])
{
    Environment *ev = somGetGlobalEnvironment();
    App1 obj = App1New();
    obj =  _CreateWindow(_App1, ev,
                (Window)NULL,   /* parent will be HWND_DESKTOP */
                (Window)NULL,   /* owner as well */
                0L, "Wrapped Controls Three!",
                20, 20, 580, 330, (somToken)0);
    _ProcessEvents(_Window, somGetGlobalEnvironment());
    _somFree(obj);
}

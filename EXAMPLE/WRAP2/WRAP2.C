#include <app1.h>
void main(int argc, char *argv[])
{
    App1 obj = App1New();
    _ProcessEvents(_Window, somGetGlobalEnvironment());
    _somFree(obj);
}

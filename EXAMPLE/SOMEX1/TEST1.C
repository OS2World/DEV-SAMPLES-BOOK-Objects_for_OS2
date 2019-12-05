#include <ex1.h>
int main(int argc, char *argv[])
{
    ExampleOne obj;
    Environment *ev=somGetGlobalEnvironment();

    obj = ExampleOneNew();

    _xSayHi(obj, ev);

    _somFree(obj);

    return (0);
}

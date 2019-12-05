#include <ex3.h>  /* the Hello class and M_Hello metaclass are defined here */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    Hello a, b, c;
    Environment *ev = somGetGlobalEnvironment();
    M_Hello hello = HelloNewClass(Hello_MajorVersion, Hello_MinorVersion);

    printf("now create the 3 Hello objects\n");
    a = _HelloCreate(hello, ev, "Hello from A");
    b = _HelloCreate(hello, ev, "Hello from B");
    c = _HelloCreate(hello, ev, "Hello from C");

    printf("and then say Hello on each of the objects\n");
    _sayHello(a, ev);
    _sayHello(b, ev);
    _sayHello(c, ev);

    printf("now free up the space\n");
    _somFree(c);
    _somFree(b);
    _somFree(a);

    return (0);
}

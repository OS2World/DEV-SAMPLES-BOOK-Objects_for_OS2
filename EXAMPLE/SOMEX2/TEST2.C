/************/
/** test2.c */
/************/

#include <ex2.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    ExampleTwo obj;
    Environment *ev=somGetGlobalEnvironment();

    obj = ExampleTwoNew();  /* creates an instance of class ExampleTwo */
    _xSayHi(obj, ev);
    __set_msg(obj, ev, "Pssst, this is a msg");
    __set_mylong(obj, ev, 4444L);
    _xSayHi (obj, ev);
    _somFree (obj);

    return (0);
}

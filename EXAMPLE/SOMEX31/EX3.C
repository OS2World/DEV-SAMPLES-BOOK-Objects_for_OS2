
#define Hello_Class_Source
#define M_Hello_Class_Source
#include <ex3.ih>

#include <stdio.h>

/*
 *  This method creates an instance of the Hello class and
 *  uses 'message' parameter to initialize the Hello Objects
 */

SOM_Scope Hello  SOMLINK HelloCreate(M_Hello somSelf,  Environment *ev,
                                     string message)
{
    Hello obj = _somNew(somSelf);
    static short i=0;

    __set_msg(obj, ev, message);
    printf("HelloCreate Metaclass:  Hello  %d\n",i++);

    return obj;
}

SOM_Scope void  SOMLINK sayHello(Hello somSelf,  Environment *ev)
{
    printf("%s\n", __get_msg(somSelf, ev));
}

SOM_Scope void  SOMLINK somInit(Hello somSelf)
{
    HelloData *somThis = HelloGetData(somSelf);
    static short i=0;
    HelloMethodDebug("Hello","somInit");
    printf("somInit:  Hello %d\n",i++);
    Hello_parent_SOMObject_somInit(somSelf);
}

SOM_Scope void  SOMLINK somUninit(Hello somSelf)
{
    HelloData *somThis = HelloGetData(somSelf);
    HelloMethodDebug("Hello","somUninit");

    Hello_parent_SOMObject_somUninit(somSelf);
}

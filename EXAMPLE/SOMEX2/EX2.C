
/**********/
/** ex2.c */
/**********/

#define ExampleTwo_Class_Source
#include <ex2.ih>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SOM_Scope void  SOMLINK xSayHi(ExampleTwo somSelf,  Environment *ev)
{
    ExampleTwoData *somThis = ExampleTwoGetData(somSelf);

    printf("--> xSayHi begin\n");
    printf("-->   the attribute 'msg' => %s <=\n",__get_msg(somSelf,ev));
    printf("-->   the attribute 'mylong' => %ld <=\n", _mylong);
}

/*
 *  override the method 'somInit' which I inherit from SOMObject
 */

SOM_Scope void  SOMLINK somInit(ExampleTwo somSelf)
{
    ExampleTwoData *somThis = ExampleTwoGetData(somSelf);
    Environment *ev = somGetGlobalEnvironment();

    ExampleTwoMethodDebug("ExampleTwo","somInit");

    printf("---somInit: Initializing an instance of the ExampleTwo Class...\n");

    /* give the 'msg' parameter a default value here in somInit */
    /* the next two assignments could be made in a number of ways: */
    /*        _msg = "default string"; */
    /*        somThis->msg = "default string"; */
    /*        __set_mylong(somSelf, ev, 33L); */
    /*        somThis->mylong = 33L; */
    __set_msg(somSelf, ev, "default string");
    _mylong = 33L;

    ExampleTwo_parent_SOMObject_somInit(somSelf);
}


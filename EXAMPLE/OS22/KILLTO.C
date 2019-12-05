
/***************************************************/
/** killto.c                                       */
/***************************************************/
/** a program designed to kill the timeout program */
/***************************************************/

#include <stdio.h>

#include <semaphor.h>

string Name1="KillTO";  /* kill the timeout application */

int main(int argc, char *argv[])
{
    Semaphore a;
    Environment *ev = somGetGlobalEnvironment();
    M_Semaphore clsObj = SemaphoreNewClass(0,0);

    a = _CreateEventSem(clsObj, ev, Name1);

    fprintf(stderr,"I am the great timeout killer!!\n", a);
    _PostSemaphore(a, ev);

    _somFree(a);

    return(0);
}


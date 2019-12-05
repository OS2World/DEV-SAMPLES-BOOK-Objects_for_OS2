
/****************/
/** timeout.c ***/
/****************/

#include <stdio.h>

#include <semaphor.h>

#define STACKSIZE  4096
#define TIMEOUT_VALUE 20000

VOID timeoutThread (Semaphore semEvent);
string Name1="KillTO";   /* kill the TimeOut application */

int main(int argc, char *argv[])
{
    Semaphore a;
    short cnt=0;
    Environment *ev = somGetGlobalEnvironment();
    M_Semaphore clsObj = SemaphoreNewClass(0,0);

    a = _CreateEventSem(clsObj, ev, Name1);

    _beginthread ((void (*)(void *))timeoutThread, NULL, STACKSIZE, a);

    printf("Hi.\n");
    DosSleep(1000);  /* pause a second  */
    printf("I'm waiting to be killed.\n");
    while(_WaitForEvent(a, ev, 1000)) {  /* wait 1000 milliseconds */
        printf("%d\n", ++cnt);   /* count while waiting to time out */
    }
    printf("  Aaaaaaa! I'm dead\n");

    _somFree(a);

    return(0);
}

VOID timeoutThread (Semaphore semEvent)
{
    Environment *ev = somGetGlobalEnvironment();
    short i;

    DosSleep(TIMEOUT_VALUE);
    _PostSemaphore(semEvent, ev);
    _endthread();
}

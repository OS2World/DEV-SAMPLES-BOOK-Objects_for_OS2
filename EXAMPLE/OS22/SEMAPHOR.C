
#define Semaphore_Class_Source
#define M_Semaphore_Class_Source

#define INCL_DOSERRORS
#include <semaphor.ih>

#include <stdio.h>

/*
 *
 * WaitForEvent blocks current thread for a period of time
 * controlled by the waitType parameter.  Blocking will stop when
 * an event (associated with somSelf) occurs.
 *
 * waitType's include:
 * -1, or SEM_INDEFINITE_WAIT
 *   Wait indefinitely, blocking thread until event occurs or is posted.
 * 0, or SEM_IMMEDIATE_RETURN
 *   No wait, quick check to see if event has been posted.
 * Any positive value.  A timeout in milliseconds.
 *
 * return values include:
 * 0.  when event indicated in somSelf is posted.
 * ERROR_TIMEOUT.  timeout occurs with no event posted.
 * ERROR_INTERRUPT.
 */

SOM_Scope long  SOMLINK WaitForEvent(Semaphore somSelf,  Environment *ev,
                                     unsigned long waitType)
{
    SemaphoreData *somThis = SemaphoreGetData(somSelf);
    APIRET rc;
    SemaphoreMethodDebug("Semaphore","WaitForEvent");

    return (long)DosWaitEventSem(_hEventSem, waitType);
}

/*
 * for posting a semaphore.  for informing other threads or processing
 * that an event has occured.
 */

SOM_Scope void  SOMLINK PostSemaphore(Semaphore somSelf,  Environment *ev)
{
    SemaphoreData *somThis = SemaphoreGetData(somSelf);
    SemaphoreMethodDebug("Semaphore","PostSemaphore");

    DosPostEventSem(_hEventSem);
}

/*
 * to reset a semaphore.
 * returns the count of posts made before this current reset.
 */

SOM_Scope unsigned long  SOMLINK ResetSemaphore(Semaphore somSelf,  Environment *ev)
{
    SemaphoreData *somThis = SemaphoreGetData(somSelf);
    APIRET rc;
    unsigned long postCount;

    SemaphoreMethodDebug("Semaphore","ResetSemaphore");
    rc = DosResetEventSem(_hEventSem, &postCount);
    return postCount;

}

SOM_Scope void  SOMLINK somInit(Semaphore somSelf)
{
    SemaphoreData *somThis = SemaphoreGetData(somSelf);
    Environment *ev = somGetGlobalEnvironment();
    SemaphoreMethodDebug("Semaphore","somInit");

    Semaphore_parent_GenericFile_somInit(somSelf);

    /* make call to parent first because the GenericFile class */
    /* will initialize the path attribute to NULL */
    /* After that we can set path to our SEM32 value */

    __set_path(somSelf, ev, "\\SEM32\\");
}

SOM_Scope void  SOMLINK somUninit(Semaphore somSelf)
{
    SemaphoreData *somThis = SemaphoreGetData(somSelf);
    SemaphoreMethodDebug("Semaphore","somUninit");

    DosCloseEventSem(_hEventSem);

    Semaphore_parent_GenericFile_somUninit(somSelf);
}

/*
 * returns a named Event Semaphore object
 */

SOM_Scope Semaphore  SOMLINK CreateEventSem(M_Semaphore somSelf,
                                             Environment *ev,
                                            string semName)
{
    /* M_SemaphoreData *somThis = M_SemaphoreGetData(somSelf); */
    SemaphoreData *somThat;
    string semPath;
    char semFullName[256];
    HEV hEvSem;
    APIRET rc;
    BOOL32 fState=0;       /* initialize to "set" */
    ULONG  flAttr=0;       /* flAttr is ignored with named semaphores */
    Semaphore semNew;

    M_SemaphoreMethodDebug("M_Semaphore","CreateEventSem");

    if(!semName) {
        return (Semaphore)NULL;
    }
    semNew = SemaphoreNew();
    somThat = SemaphoreGetData(semNew);
    semPath = __get_path(semNew, ev);
    strcpy(semFullName, semPath);
    __set_name(semNew, ev, semName);

    /* we are not supporting unnamed semaphores in this example so just */
    /* return a NULL if the caller gives a NULL for a semaphore name */

    /* set up the semaphore name using the standard path/name combination */
    /* which is also used for files in the file system. */
    strcpy(semFullName, semPath);
    strcat(semFullName, semName);

/*******  initialize the event semaphore ****/
    flAttr = 0;
    if(rc = DosCreateEventSem(semFullName, &hEvSem, flAttr, fState)) {
       if(rc==ERROR_DUPLICATE_NAME) {
           hEvSem = 0;  /* set to for named DosOpenEventSem */
           rc = DosOpenEventSem(semFullName, &hEvSem);
        /*   fprintf(stderr,"DUPLICATE name, use Open instead of Create now rc=%d\n",rc); */
       } else {
           fprintf(stderr,"DosCreateEventSem error! semName=%s rc=%ld 1 hEvSem=%lx\n",
               semFullName, rc,hEvSem);
           _somFree(semNew);
           return (Semaphore)NULL;
       }
    }
    somThat->hEventSem = hEvSem;

    return semNew;
}


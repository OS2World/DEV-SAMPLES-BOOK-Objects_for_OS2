
#define AsciiSpeak_Class_Source
#include <ascout.ih>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SOM_Scope void  SOMLINK outputAsciiBuffer(AsciiSpeak somSelf,
                              Environment *ev, string ascBuffer)
{
    AsciiSpeakData *somThis = AsciiSpeakGetData(somSelf);
    FILE *fp = (FILE *)_asciiFilePtr;
    AsciiSpeakMethodDebug("AsciiSpeak","outputAsciiBuffer");

    fprintf(fp,"%s\n", ascBuffer);
}

SOM_Scope void  SOMLINK somInit(AsciiSpeak somSelf)
{
    AsciiSpeakData *somThis = AsciiSpeakGetData(somSelf);
    FILE *fp;
    string filename;

    static char tmpStr[256];
    static short defaultFontSize=20, counter=0;

    AsciiSpeakMethodDebug("AsciiSpeak","somInit");

    /* ---------------------------------------------------------------*/
    /* open the file that we will use to store this ascii output */
    /* ---------------------------------------------------------------*/
    sprintf(tmpStr, "ascii%d", counter++);
    filename = (string)malloc(strlen(tmpStr)+1);
    strcpy(filename, tmpStr);
    fp = fopen(filename, "w");
    _asciiFilePtr = (void *)fp;

    AsciiSpeak_parent_SOMObject_somInit(somSelf);
}

SOM_Scope void  SOMLINK somUninit(AsciiSpeak somSelf)
{
    AsciiSpeakData *somThis = AsciiSpeakGetData(somSelf);
    FILE *fp = (FILE *)_asciiFilePtr;
    AsciiSpeakMethodDebug("AsciiSpeak","somUninit");

    fclose(fp);

    AsciiSpeak_parent_SOMObject_somUninit(somSelf);
}


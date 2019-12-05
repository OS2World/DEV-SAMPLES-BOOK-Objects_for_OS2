
#define EAFile_Class_Source
#define M_EAFile_Class_Source

#define INCL_DOS
#include <os2.h>

#include <eafile.ih>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**********************************************************************/
/* Function prototypes.                                               */
/**********************************************************************/
VOID allocMem(PVOID *pvMessage,
              ULONG ulSize);
BOOL readEA(char *path);
VOID GetEAsFromFile(CHAR  *szFilename,
                    DENA2 *eaList,
                    ULONG ulEnumCount);
VOID PrintData(USHORT *pEAData);

/**********************************************************************/
/* Defines.                                                           */
/**********************************************************************/
#define MAX_GEA 500L


/*
 * retrieves a line of text from the file.
 * if lineLength set to -1 will read to the next newline
 */

SOM_Scope string  SOMLINK readLine(EAFile somSelf,  Environment *ev,
                                   long lineLength)
{
    EAFileData *somThis = EAFileGetData(somSelf);
    EAFileMethodDebug("EAFile","readLine");

    return;
}

/*
 * for simple string buffers that are null terminated you
 * could allow for a -1 bufferLength
 */

SOM_Scope void  SOMLINK writeLine(EAFile somSelf,  Environment *ev,
                                  somToken writeBuffer, long bufferLength)
{
    EAFileData *somThis = EAFileGetData(somSelf);
    EAFileMethodDebug("EAFile","writeLine");

}

/*
 * make a listing of all of the extended attributes
 * also return the listing in the form of an array
 * of strings, terminated by a NULL
 */

SOM_Scope void SOMLINK listEAs(EAFile somSelf,  Environment *ev)
{
    EAFileData *somThis = EAFileGetData(somSelf);
    EAFileMethodDebug("EAFile","listEAs");
    readEA(__get_name(somSelf, ev));

    return;
}

SOM_Scope somToken  SOMLINK getFirstEA(EAFile somSelf,  Environment *ev,
                                       string EAName)
{
    EAFileData *somThis = EAFileGetData(somSelf);
    EAFileMethodDebug("EAFile","getFirstEA");

    return;
}

SOM_Scope somToken  SOMLINK getNextEA(EAFile somSelf,  Environment *ev)
{
    EAFileData *somThis = EAFileGetData(somSelf);
    EAFileMethodDebug("EAFile","getNextEA");

    return;
}

SOM_Scope somToken  SOMLINK getEAValue(EAFile somSelf,  Environment *ev,
                                       somToken EAttr)
{
    EAFileData *somThis = EAFileGetData(somSelf);
    EAFileMethodDebug("EAFile","getEAValue");

    return;
}

SOM_Scope void  SOMLINK addExAttr(EAFile somSelf,  Environment *ev,
                                  somToken EAttr, somToken EAttrVal)
{
    EAFileData *somThis = EAFileGetData(somSelf);
    EAFileMethodDebug("EAFile","addExAttr");

}


SOM_Scope void  SOMLINK somInit(EAFile somSelf)
{
    EAFileData *somThis = EAFileGetData(somSelf);
    EAFileMethodDebug("EAFile","somInit");

    EAFile_parent_GenericFile_somInit(somSelf);
}

/*
 * retrieve the first file that matches a pattern indicated in
 *     the string 'lookfor'.  Standard wildcards supported.
 */

/*
 * retrieve the first file that matches a pattern indicated in
 *     the string 'lookfor'.  Standard wildcards supported.
 *     When using wildcards, you can cycle through all the files that
 *     match the pattern, by following up with repeated calls to
 *     getNextFile.
 *     Also used for a single file.
 */

SOM_Scope EAFile  SOMLINK getFirstFile(M_EAFile somSelf,  Environment *ev,
                                       string lookfor)
{
    M_EAFileData *somThis = M_EAFileGetData(somSelf);
    EAFile returnObj;
    APIRET retVal;
    FILEFINDBUF3 buf;
    ULONG srchcnt = 1;
    HDIR hdir=HDIR_SYSTEM;

    /* the next vars are for the EA info management */
    CHAR *pAlloc=NULL;                 /* Buffer for returned EA data.*/
    ULONG ulEntryNum = 1;              /* Current EA counter.         */
    ULONG ulEnumCnt;                   /* Number of EAs to return.    */
    FEA2 *pFEA;                        /* Pointer to returned values. */

    M_EAFileMethodDebug("M_EAFile","getFirstFile");

    if (!(retVal = DosFindFirst(lookfor,
                                &hdir,
                                FILE_NORMAL + FILE_HIDDEN + FILE_SYSTEM + FILE_READONLY,
                                &buf,
                                sizeof(buf),
                                &srchcnt, 1L))) {
        returnObj = _somNew(somSelf);
        __set_name(returnObj, ev, buf.achName);
        _hdir = hdir;
    } else {
        return NULL;
    }

    /******************************************************************/
    /* Allocate enough room for any attribute list for the returned   */
    /* EA buffer and initialize the pFEA to the start of this buffer. */
    /* The pFEA buffer is used to traverse the returned buffer.       */
    /******************************************************************/
    allocMem((PPVOID)&pAlloc, MAX_GEA);
    pFEA = (FEA2 *) pAlloc;
    ulEnumCnt = 1;
    if(DosEnumAttribute(1,
                    buf.achName,
                    ulEntryNum,
                    pAlloc,
                    MAX_GEA,
                    &ulEnumCnt,
                    1L) )   {
          printf("\nERROR\n");
    }

    return returnObj;
}

/*
 * must be used after getFirstFile
 */

SOM_Scope EAFile  SOMLINK getNextFile(M_EAFile somSelf,  Environment *ev)
{
    M_EAFileData *somThis = M_EAFileGetData(somSelf);
    EAFile returnObj;
    APIRET retVal;
    FILEFINDBUF3 buf;
    HDIR hdir = _hdir;
    ULONG srchcnt = 1;
    M_EAFileMethodDebug("M_EAFile","getNextFile");
    if(!(retVal = DosFindNext(hdir,
                                     &buf,
                                     sizeof(buf),
                                     &srchcnt))) {
            returnObj = _somNew(somSelf);
            __set_name(returnObj, ev, buf.achName);
            return returnObj;
    }  else
        return NULL;
}

VOID allocMem (PVOID *ppv, ULONG cb)
{
    BOOL failed;

    failed =(BOOL) DosAllocMem(ppv, cb, fPERM|PAG_COMMIT);
    if (failed) {
        fprintf(stderr,"ERROR: Memory is full\n");
        *ppv = NULL;
        exit(1);
    }
    return;
}

BOOL readEA(CHAR *path)
{
    CHAR *pAlloc=NULL;                 /* Buffer for returned EA data.*/
    ULONG ulEntryNum = 1;              /* Current EA counter.         */
    ULONG ulEnumCnt;                   /* Number of EAs to return.    */
    FEA2 *pFEA;                        /* Pointer to returned values. */

    /******************************************************************/
    /* Allocate enough room for any attribute list for the returned   */
    /* EA buffer and initialize the pFEA to the start of this buffer. */
    /* The pFEA buffer is used to traverse the returned buffer.       */
    /******************************************************************/
    allocMem((PPVOID)&pAlloc, MAX_GEA);
    pFEA = (FEA2 *) pAlloc;

    /******************************************************************/
    /* Loop through all the EAs in the file.  A break is issued when  */
    /* no EAs are left to be processed.                               */
    /******************************************************************/
    for(;;) {
       /***************************************************************/
       /* Use DosEnumAttribute to get a buffer (pAlloc) containing    */
       /* basic EA information about the EA.  The actual data for the */
       /* EA is not returned by this function.  The data is acquired  */
       /* by using DosQueryFileInfo.                                  */
       /***************************************************************/
       ulEnumCnt = 1;
       if(DosEnumAttribute(1,
                           path,
                           ulEntryNum,
                           pAlloc,
                           MAX_GEA,
                           &ulEnumCnt,
                           1L) )   {
          fprintf(stderr,"ERROR on DosEnumAttribute -- path=%s enumCnt=%ld\n",
                          path, ulEnumCnt);
          break;                       /* An error occured.           */
       }

       /***************************************************************/
       /* If the first EA for the file is not available then there    */
       /* are no EAs associated with this file.                       */
       /***************************************************************/
       if (ulEnumCnt != 1) {
          if(ulEntryNum==1) {
              printf("            ::> none\n");
          } else {
              printf("\n");
          }
          break;
       }

       ulEntryNum++;

       /***************************************************************/
       /* Print the attribute name.                                   */
       /***************************************************************/
       if(pFEA->szName)
               printf("          %10s\n", pFEA->szName);

       /***************************************************************/
       /* Open the file, get the attribute value, and then print the  */
       /* EA values information.                                      */
       /***************************************************************/
       GetEAsFromFile(path, (DENA2 *)pAlloc, ulEnumCnt);
    }

    DosFreeMem(pAlloc);                /* Free the buffer.           */
    return (TRUE);
}

VOID GetEAsFromFile(CHAR  *szFileName,
                    DENA2 *eaList,
                    ULONG ulEnumCount)

{
    HFILE       FileHandle;
    ULONG       ulActionTaken;
    ULONG       ulFileAttributes = FILE_READONLY;
    ULONG       ulOpenFlag = OPEN_ACTION_FAIL_IF_NEW  |
                             OPEN_ACTION_OPEN_IF_EXISTS;
    ULONG       ulOpenMode = OPEN_SHARE_DENYREADWRITE |
                             OPEN_ACCESS_READONLY;
    ULONG       ulEACount;
    ULONG       ulEASize;
    EAOP2       eaop2;
    USHORT      *pEAData;
    APIRET      rc = 0;


    /******************************************************************/
    /* Open the file for Extended Attribute access.  The file must    */
    /* be opened for read access with a deny-write sharing mode.      */
    /******************************************************************/
    rc = DosOpen(szFileName,
                 &FileHandle,
                 &ulActionTaken,
                 100L,
                 ulFileAttributes,
                 ulOpenFlag,
                 ulOpenMode,
                 0L);

    if (rc == 0) {
        for (ulEACount=0;ulEACount<ulEnumCount;ulEACount++) {
            /**********************************************************/
            /* Allocate storage for the EAs and setup the GEA2LIST    */
            /* based on the list retrieved from DosEnumAttribute. The */
            /* storage size is based on the following formula :       */
            /*                                                        */
            /*    4 Bytes - for oNextEntryOffset                      */
            /*    1 Bytes - for usFlags                               */
            /*    1 Bytes - for cbName                                */
            /*    2 Bytes - for cbValue                               */
            /*   ---------------------------------                    */
            /*    8 Bytes - Total                                     */
            /*                                                        */
            /*    This value (8) is added to the length of the        */
            /*    attribute name (cbName) plus 1 byte for the NULL    */
            /*    terminator plus the length of the actual attribute  */
            /*    value information (cbValue).                        */
            /*                                                        */
            /* ------------------------------------------------------ */
            /*                                                        */
            /*    8 + cbName + 1 + cbValue                            */
            /*                                                        */
            /**********************************************************/
            if (rc == 0) {
                ulEASize = 8 +
                           (eaList[0].cbName + 1) +
                           eaList[0].cbValue;
                eaop2.fpGEA2List = malloc(ulEASize);
                eaop2.fpFEA2List = malloc(ulEASize);

                if ((eaop2.fpGEA2List != NULL) &&
                    (eaop2.fpFEA2List != NULL)) {
                    eaop2.fpGEA2List->list->oNextEntryOffset = 0;
                    eaop2.fpGEA2List->list->cbName = eaList[0].cbName;
                    strcpy(eaop2.fpGEA2List->list->szName,
                           eaList[0].szName);
                    eaop2.fpGEA2List->cbList = ulEASize;
                } else {
                    rc = -1;
                }
            }

            if (rc == 0) {
                /******************************************************/
                /* Use DosQueryFileInfo to retrieve the Full EA data  */
                /* for the EA specified in fpGEA2List.                */
                /******************************************************/
                eaop2.fpFEA2List->cbList = ulEASize;
                rc = DosQueryFileInfo(FileHandle,
                                      FIL_QUERYEASFROMLIST,
                                      &eaop2,
                                      ulEASize * 2);

                /******************************************************/
                /* If attribute information was found then allocate   */
                /* enough storage to hold the data and copy the data  */
                /* into the new storage.  Pass the EA data to the     */
                /* printing routine.                                  */
                /******************************************************/
                if (rc == 0) {
                    pEAData = malloc(eaop2.fpFEA2List->list->cbValue);

                    if (pEAData != NULL) {
                        memcpy((UCHAR *)pEAData,
                               (PBYTE)eaop2.fpFEA2List +
                               sizeof(FEA2LIST) +
                               eaop2.fpFEA2List->list->cbName,
                               eaop2.fpFEA2List->list->cbValue);
                        PrintData(pEAData);
                        free(pEAData);
                    } else {
                        fprintf(stderr, "- Memory allocation error\n");
                    }
                } else {
                    fprintf(stderr, "- DosQueryFileInfo RC = %d\n",rc);
                }
                free(eaop2.fpGEA2List);
                free(eaop2.fpFEA2List);
                eaList = (DENA2 *)((PBYTE)eaList + eaList[0].oNextEntryOffset);
            } else {
                fprintf(stderr, "- Memory allocation error.\n");
            }
        }
        DosClose(FileHandle);
    } else {
        fprintf(stderr, "- Unable to open file - DosOpen RC = %d.\n",rc);
    }
    return;
}


VOID PrintData(USHORT *pEAData)
{
    CHAR        szPrintBuffer[1024];
    USHORT      usEAType;
    USHORT      usEALen;
    BYTE        bByteValue;
    ULONG       i;

    /******************************************************************/
    /* pEAData points to a section of memory that contains three      */
    /* pieces of information for the EA.  These are EA Type, EA length*/
    /* and EA data for most EAs.  For multiple value EAs, this info   */
    /* is a little different.                                         */
    /*                                                                */
    /* usEAType is stored at the beginning and has a length 2 bytes.  */
    /* usEALen  is follows usEAType and also has 2 bytes.             */
    /* The EA data  follows the usEALen and a length of usEAlen bytes.*/
    /*                                                                */
    /* After storing type and length we move up the pEAData pointer   */
    /* to point to the value itself.  In order to move up the pEAData */
    /* pointer a byte at a time we use the (PBYTE) cast.              */
    /******************************************************************/
    usEAType = (USHORT)*pEAData;
    pEAData = (USHORT *)((PBYTE)pEAData + 2);
    usEALen  = (USHORT)*pEAData;
    pEAData = (USHORT *)((PBYTE)pEAData + 2);

    printf("%4d - ", usEALen);

    /******************************************************************/
    /* Print EA information based on the EA type.                     */
    /******************************************************************/
    switch(usEAType) {
        case EAT_EA        :
        case EAT_ASCII     :
            strncpy(szPrintBuffer,(UCHAR *)pEAData,usEALen);
            szPrintBuffer[usEALen] = '\0';
            printf("\"%s\"\n", szPrintBuffer);
            break;

        case EAT_BINARY    :
            for (i=0;i<usEALen;i++) {
                bByteValue = (BYTE)*pEAData;
                if (bByteValue<=0x0F) {
                    printf("0");
                }
                printf("%x",bByteValue);
                pEAData = (USHORT *)((PBYTE)pEAData + 1);
            }
            printf("\"\n");
            break;

        case EAT_METAFILE  :
            printf("METAFILE Type\n");
            break;

        case EAT_BITMAP    :
        case EAT_ICON      :
            printf("ICON or BITMAP Type\n");
            break;

        case EAT_MVMT     :
        case EAT_MVST     :
            printf("MVMT or MVST multivalue Type\n");
            break;

        default : {
            printf("Unknown listType EA Type: %d\n", usEAType);
        }
    }
    return;
}

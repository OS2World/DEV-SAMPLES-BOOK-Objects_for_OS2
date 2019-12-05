
#define PostScriptSpeak_Class_Source
#include <postscr.ih>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 *Method from the IDL attribute statement:
 *"attribute string currentFont"
 */

/*
 * the font and style in a string
 */

SOM_Scope void  SOMLINK _set_currentFont(PostScriptSpeak somSelf,
                               Environment *ev, string currentFont)
{
    PostScriptSpeakData *somThis = PostScriptSpeakGetData(somSelf);
    FILE *fp = (FILE *)_psFilePtr;

    PostScriptSpeakMethodDebug("PostScriptSpeak","_set_currentFont");

    free(_currentFont);
    _currentFont = (string)malloc(strlen(currentFont)+1);
    strcpy(_currentFont, currentFont);
    fprintf(fp,"/%s choosefont\n", __get_currentFont(somSelf, ev));
}

/*
 * the font's "scale"
 */

SOM_Scope void  SOMLINK _set_fontSize(PostScriptSpeak somSelf,
                                 Environment *ev, long fontSize)
{
    PostScriptSpeakData *somThis = PostScriptSpeakGetData(somSelf);
    FILE *fp = (FILE *)_psFilePtr;
    long oldLineIncrement = _lineIncrement;

    PostScriptSpeakMethodDebug("PostScriptSpeak","_set_fontSize");

    _fontSize = fontSize;
    _lineIncrement = fontSize+4;  /* update this instance variable */

    fprintf(fp,"/fontSize %ld def\n", fontSize);
    fprintf(fp,"/lineIncrement %ld def\n", _lineIncrement);

/*
 * The next two lines correct the lineIncrement from the previous
 *   'newline' directive which was based on the old fontSize
 * This will also act as a correction to the placement given in 'somInit'
 *   for vertpos which is based on a default fontSize.  This second type
 *   of correction is needed if you are setting the fontSize before
 *   your first newline.
 */
    fprintf(fp,"/vertpos vertpos %d sub def\n",
                    _lineIncrement - oldLineIncrement);
    fprintf(fp,"horzpos vertpos moveto\n");
    return;
}

/*
 *Method from the IDL attribute statement:
 *"attribute string currentFont"
 */

/*
 * take a buffer of text and output using PostScript format.
 */

SOM_Scope void  SOMLINK outputPostScriptBuffer(PostScriptSpeak somSelf,
                                  Environment *ev, string psBuffer)
{
    PostScriptSpeakData *somThis = PostScriptSpeakGetData(somSelf);
    FILE *fp = (FILE *)_psFilePtr;

    PostScriptSpeakMethodDebug("PostScriptSpeak","outputPostScriptBuffer");

    fprintf(fp,"(%s) show newline\n", psBuffer);
}

SOM_Scope void  SOMLINK somInit(PostScriptSpeak somSelf)
{
    PostScriptSpeakData *somThis = PostScriptSpeakGetData(somSelf);
    Environment *ev = somGetGlobalEnvironment();
    string filename;
    FILE *fp;
    static char tmpStr[256];

    static short defaultFontSize=20, counter=0;

    PostScriptSpeakMethodDebug("PostScriptSpeak","somInit");

    /* ---------------------------------------------------------------*/
    /* open the file that we will use to store this postscript output */
    /* ---------------------------------------------------------------*/
    sprintf(tmpStr, "ps%d", counter++);
    filename = (string)malloc(strlen(tmpStr)+1);
    strcpy(filename,tmpStr);
    fp = fopen(filename, "w");
    _psFilePtr = (void *)fp;

    /* --------------------------------------------------------------------*/
    /* set the default font and allocate a string buffer to store the name */
    /* --------------------------------------------------------------------*/
    _currentFont = (string)malloc(strlen("Times-Roman")+1);
    strcpy(_currentFont, "Times-Roman");
    _lineIncrement = defaultFontSize+4;


    fprintf(fp,"%% --------- Some useful procedures -------------\n");
    fprintf(fp,"/inch {72 mul} def\n");
    fprintf(fp,"/fontSize %d def\n", defaultFontSize);
    fprintf(fp,"/lineIncrement %d def\n", _lineIncrement);
    fprintf(fp,"/vertpos 10 inch fontSize sub def\n");
    fprintf(fp,"/horzpos 1 inch def\n");
    fprintf(fp,"\n");
    fprintf(fp,"/choosefont {findfont fontSize scalefont setfont} def\n");
    fprintf(fp,"\n");
    fprintf(fp,"/newline\n");
    fprintf(fp,"  {/vertpos vertpos lineIncrement sub def %% decrease vertpos\n");
    fprintf(fp,"  horzpos vertpos moveto } def\n");
    fprintf(fp,"\n");
    fprintf(fp,"%% --------- Beginning of your PS program ----------\n\n");
    fprintf(fp,"%% --------- The next few lines are some default initializations ----\n\n");
    fprintf(fp,"%% -- if the client program set these values for font and initial position \n");
    fprintf(fp,"%% -- then you should remove these lines which will only be redundant ----\n");
    fprintf(fp,"horzpos vertpos moveto\n");
    fprintf(fp,"/%s choosefont\n", _currentFont);
    fprintf(fp,"%% --------- the rest is the result of the client program. --\n\n");
    free(filename);
    PostScriptSpeak_parent_SOMObject_somInit(somSelf);
}

SOM_Scope void  SOMLINK somUninit(PostScriptSpeak somSelf)
{
    PostScriptSpeakData *somThis = PostScriptSpeakGetData(somSelf);
    FILE *fp = (FILE *)_psFilePtr;

    PostScriptSpeakMethodDebug("PostScriptSpeak","somUninit");

    fprintf(fp,"showpage");
    fclose(fp);

    PostScriptSpeak_parent_SOMObject_somUninit(somSelf);
}

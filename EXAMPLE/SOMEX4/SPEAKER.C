
/*************/
/* speaker.c */
/*************/

#define Speaker_Class_Source
#include <speaker.ih>

#include <stdio.h>

SOM_Scope void  SOMLINK speakOut(Speaker somSelf,  Environment *ev,
                                                    string whatToSay)
{
    SpeakerData *somThis = SpeakerGetData(somSelf);
    M_SpeakerData *somThat = M_SpeakerGetData(_Speaker);

    SpeakerMethodDebug("Speaker","speakOut");
    switch(__get_formatOut(somSelf, ev)) {
        case Speaker_ascii:
            _outputAsciiBuffer(somSelf, ev, whatToSay);
            break;
        case Speaker_postscript:
            _outputPostScriptBuffer(somSelf, ev, whatToSay);
            break;
        default:
            fprintf(stderr, "ERROR: formatOut=%ld not supported\n",
                        (long)__get_formatOut(somSelf, ev));
            break;
    }
    somThis->numberTimesSpoke++;
    somThat->numberTimesSpokeAll++;

}

SOM_Scope void  SOMLINK setFontAndSize(Speaker somSelf,  Environment *ev,
                                       string font, long fontSize)
{
    SpeakerData *somThis = SpeakerGetData(somSelf);
    SpeakerMethodDebug("Speaker","setFontAndSize");
    switch(__get_formatOut(somSelf, ev)) {
        case Speaker_ascii:
            /* do nothing.  A flat ascii file will not use this info */
            break;
        case Speaker_postscript:
            __set_fontSize(somSelf, ev, fontSize);  /* set size first */
            __set_currentFont(somSelf, ev, font);   /* set font after size */
            break;
        default:
            printf("ERROR: formatOut=%ld not supported\n", (long)__get_formatOut(somSelf, ev));
            break;
    }

}


SOM_Scope void  SOMLINK somInit(Speaker somSelf)
{
    SpeakerData *somThis = SpeakerGetData(somSelf);
    SpeakerMethodDebug("Speaker","somInit");

    Speaker_parent_PostScriptSpeak_somInit(somSelf);
    Speaker_parent_AsciiSpeak_somInit(somSelf);
}


SOM_Scope Speaker  SOMLINK SpeakerCreate(M_Speaker somSelf,  Environment *ev,
                                         long speakLanguage)
{
    M_SpeakerData *somThis = M_SpeakerGetData(somSelf);
    Speaker returnObj = _somNew(somSelf);
    M_SpeakerMethodDebug("M_Speaker","SpeakerCreate");

    __set_formatOut(returnObj, ev, speakLanguage);
    _numberObjs++;
    return returnObj;
}


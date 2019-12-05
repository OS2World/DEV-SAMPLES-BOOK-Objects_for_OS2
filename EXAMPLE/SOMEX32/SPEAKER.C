/*************/
/* speaker.c */
/*************/

#define Speaker_Class_Source
#include <speaker.ih>

SOM_Scope void  SOMLINK speakToMe(Speaker somSelf,  Environment *ev)
{
    SpeakerData *somThis = SpeakerGetData(somSelf);
    M_SpeakerData *somThat = M_SpeakerGetData(_Speaker);

    SpeakerMethodDebug("Speaker","speakToMe");
    printf("%s\n", __get_whatToSay(somSelf, ev));
    somThis->numberTimesSpoke++;
    somThat->numberTimesSpokeAll++;

}

SOM_Scope Speaker  SOMLINK SpeakerCreate(M_Speaker somSelf,  Environment *ev,
                                     string message)
{
    M_SpeakerData *somThis = M_SpeakerGetData(somSelf);
    Speaker returnObj = _somNew(somSelf);
    M_SpeakerMethodDebug("M_Speaker","SpeakerCreate");

    __set_whatToSay(returnObj, ev, message);
    _numberObjs++;
    return returnObj;
}


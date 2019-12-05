/*************/
/** main.c ***/
/*************/

#include <speaker.h>

int main(int argc, char *argv[])
{
    Speaker a, b, c;
    Environment *ev = somGetGlobalEnvironment();

    M_Speaker clsObj = SpeakerNewClass(0,0);

    a = _SpeakerCreate(clsObj, ev, "I am A");
    b = _SpeakerCreate(_Speaker, ev, "I am B");
    c = _SpeakerCreate(_Speaker, ev, "I am C");

    _speakToMe(a, ev);
    __set_whatToSay(a, ev, "I say whatever you ask me to say");
    _speakToMe(a, ev);
    _speakToMe(b, ev);
    _speakToMe(c, ev);

    printf(" we now have %ld total objects\n",
                __get_numberObjs(_Speaker, ev));
    printf(" total times spoken by all objects was %ld times\n",
                __get_numberTimesSpokeAll(_Speaker, ev));
    printf(" A spoke %ld times, B spoke %ld times and C spoke %ld times\n",
                __get_numberTimesSpoke(a, ev),
                __get_numberTimesSpoke(b, ev),
                __get_numberTimesSpoke(c, ev));

    _somFree(a);
    _somFree(b);
    _somFree(c);

    return(0);
}


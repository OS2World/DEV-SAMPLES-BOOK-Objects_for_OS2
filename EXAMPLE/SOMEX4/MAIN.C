/*************/
/** main.c ***/
/*************/

#include <speaker.h>

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    Speaker a, b, c;
    languageSpoken speakOut = Speaker_ascii;   /* initialize to ascii */
    Environment *ev = somGetGlobalEnvironment();

    M_Speaker clsObj = SpeakerNewClass(0,0);

    a = _SpeakerCreate(clsObj, ev, Speaker_ascii);
    b = _SpeakerCreate(_Speaker, ev, Speaker_ascii);
    c = _SpeakerCreate(_Speaker, ev, Speaker_ascii);

/* speak out in the two ways that we know how to speak */
/* which are enumerated as ascii and postscript */

    for(speakOut = Speaker_ascii; speakOut < Speaker_ascii+2; speakOut++) {
            /* in the header to the section, print out a banner */
            /* using a percent sign which is interpreted as a comment */
            /* by the postscript interpreter */

        printf("\n%%%%%% language = %s %%%%%%\n\n",
                (speakOut==Speaker_ascii?"ASCII":"POSTSCRIPT"));

        /* now let's all the speak the same language or format */
        /* a, b, and c are all multi-lingual */
        /* by virtue of their multiple inheritance */

        __set_formatOut(a, ev, speakOut);
        __set_formatOut(b, ev, speakOut);
        __set_formatOut(c, ev, speakOut);

        _speakOut(a, ev, "Object A speaks out in default font");
        _setFontAndSize(a, ev, "Helvetica-Oblique", 50L);
        _speakOut(a, ev, "Object A can talk big!");
        _setFontAndSize(a, ev, "Times-Roman", 5L);
        _speakOut(a, ev, "or it can talk quite small!");

        _speakOut(b, ev, "B Speaks out");  /* now object b speaks out */
        _speakOut(c, ev, "C Speaks out");  /* and object c speaks as well */
    }

    _somFree(a);
    _somFree(b);
    _somFree(c);

    return(0);
}



/**********/
/* APP1.C */
/**********/

#define App1_Class_Source

#define INCL_DOSPROCESS   /* for the DosBeep call used below */
#include <app1.ih>
#include <pushb.h>

#include <stdio.h>
#include <string.h>
#include <math.h>

#define TOTALKEYS 13
static USHORT frequency_table[TOTALKEYS];
static PushButton PBTable[TOTALKEYS];

SOM_Scope void  SOMLINK somInit(App1 somSelf)
{
    App1Data *somThis = App1GetData(somSelf);
    double freq;
    short i;
    char keyText[2];
    char keySave='B';  /* creating a C scale so initialize to one less than 'C' */
    char thisKey;
    USHORT whiteKey = 0;
    static short x=0,y=10,w=30,h=300;
    short xB=-20, xW=10;
    Environment *ev = somGetGlobalEnvironment();
    BYTE fgRGB, bgRGB;

    fprintf(stderr,"somInit of app1.c: begin\n");
    App1_parent_Client_somInit(somSelf);

    PushButtonNewClass(0,0);
    freq=220;   /* initial frequency  (400 Hz or middle A  */
    keyText[1] = '\0';
    for(i=0;i<TOTALKEYS;i++)  {
        frequency_table[i] = (short)(freq * pow((double)2, (double)i/(double)12.0));
        fprintf(stderr,"--%d freq=%d\n", i+1, frequency_table[i]);
        switch(i) {
            case 0: case 2: case 4: case 5: case 7: case 9: case 11: case 12:
                fgRGB = 0;
                bgRGB = 255;
                if(++keySave>'G')
                    keySave='A';
                keyText[0]=keySave;
                x = xW;
                y = 10;
                w = 70;
                h = 100;
                xW+=75;
                xB+=75;  /* black keys always follow white keys. reverse not true */
                break;
            case 1: case 3: case 6: case 8: case 10:
                keyText[0]='\0';   /* no text needed on the black keys */
                fgRGB = 255;
                bgRGB = 0;
                x = xB;
                y = 110;
                w = 55;
                h = 200;
                break;
        }
        PBTable[i] = _CreateWindow(_PushButton, ev,
                    somSelf,      /* parent */
                    somSelf,      /* owner */
                    WS_VISIBLE, keyText,
                    x, y, w, h, NULL);
        _SetColorFG(PBTable[i], ev, fgRGB, fgRGB, fgRGB);        /* black foreground */
        _SetColorBG(PBTable[i], ev, bgRGB, bgRGB, bgRGB);  /* on white background */
        __set_userData(PBTable[i], ev, (somToken)i);
    }
}

SOM_Scope void  SOMLINK PushedButton(App1 somSelf, Environment *ev, Window sourceWindow)
{
    App1Data *somThis = App1GetData(somSelf);
    char strBuf[20];

    _timesPressed++;
    sprintf(strBuf, "Pressed %d times", _timesPressed);
    __set_text(somSelf, ev, strBuf);  /* set the text on the title bar */
    DosBeep(frequency_table[(short)__get_userData(sourceWindow, ev)], 250);
}


SOM_Scope void  SOMLINK CharPress(App1 somSelf, Environment *ev,
                char key, short kbmState)
{
    App1Data *somThis = App1GetData(somSelf);
    unsigned short i;
    static unsigned short keynum=0;

    switch(key) {
        case 'c':
            keynum = 0;
            break;
        case 'C': /* use capital C for the top of our octave */
            keynum = 12;
            break;
        case 'D': case 'd':
            keynum = 2;
            break;
        case 'E': case 'e':
            keynum = 4;
            break;
        case 'F': case 'f':
            keynum = 5;
            break;
        case 'G': case 'g':
            keynum = 7;
            break;
        case 'A': case 'a':
            keynum = 9;
            break;
        case 'B': case 'b':
            keynum = 11;
            break;
        default:
            if(keynum<(TOTALKEYS-1))
                keynum++;
            else
                keynum=0;
            break;
    }
    _DoPress(PBTable[keynum], ev);
}

void SOMLINK app1Init(SOMClass *cls)
{
}

/*************/
/* GRAPHIC.C */
/*************/

#define Graphic_Class_Source

#include <graphic.ih>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void InitGraphic(GraphicData *somThis);
void QuitGraphic(GraphicData *somThis);

/*
 * clears current window using current background color
 */

SOM_Scope void  SOMLINK grClearWindow(Graphic somSelf,  Environment *ev)
{
    GraphicData *somThis = GraphicGetData(somSelf);
    POINTL upRightPt;
    RECTL rcl;
    HWND hwndGraphic = __get_hwndWindow(_grWin, ev);
    GraphicMethodDebug("Graphic","grClearWindow");

    _currentX = 0;
    _currentY = 0;
    InitGraphic(somThis);
    WinQueryWindowRect(hwndGraphic, &rcl);
    upRightPt.x = rcl.xRight;
    upRightPt.y = rcl.yTop;

    GpiSetColor(_hps, _bgColor);  /* the box drawing on next line will have effect of filling with our background */
    GpiBox(_hps, DRO_FILL, &upRightPt, 0L, 0L);

    QuitGraphic(somThis);

}

/*
 * draws line from (x1,y1) to (x2,y2)
 */

SOM_Scope void  SOMLINK grDrawLineAt(Graphic somSelf,  Environment *ev,
                                   short x1, short y1, short x2, short y2)
{
    GraphicData *somThis = GraphicGetData(somSelf);
    POINTL  upRightPt;
    GraphicMethodDebug("Graphic","grDrawLineAt");

    _currentX = x1;
    _currentY = y1;
    InitGraphic(somThis);
    upRightPt.x=(long)x2;
    upRightPt.y =(long)y2;

    GpiLine(_hps, &upRightPt);

    QuitGraphic(somThis);

}

/*
 * draws a box from (x1,y1) to (x2,y2)
 */

SOM_Scope void  SOMLINK grDrawBoxAt(Graphic somSelf,  Environment *ev,
                                  short x1, short y1,
                                  short x2, short y2)
{
    GraphicData *somThis = GraphicGetData(somSelf);
    POINTL  upRightPt;

    GraphicMethodDebug("Graphic","grDrawBoxAt");

    _currentX = x1;
    _currentY = y1;
    InitGraphic(somThis);
    upRightPt.x=(long)x2;
    upRightPt.y =(long)y2;
    GpiBox(_hps, _drawControl, &upRightPt, 0L, 0L);
    QuitGraphic(somThis);
}


/*
 * draws textStr at point (x,y)
 */

SOM_Scope void  SOMLINK grDrawStringAt(Graphic somSelf,  Environment *ev,
                        short x, short y, string textStr)
{
    GraphicData *somThis = GraphicGetData(somSelf);
    GraphicMethodDebug("Graphic","grDrawStringAt");
    _currentX = x;
    _currentY = y;
    InitGraphic(somThis);
    GpiCharString(_hps, strlen(textStr), textStr);
    QuitGraphic(somThis);

}

SOM_Scope void  SOMLINK somInit(Graphic somSelf)
{
    GraphicData *somThis = GraphicGetData(somSelf);
    GraphicMethodDebug("Graphic","somInit");
    _currentX = 0;
    _currentY = 0;
    _bgColor = CLR_BLACK;
    _fgColor = CLR_YELLOW;
    _fgMixMode = FM_DEFAULT;
    _drawControl = DRO_OUTLINE;

    Graphic_parent_SOMObject_somInit(somSelf);
}

SOM_Scope void  SOMLINK somUninit(Graphic somSelf)
{
    GraphicData *somThis = GraphicGetData(somSelf);
    GraphicMethodDebug("Graphic","somUninit");

    Graphic_parent_SOMObject_somUninit(somSelf);
}

void InitGraphic(GraphicData *somThis)
{
    Environment *ev = somGetGlobalEnvironment();
    POINTL pt;
    HWND hwndGraphic = __get_hwndWindow(_grWin, ev);

    pt.x = _currentX;
    pt.y = _currentY;
    _hps = WinGetPS(hwndGraphic);
    GpiMove(_hps, &pt);
    GpiSetColor(_hps, _fgColor);
    GpiSetBackColor(_hps, _bgColor);
    GpiSetMix (_hps, _fgMixMode);
}

void QuitGraphic(GraphicData *somThis)
{
    WinReleasePS(_hps);
}



/*************/
/* BOXDRAW.C */
/* a client program that is good at drawing boxes */
/*************/

#define Boxdraw_Class_Source

#include <graphic.h>  /* this picks up pmgpi.h (window.h won't) */
#include <boxdraw.ih>

#include <stdio.h>


SOM_Scope void  SOMLINK CharPress(Boxdraw somSelf, Environment *ev,
                char key, short kbmState)
{
    BoxdrawData *somThis = BoxdrawGetData(somSelf);
    Graphic gr = __get_gr(somSelf, ev);
    long saveDrawMode;

    if(key=='f' || key =='F') {
        saveDrawMode = __get_drawControl(gr, ev);
        __set_drawControl(gr, ev, DRO_FILL);
        _grDrawBoxAt(gr, ev, _anchorX, _anchorY, _lastX, _lastY);
        __set_drawControl(gr, ev, saveDrawMode);
    }
}

SOM_Scope void  SOMLINK Button1Click(Boxdraw somSelf, Environment *ev,
                short x, short y)
{
    BoxdrawData *somThis = BoxdrawGetData(somSelf);
    Graphic gr = __get_gr(somSelf, ev);

    if(_wireExists)
        _grDrawBoxAt(gr, ev, _anchorX, _anchorY, _lastX, _lastY);
    _wireExists = FALSE;
}

SOM_Scope void  SOMLINK Button1DragBegin(Boxdraw somSelf, Environment *ev,
                short x, short y)
{
    BoxdrawData *somThis = BoxdrawGetData(somSelf);
    Graphic gr = __get_gr(somSelf, ev);

    if(_wireExists)
        _grDrawBoxAt(gr, ev, _anchorX, _anchorY, _lastX, _lastY);

    _lastX = x;
    _lastY = y;
    __set_currentX(gr, ev, (long)x);
    __set_currentY(gr, ev, (long)y);
    _anchorX = x;
    _anchorY = y;
}

SOM_Scope void  SOMLINK Button1Drag(Boxdraw somSelf, Environment *ev,
                short x, short y)
{
    BoxdrawData *somThis = BoxdrawGetData(somSelf);
    Graphic gr = __get_gr(somSelf, ev);

  /* erase the old line */
    if(_anchorX!=_lastX || _anchorY!=_lastY) {
        _grDrawBoxAt(gr, ev, _anchorX, _anchorY, _lastX, _lastY);
        _wireExists = TRUE;
    }

    _lastX = x;
    _lastY = y;

  /* and draw the new one */
    _grDrawBoxAt(gr, ev, (long)_anchorX, (long)_anchorY, (long)_lastX, (long)_lastY);

}

SOM_Scope void  SOMLINK PaintWindow(Boxdraw somSelf, Environment *ev,
                RECTL* rcl)
{
    BoxdrawData *somThis = BoxdrawGetData(somSelf);
    Graphic gr = __get_gr(somSelf, ev);
    long saveMixMode = __get_fgMixMode(gr,ev);

    __set_fgMixMode(gr, ev, FM_DEFAULT);
    _grClearWindow(gr, ev);
    __set_fgMixMode(gr, ev, saveMixMode);
    _grDrawBoxAt(gr, ev, rcl->xLeft,  rcl->yBottom,
                        rcl->xRight, rcl->yTop);
    if(_wireExists)
        _grDrawBoxAt(gr, ev, _anchorX, _anchorY, _lastX, _lastY);
    _grDrawStringAt(gr, ev, 10, 30, "Draw a wire rectangle by dragging mouse button 1");
    _grDrawStringAt(gr, ev, 10, 10, "Press F to fill wire rectangle");
}

SOM_Scope void  SOMLINK somInit(Boxdraw somSelf)
{
    BoxdrawData *somThis = BoxdrawGetData(somSelf);
    Environment *ev;

    Boxdraw_parent_Client_somInit(somSelf);
    __set_fgMixMode(__get_gr(somSelf, ev), ev, FM_INVERT);  /* foreground mixmode */
    _wireExists = FALSE;

}

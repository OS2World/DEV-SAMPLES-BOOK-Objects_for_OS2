
#define INCL_GPI
#define INCL_WIN
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


MRESULT EXPENTRY ClientWndProc (HWND, ULONG, MPARAM, MPARAM);

HAB   hab;
HWND hwndPushbutton1, hwndPushbutton2;  /* push button window handles */
#define clipIn  11       /* push button id's */
#define clipOut 12

int main (void)
{
   static CHAR szClientClass[]="clipTest";
   static ULONG flFrameFlags = FCF_TITLEBAR     | FCF_SYSMENU  |
                               FCF_SIZEBORDER   | FCF_MINMAX   |
                               FCF_SHELLPOSITION | FCF_TASKLIST;
   HMQ   hmq;
   HWND  hwndFrame, hwndClient;
   QMSG  qmsg;
   RECTL rcl;

   hab = WinInitialize(0);
   hmq = WinCreateMsgQueue (hab, 0);
   WinRegisterClass(
                  hab,           /* Anchor block handle */
                  szClientClass, /* Name of class being registered */
                  ClientWndProc, /* Window procedure for class */
                  CS_SIZEREDRAW, /* Class style */
                  0);
   hwndFrame = WinCreateStdWindow (
                  HWND_DESKTOP,        /* Parent window handle */
                  WS_VISIBLE,          /* Style of frame window */
                  &flFrameFlags,       /* Pointer to control data */
                  szClientClass,       /* Client window class name */
                  NULL,                /* Title bar text */
                  0L,                  /* Style of client window */
                  (HMODULE)NULL,       /* Module handle for resources */
                  0,                   /* ID of resources */
                  &hwndClient);        /* Pointer to client window handle */

    WinQueryWindowRect(hwndClient, &rcl);

    hwndPushbutton1 =WinCreateWindow(
                   hwndClient,
                   WC_BUTTON,             /* window class */
                   "Clip In",  /* window text */
                   WS_VISIBLE | BS_PUSHBUTTON,  /* window style */
                   10L, 10L, 80L, 40L,   /* position and size */
                   hwndClient,          /* owner window */
                   HWND_TOP,            /* placement */
                   clipIn,               /* push button 1 id */
                   NULL,               /* Control data */
                   NULL);              /* pres. params */

    hwndPushbutton2 =WinCreateWindow(
                   hwndClient,
                   WC_BUTTON,             /* window class */
                   "Clip Out",  /* window text */
                   WS_VISIBLE | BS_PUSHBUTTON,  /* window style */
                   10L, 60L, 80L, 40L,   /* position and size */
                   hwndClient,          /* owner window */
                   HWND_TOP,            /* placement */
                   clipOut,               /* push button 1 id */
                   NULL,               /* Control data */
                   NULL);              /* pres. params */

    fprintf(stderr,"push buttons created: hwndPushbutton1=%lx hwndPushbutton2=%lx\n",
                    hwndPushbutton1, hwndPushbutton2);
    fprintf(stderr,"client region dimensions are posn: %ld,%ld wXh: %ldX%ld\n",
                    rcl.xLeft, rcl.yBottom, rcl.xRight, rcl.yTop);

   WinSendMsg (hwndFrame, (ULONG)WM_SETICON,
               (MPARAM)WinQuerySysPointer(HWND_DESKTOP, SPTR_APPICON, FALSE),
               (MPARAM)0L);

   while (WinGetMsg (hab, &qmsg, NULLHANDLE, 0, 0))
               WinDispatchMsg (hab, &qmsg);

   WinDestroyWindow (hwndFrame);
   WinDestroyMsgQueue (hmq);
   WinTerminate (hab);
   return 0;
}

MRESULT EXPENTRY ClientWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
    static  HPS      hps;
    static BOOL button1motion=FALSE, button2motion=FALSE;
    static BOOL rectExists=FALSE, segExists=FALSE;
    static POINTL anchor, anchor2;
    static POINTL mousePos, mousePos2;
    SHORT  xMouse=SHORT1FROMMP(mp1), yMouse=SHORT2FROMMP(mp1);
    RECTL rcl;
    USHORT src;
    USHORT viewID;
    LONG fg,bg;

    switch(msg)  {

        case WM_BUTTON2DOWN:
            return 0;
        case WM_BUTTON2UP:
            return 0;


        case WM_COMMAND:
            src = SHORT1FROMMP(mp2);  /* source of WM_COMMAND, should be ..._PUSHBUTTON */
            viewID = SHORT1FROMMP(mp1);
            fprintf(stderr,"WM_COMMAND received mp's %lx %lx ID=%d\n", mp1, mp2, viewID);
            switch(src) {
                case CMDSRC_PUSHBUTTON:
                    switch(viewID) {
                        case clipIn:
                            fprintf(stderr,"clip in button pressed\n");
                            break;
                        case clipOut:
                            fprintf(stderr,"clip out button pressed\n");
                            break;
                    }
                    break;
                default:
                    fprintf(stderr,"  pushb.c ERROR: shouldn't have executed pbSubProc\n");
                    break;
            }
            return 0;
        case WM_BUTTON1DOWN:
            fprintf(stderr, "WM_BUTTON1DOWN x,y->%d %d\n",xMouse,yMouse);
            break;
        case WM_BUTTON1UP:
            fprintf(stderr, "WM_BUTTON1UP x,y->%d %d\n",xMouse,yMouse);
            break;
        case WM_BUTTON1CLICK:
            if(rectExists) {
                hps = WinGetPS(hwnd);
                GpiSetMix (hps, FM_INVERT);
                GpiMove(hps, &anchor);
                GpiBox(hps, DRO_OUTLINE, &mousePos, 0L, 0L);
                WinReleasePS(hps);
            }
            rectExists=FALSE;
            break;

        case WM_BUTTON1MOTIONSTART:
            button1motion=TRUE;
            fprintf(stderr, "WM_BUTTON1MOTIONSTART x,y->%d %d\n",xMouse,yMouse);
            if(rectExists) {
                hps = WinGetPS(hwnd);
                GpiSetMix (hps, FM_INVERT);
                GpiMove(hps, &anchor);
                GpiBox(hps, DRO_OUTLINE, &mousePos, 0L, 0L);
                WinReleasePS(hps);
            }
            anchor.x = xMouse;
            anchor.y = yMouse;
            mousePos.x = xMouse;
            mousePos.y = yMouse;
            break;
        case WM_BUTTON1MOTIONEND:
            button1motion=FALSE;
            fprintf(stderr, "WM_BUTTON1MOTIONEND x,y->%d %d\n",xMouse,yMouse);
            break;
        case WM_BUTTON2MOTIONSTART:
            button2motion=TRUE;
            fprintf(stderr, "WM_BUTTON1MOTIONSTART x,y->%d %d\n",xMouse,yMouse);
            if(segExists) {
                hps = WinGetPS(hwnd);
                GpiSetMix (hps, FM_INVERT);
                GpiMove(hps, &anchor2);
               /* GpiBox(hps, DRO_OUTLINE, &mousePos2, 0L, 0L); */
                GpiLine(hps, &mousePos2);
                WinReleasePS(hps);
            }
            anchor2.x = xMouse;
            anchor2.y = yMouse;
            mousePos2.x = xMouse;
            mousePos2.y = yMouse;
            break;
        case WM_BUTTON2MOTIONEND:
            button2motion=FALSE;
            fprintf(stderr, "WM_BUTTON1MOTIONEND x,y->%d %d\n",xMouse,yMouse);
            break;
        case WM_MOUSEMOVE:
            if(button1motion) {                                
/*                fprintf(stderr, "WM_MOUSEMOVE dragging\n"); */
                rectExists=TRUE;
                hps = WinGetPS(hwnd);
                GpiSetMix (hps, FM_INVERT);
                GpiMove(hps, &anchor);
                GpiBox(hps, DRO_OUTLINE, &mousePos, 0L, 0L);

                mousePos.x = xMouse;
                mousePos.y = yMouse;
                GpiBox(hps, DRO_OUTLINE, &mousePos, 0L, 0L);

                WinReleasePS(hps);
            }
            if(button2motion) {
/*                fprintf(stderr, "WM_MOUSEMOVE dragging\n"); */
                segExists=TRUE;
                hps = WinGetPS(hwnd);
                GpiSetMix (hps, FM_INVERT);
                GpiMove(hps, &anchor2);
                GpiLine(hps, &mousePos2);
                mousePos2.x = xMouse;
                mousePos2.y = yMouse;
                GpiMove(hps, &anchor2);
                GpiLine(hps, &mousePos2);

                WinReleasePS(hps);
            }
            break;

        case WM_PAINT:
            hps = WinBeginPaint (hwnd, NULLHANDLE, &rcl);
            fg=CLR_RED;
            bg=CLR_WHITE;
                WinDrawText (hps, -1, "Clip Test", &rcl, fg, bg, DT_CENTER | DT_VCENTER | DT_ERASERECT);
                GpiSetMix (hps, FM_INVERT);
                GpiMove(hps, &anchor);
                GpiBox(hps, DRO_OUTLINE, &mousePos, 0L, 0L);
                GpiMove(hps, &anchor2);
                GpiBox(hps, DRO_OUTLINE, &mousePos2, 0L, 0L);
            WinEndPaint (hps);
            return 0;
     }
     return WinDefWindowProc (hwnd, msg, mp1, mp2);
}


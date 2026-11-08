/* include/sly2/widget.h -- WIDGET (class 126, 0x110 bytes), the HUD element: its fields and virtual layout. Included
 * by sly2/classes.h at WIDGET's place in the registry classes' emission order (its subclasses stay there); files that
 * cannot include classes.h (sw.cpp: a TU-private GRABSPOT) include this header alone. The widget's records (sprite
 * groups, the deferred load record, the state values) are in sly2/hud.h. */
#ifndef SLY2_WIDGET_H
#define SLY2_WIDGET_H
#include "sly2/basic.h"
#include "sly2/lo.h"

/* 126 WIDGET (0x110): a HUD element (Sly 1's BLOT): a level object with a show/hide state machine, sprite groups
   and argument texts (engine/presentation/hud.cpp; docs/systems/ui-and-hud.md); vtable 0x2DAF30, 50 slots.
   Fields from hud.cpp's per-function views (names in brackets: relay.cpp's Lilly-port view); the sprite types are
   in sly2/hud.h. */
struct SPRITEDESC;   /* a sprite descriptor (0x40 bytes; LoadSpriteDescriptor) */
struct SPRITEGRP;    /* a sprite group (0x30 bytes; sly2/hud.h) */
struct SPRITEXF;     /* a 2-D transform override (0x20 bytes: a 2x3 matrix) */
struct WIDGETLOAD;   /* the deferred sprite-file load record (0x50 bytes; sly2/hud.h) */
class HEAP;
struct WIDGET : LO
{
    int spriteCount;             /* +0x50 entries of the sprite descriptor table (LoadSpriteTable) */
    SPRITEDESC *sprites;         /* +0x54 the sprite descriptor table */
    int state;                   /* +0x58 WIDGET_STATE: 0 unloaded, 1 hidden, 2 visible, 3 appearing, 4 disappearing [nState] */
    float stateTime;             /* +0x5C g_clock time of the last state change [tState] */
    int stateFrame;              /* +0x60 frame count of the last state change [cframeState] */
    int request;                 /* +0x64 WIDGET_REQUEST: 1 hide, 2 show [nStateStamped] */
    float requestTime;           /* +0x68 real time of the last request change [tStateStamped] */
    char unknown6C;              /* +0x6C */
    uchar isShownOnLoad;         /* +0x6D PostWidgetLoad shows the widget (a load option, inferred) */
    char pad6E[2];
    int loopAnimId;              /* +0x70 label played while visible [idLabelHold] */
    int showAnimId;              /* +0x74 appear label, -1 none [idLabelIn] */
    int hideAnimId;              /* +0x78 disappear label, -1 none [idLabelOut] */
    int mainAnimId;              /* +0x7C label on the main track, -1 none [idLabelPlaying] */
    ushort numSpriteGroups;      /* +0x80 [cclip] */
    SPRITEGRP *spriteGroups;     /* +0x84 group 0 is the main sprite [aclip] */
    float mainAnimTime;          /* +0x88 time on the main track */
    float visibleTimeout;        /* +0x8C UpdateWidget hides a visible widget after this long (0: never); game.cpp
                                    ShowCountdownHud clears it */
    float duration;              /* +0x90 LoadWidgetFromBrx */
    int width;                   /* +0x94 size in 640-wide screen units */
    int height;                  /* +0x98 */
    float screenX;               /* +0x9C top-left corner in GS coordinates (PostWidgetLoad centres the widget) */
    float screenY;               /* +0xA0 */
    int priority;                /* +0xA4 draw priority; >= 16 counts in g_sw.numShownWidgets [nPriority] */
    int isNonExclusive;          /* +0xA8 0 while on g_sw.dlWidgetExclusive (SetWidgetExclusive; -1 after load) [fHidden] */
    int isExclusive;             /* +0xAC joins the exclusive list while its main animation plays */
    int pausesGame;              /* +0xB0 stops the clock (rate slot 7) while visible [fFreezeClock] */
    DLE exclusiveLink;           /* +0xB4 link in g_sw.dlWidgetExclusive [dleDraw] */
    int hasFont;                 /* +0xBC PostWidgetLoad finds g_sw.ploHudFont */
    WIDGETLOAD *deferredLoad;    /* +0xC0 streamed widgets: the load record on g_sw.slWidgetDeferred */
    HEAP *spriteHeap;            /* +0xC4 the heap the sprites come from (the streamed-sprite or the level heap) */
    WIDGET *ownerWidget;         /* +0xC8 the widget whose position the sprite texts use (itself) */
    int argTextIds[10];          /* +0xCC argument text ids (ShowWidgetArgs; -1 none) */
    LO *followObject;            /* +0xF4 world object the widget follows (tu_201D78's recon target; inferred) */
    SPRITEXF *xform;             /* +0xF8 2-D transform override (SetWidgetXf) */
    SL renderQueue;              /* +0xFC sprite items queued for this frame's render (link at +0x68) */
    char pad108[0x110 - 0x108];
    WIDGET() __asm__("WIDGET_ctor");          /* out of line: presentation/hud/WIDGET_ctor.c */
    virtual void RenderSelf() __asm__("RenderWidget");   /* slot 19; key function (hud.cpp) */
    virtual CLASSINFO *GetClassInfo() __asm__("GetWidgetClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadWidgetFromBrx");   /* slot 14 */
    virtual void PostLoad() __asm__("PostWidgetLoad");   /* slot 15 */
    virtual void Update() __asm__("UpdateWidget");   /* slot 16 */
    virtual void Show(int idLabel) __asm__("ShowWidget");   /* slot 49: shows the widget with an animation label (-1 default; game.cpp ShowCountdownHud, letterbox, relay AddRelayWidgetShow) */
    virtual void Hide() __asm__("HideWidget");   /* slot 50 */
};
typedef char assert_WIDGET_size[sizeof(WIDGET) == 0x110 ? 1 : -1];
typedef char assert_WIDGET_state[SLY2_OFFSETOF(WIDGET, state) == 0x58 ? 1 : -1];
typedef char assert_WIDGET_isShownOnLoad[SLY2_OFFSETOF(WIDGET, isShownOnLoad) == 0x6D ? 1 : -1];
typedef char assert_WIDGET_spriteGroups[SLY2_OFFSETOF(WIDGET, spriteGroups) == 0x84 ? 1 : -1];
typedef char assert_WIDGET_priority[SLY2_OFFSETOF(WIDGET, priority) == 0xA4 ? 1 : -1];
typedef char assert_WIDGET_exclusiveLink[SLY2_OFFSETOF(WIDGET, exclusiveLink) == 0xB4 ? 1 : -1];
typedef char assert_WIDGET_argTextIds[SLY2_OFFSETOF(WIDGET, argTextIds) == 0xCC ? 1 : -1];
typedef char assert_WIDGET_renderQueue[SLY2_OFFSETOF(WIDGET, renderQueue) == 0xFC ? 1 : -1];

#endif /* SLY2_WIDGET_H */

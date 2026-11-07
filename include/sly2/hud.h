/* include/sly2/hud.h -- the HUD widget's own records (engine/presentation/hud.cpp; docs/systems/ui-and-hud.md):
 * the widget state/request values, the sprite groups a widget plays, and the deferred sprite-file load record.
 * The WIDGET class is in sly2/widget.h, its subclasses in sly2/classes.h (their vtables and class_registry's factories
 * depend on the class order there); widget.h only forward-declares the types below. */
#ifndef SLY2_HUD_H
#define SLY2_HUD_H
#include "sly2/sw.h"
#include "sly2/widget.h"

/* WIDGET::state values. */
enum WIDGET_STATE
{
    WIDGET_STATE_UNLOADED = 0,
    WIDGET_STATE_HIDDEN = 1,
    WIDGET_STATE_VISIBLE = 2,
    WIDGET_STATE_APPEARING = 3,
    WIDGET_STATE_DISAPPEARING = 4,
};
/* WIDGET::request values. */
enum WIDGET_REQUEST
{
    WIDGET_REQUEST_NONE = 0,
    WIDGET_REQUEST_HIDE = 1,
    WIDGET_REQUEST_SHOW = 2,
};

/* A sprite group's animation (0x18 bytes; LoadSpriteGroups): its keys, length and label id. */
struct SPRITEANIM
{
    ushort numKeys;              /* +0x00 */
    ushort pad2;
    void *keys;                  /* +0x04 the 0x20-byte keys */
    float duration;              /* +0x08 */
    WIDGET *owner;               /* +0x0C */
    SPRITEGRP *group;            /* +0x10 */
    int id;                      /* +0x14 the label (0x30F appear, 0x310 loop, 0x311 disappear, ...) */
};

/* A 2-D transform override (a 2x3 matrix, 0x20 bytes: WIDGET::xform, SPRITEGRP::xform, the sprite items' poses). */
struct SPRITEXF { float a[6]; } __attribute__((aligned(16)));

/* A sprite descriptor (0x40 bytes; WIDGET::sprites, LoadSpriteDescriptor): its id, the SPRITE_KIND, the widget, and
   the kind's payload from +0x10 (only its first word is named here: the nested group of a NESTED sprite, the texture
   resource id of an EFFECT; LoadSpriteDescriptor's view has the whole union). */
struct SPRITEDESC
{
    int id;                      /* +0x00 */
    int kind;                    /* +0x04 SPRITE_KIND */
    WIDGET *owner;               /* +0x08 */
    int unknownC;                /* +0x0C */
    int index;                   /* +0x10 NESTED: the sprite group; EFFECT: the texture resource id */
    void *texture;               /* +0x14 EFFECT: its texture reference (a render-target record, resource at +0x20) */
    char pad18[0x40 - 0x18];
};

/* A sprite item (0x70 bytes): one sprite placed by a slot's current key: its colours, 2-D pose, descriptor, the slot
   of the nested group it plays, an argument, and the link of the widget's render queue. */
struct SPRITEITEM
{
    RGBA color0;                 /* +0x00 */
    RGBA color1;                 /* +0x04 */
    char pad8[8];
    SPRITEXF matrix;             /* +0x10 */
    SPRITEDESC *descriptor;      /* +0x30 */
    char pad34[0x58 - 0x34];
    int spriteSlot;              /* +0x58 NESTED: the slot of the nested group */
    char pad5C[0x61 - 0x5C];
    uchar drawFlags;             /* +0x61 QueueSpriteItem's flags (RenderWidget's channel) */
    char pad62[0x64 - 0x62];
    int arg;                     /* +0x64 */
    SLE link;                    /* +0x68 link in WIDGET::renderQueue */
    int unknown6C;               /* +0x6C */
};
typedef char assert_SPRITEITEM_size[sizeof(SPRITEITEM) == 0x70 ? 1 : -1];

/* A sprite group's playback slot (0x18 bytes): its sprite items, the animation playing (-1 none), the key, the time,
   whether it finished (or holds a seeked time), its sound. */
struct SPRITESLOT
{
    int numItems;                /* +0x00 */
    SPRITEITEM *items;           /* +0x04 */
    short animIndex;             /* +0x08 -1: free */
    short keyIndex;              /* +0x0A */
    float time;                  /* +0x0C */
    int isFinished;              /* +0x10 */
    void *sound;                 /* +0x14 */
};

/* A sprite group (0x30 bytes; WIDGET::spriteGroups, group 0 the main sprite) [WIDGETCLIP]. */
struct SPRITEGRP
{
    ushort numAnims;             /* +0x00 */
    ushort numSlots;             /* +0x02 */
    SPRITEANIM *anims;           /* +0x04 */
    SPRITESLOT *slots;           /* +0x08 slot 0 plays the group's main track */
    char padC[0x20 - 0xC];
    int currentAnim;             /* +0x20 animation to start (-1 none; PlayWidgetAnim, StopWidgetAnim) */
    WIDGET *owner;               /* +0x24 */
    int isEnabled;               /* +0x28 set for group 0 */
    SPRITEXF *xform;             /* +0x2C 2-D transform override (SetWidgetAnimXf) */
};
typedef char assert_SPRITEGRP_size[sizeof(SPRITEGRP) == 0x30 ? 1 : -1];

/* The deferred sprite-file load record of a streamed widget (0x50 bytes; on g_sw.slWidgetDeferred): the file record
   read from the level (its key words aw[0], aw[1], aw[13], aw[15] give the file key), the widget, the sprite-heap
   marks of the load. */
struct WIDGETLOAD
{
    FILEREC file;                /* +0x00 */
    WIDGET *owner;               /* +0x40 */
    int priority;                /* +0x44 sprite-heap mark before the load */
    int lastPriority;            /* +0x48 sprite-heap mark after the load */
    WIDGETLOAD *next;            /* +0x4C link in g_sw.slWidgetDeferred */
};
typedef char assert_WIDGETLOAD_size[sizeof(WIDGETLOAD) == 0x50 ? 1 : -1];

#endif /* SLY2_HUD_H */

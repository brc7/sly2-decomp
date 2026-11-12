/* include/sly2/hud.h -- the HUD widget's own records (engine/presentation/hud.cpp; docs/systems/ui-and-hud.md):
 * the widget state/request values, the sprite groups a widget plays, and the deferred sprite-file load record.
 * The WIDGET class is in sly2/widget.h, its subclasses in sly2/classes.h (their vtables and class_registry's factories
 * depend on the class order there); widget.h only forward-declares the types below. */
#ifndef SLY2_HUD_H
#define SLY2_HUD_H
#include "sly2/sw.h"
#include "sly2/widget.h"
#include "sly2/port.h"   /* SLY2_EE_LAYOUT (SPRITEDESC's EE size) */

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

/* An animation key (0x20 bytes; LoadSpriteGroups): its time, the poses of its items (0x40-byte records), colour
   pairs, the items it refers to, its actions (7: a sound event) and tagged values. */
struct SPRITEKEY
{
    float time;                  /* +0x00 */
    uchar numPoses;              /* +0x04 */
    uchar numColors;             /* +0x05 */
    uchar numItemRefs;           /* +0x06 */
    uchar numActions;            /* +0x07 */
    uchar *actions;              /* +0x08 */
    void *poses;                 /* +0x0C */
    void *colors;                /* +0x10 two RGBA words each */
    ushort *itemRefs;            /* +0x14 indices into the slot's items */
    uchar numTags;               /* +0x18 */
    char pad19[3];
    void *tags;                  /* +0x1C {kind, value} pairs */
};
typedef char assert_SPRITEKEY_size[sizeof(SPRITEKEY) == 0x20 ? 1 : -1];

/* A sprite group's animation (0x18 bytes; LoadSpriteGroups): its keys, length and label id. */
struct SPRITEANIM
{
    ushort numKeys;              /* +0x00 */
    ushort pad2;
    SPRITEKEY *keys;             /* +0x04 */
    float duration;              /* +0x08 */
    WIDGET *owner;               /* +0x0C */
    SPRITEGRP *group;            /* +0x10 */
    int id;                      /* +0x14 the label (0x30F appear, 0x310 loop, 0x311 disappear, ...) */
};

/* A 2-D transform override (a 2x3 matrix, 0x20 bytes: WIDGET::xform, SPRITEGRP::xform, the sprite items' poses). */
struct SPRITEXF { float a[6]; } __attribute__((aligned(16)));

/* Sprite descriptor kinds (SPRITEDESC::kind; LoadSpriteDescriptor). */
enum SPRITE_KIND
{
    SPRITE_KIND_NONE = 0,
    SPRITE_KIND_GEOMETRY = 1,   /* VU1 line / strip records */
    SPRITE_KIND_POLYGON = 2,
    SPRITE_KIND_TEXT = 3,       /* drawn by RenderSpriteText */
    SPRITE_KIND_NESTED = 4,     /* shows another sprite group */
    SPRITE_KIND_UNUSED5 = 5,
    SPRITE_KIND_EFFECT = 6,     /* a textured quad from a render target */
    SPRITE_KIND_PICTURE = 7,    /* a textured quad from a texture id */
    SPRITE_KIND_TABLE = 8,      /* a nested sprite table with word / float parameters */
};

union QW;
struct SPRITEDESC;

/* SPRITE_KIND_TABLE: a nested descriptor table and its parameter tables. EE offsets are from the descriptor. */
struct SPRITETABLE
{
    int count;                   /* +0x10 */
    SPRITEDESC *children;        /* +0x14 */
    int floatCount;              /* +0x18 */
    float *floats;               /* +0x1C */
    int wordCount;               /* +0x20 */
    uint *words;                 /* +0x24 */
};

/* SPRITE_KIND_TEXT: the text and its layout (RenderSpriteText, ConstructRichText2FromSprite). The loader reads color,
   scale, height, width, hAlign, source and id in that order; vAlign is 1 only for the language string 0x2F2. */
struct SPRITETEXT
{
    int tag;                     /* +0x10 (unknown) */
    int source;                  /* +0x14 0: a language string, 1: a text table entry (the getter's id) */
    int id;                      /* +0x18 */
    int color;                   /* +0x1C an RGBA word */
    float scale;                 /* +0x20 */
    float width;                 /* +0x24 */
    float height;                /* +0x28 */
    int hAlign;                  /* +0x2C */
    int vAlign;                  /* +0x30 */
};

/* A polygon vertex as stored in the file (0x18 bytes, not interpreted by the loader). */
struct SPRITEVERTEX { uchar data[0x18]; };

/* SPRITE_KIND_POLYGON: a 2x3 transform and its vertices. */
struct SPRITEPOLYGON
{
    SPRITEXF matrix;             /* +0x10 */
    ushort count;                /* +0x30 */
    ushort pad;
    SPRITEVERTEX *vertices;      /* +0x34 */
};

/* SPRITE_KIND_EFFECT: a quad textured with a render target, found by object id when an item binds the sprite
   (SetSpriteItemSource) and released with it (ReleaseSpriteItem). */
struct SPRITEEFFECT
{
    int targetOid;               /* +0x10 the object FindObjectForRenderTarget looks up (-1: none) */
    RENDERTGT *target;           /* +0x14 the acquired render target (its as-texture packet is drawn first) */
    int count;                   /* +0x18 the quad's VIF packet */
    QW *packet;                  /* +0x1C */
};

/* SPRITE_KIND_PICTURE: a quad textured with a texture id; the TEX packet is built after the load
   (BuildSpritePictureTexPacket, posted as a callback). */
struct SPRITEPICTURE
{
    int textureId;               /* +0x10 FindShaderById */
    int count;                   /* +0x14 the quad's VIF packet */
    QW *packet;                  /* +0x18 */
    int texPacketCount;          /* +0x1C */
    QW *texPacket;               /* +0x20 */
};

/* SPRITE_KIND_GEOMETRY: line / strip records grouped into VU1 packets (microprogram 0x140). */
struct SPRITEGEOMETRY
{
    ushort count;                /* +0x10 records */
    ushort groups;               /* +0x12 VU1 batches (RenderSpriteGeometry: 2 or more selects mode 1) */
    int samples;                 /* +0x14 vertices over all records (the fade-in limit counts these) */
    int packetCount;             /* +0x18 */
    QW *packet;                  /* +0x1C */
};

/* A sprite descriptor (0x40 bytes on the EE; WIDGET::sprites, LoadSpriteDescriptor): its id, the SPRITE_KIND, the
   widget, and the kind's payload. The payload starts at +0x10 on the EE; natively the polygon's 16-byte aligned
   matrix puts it at +0x20 (0x50 bytes), so every reader goes through these members. */
struct SPRITEDESC
{
    int id;                      /* +0x00 */
    int kind;                    /* +0x04 SPRITE_KIND */
    WIDGET *owner;               /* +0x08 */
    int unknownC;                /* +0x0C */
    union
    {
        int index;               /* +0x10 NESTED: the sprite group (WIDGET::spriteGroups) */
        SPRITETABLE table;
        SPRITETEXT text;
        SPRITEPOLYGON polygon;
        SPRITEEFFECT effect;
        SPRITEPICTURE picture;
        SPRITEGEOMETRY geometry;
    };
};
#if defined(SLY2_EE_LAYOUT)
typedef char assert_SPRITEDESC_size[sizeof(SPRITEDESC) == 0x40 ? 1 : -1];
typedef char assert_SPRITEDESC_picture[SLY2_OFFSETOF(SPRITEDESC, picture.texPacket) == 0x20 ? 1 : -1];
#endif

/* A sprite item (0x70 bytes): one sprite placed by a slot's current key: its colours, 2-D pose, descriptor, the slot
   of the nested group it plays, an argument, and the link of the widget's render queue. */
struct SPRITEITEM
{
    RGBA color0;                 /* +0x00 */
    RGBA color1;                 /* +0x04 */
    char pad8[8];
    SPRITEXF matrix;             /* +0x10 */
    SPRITEDESC *descriptor;      /* +0x30 */
    int kind;                    /* +0x34 4: queued as is when it has no descriptor (inferred) */
    /* +0x38..+0x57: the two key poses ApplySpriteKeyPoses interpolates between (names from its view; InitSpriteItem
       sets the previous matrix to the identity, D_002A3A10) (inferred) */
    float previousTime;          /* +0x38 */
    RGBA previousColor0;         /* +0x3C */
    RGBA previousColor1;         /* +0x40 */
    SPRITEXF *previousMatrix;    /* +0x44 */
    float nextTime;              /* +0x48 */
    RGBA nextColor0;             /* +0x4C */
    RGBA nextColor1;             /* +0x50 */
    SPRITEXF *nextMatrix;        /* +0x54 */
    int spriteSlot;              /* +0x58 NESTED: the slot of the nested group */
    float cachedTime;            /* +0x5C the time the pose was last evaluated at (inferred) */
    uchar interpolate;           /* +0x60 the keys are interpolated (inferred) */
    uchar drawFlags;             /* +0x61 QueueSpriteItem's flags (RenderWidget's channel) */
    char pad62[0x64 - 0x62];
    int arg;                     /* +0x64 */
    SLE link;                    /* +0x68 link in WIDGET::renderQueue */
    int unknown6C;               /* +0x6C */
};
typedef char assert_SPRITEITEM_size[sizeof(SPRITEITEM) == 0x70 ? 1 : -1];
typedef char assert_SPRITEITEM_previousTime[SLY2_OFFSETOF(SPRITEITEM, previousTime) == 0x38 ? 1 : -1];
typedef char assert_SPRITEITEM_previousMatrix[SLY2_OFFSETOF(SPRITEITEM, previousMatrix) == 0x44 ? 1 : -1];
typedef char assert_SPRITEITEM_nextMatrix[SLY2_OFFSETOF(SPRITEITEM, nextMatrix) == 0x54 ? 1 : -1];
typedef char assert_SPRITEITEM_cachedTime[SLY2_OFFSETOF(SPRITEITEM, cachedTime) == 0x5C ? 1 : -1];

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
    char padC[0x10 - 0xC];
    ulong lastStartFrame;        /* +0x10 the main track's last restart (StartSpriteAnimSlot, StopSpriteSlot; inferred) */
    float lastStartTime;         /* +0x18 */
    int lastStartAnim;           /* +0x1C */
    int pendingAnim;             /* +0x20 animation to start (-1 none; PlayWidgetAnim, StopWidgetAnim) */
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

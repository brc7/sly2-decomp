/* include/sly2/richtext.h -- CRichText2, the HUD's rich-text record.
 *
 * A second rich-text class next to font.cpp's CRichText (Sly 1's; Reset__9CRichText is 0x15F970): hud.cpp's
 * CRichText_ctor / ConstructRichText2FromSprite build it, ResetRichText2 .. DrawRichText2 read it, and render.cpp's
 * DrawWorldLabel keeps one on the stack. 0x98 bytes. Field names come from the per-function views it replaces
 * (they used the same names in every copy). */
#ifndef SLY2_RICHTEXT_H
#define SLY2_RICHTEXT_H

#include "sly2/basic.h"

struct CRichText2
{
    uchar *text;                 /* +0x00 the string */
    uchar *cursor;               /* +0x04 the next character */
    char *font;                  /* +0x08 the current font: a 0x40-byte sprite of the HUD font WIDGET (SW::ploHudFont) */
    char *baseFont;              /* +0x0C the font a reset returns to */
    char *altFont;               /* +0x10 the font of the escape sequence (the HUD font's second sprite) */
    RGBA color;                  /* +0x14 the current colour */
    RGBA baseColor;              /* +0x18 the colour a reset returns to */
    RGBA altColor;               /* +0x1C the escape colour (ResetRichText2: black at the current alpha) */
    QWROW color0;                /* +0x20 the two float colour vectors the glyphs draw with (UpdateRichText2Color) */
    QWROW color1;                /* +0x30 */
    QWROW baseColor0;            /* +0x40 their base values */
    QWROW baseColor1;            /* +0x50 */
    int changed;                 /* +0x60 a style change is pending */
    float originX;               /* +0x64 the screen origin (GS units) */
    float originY;               /* +0x68 */
    float offsetX;               /* +0x6C the box: offset, size, scale */
    float offsetY;               /* +0x70 */
    float width;                 /* +0x74 */
    float height;                /* +0x78 */
    float scaleX;                /* +0x7C */
    float scaleY;                /* +0x80 */
    float lineHeight;            /* +0x84 */
    float fontScale;             /* +0x88 */
    int vAlign;                  /* +0x8C 0 top, 1 centre, 2 bottom */
    int hAlign;                  /* +0x90 0 left, 1 centre, 2 right */
    int priority;                /* +0x94 render priority */
};
typedef char assert_CRichText2_size[sizeof(CRichText2) == 0xA0 ? 1 : -1];   /* 0x98 of fields, qword-aligned */
typedef char assert_CRichText2_changed[SLY2_OFFSETOF(CRichText2, changed) == 0x60 ? 1 : -1];
typedef char assert_CRichText2_priority[SLY2_OFFSETOF(CRichText2, priority) == 0x94 ? 1 : -1];

#endif

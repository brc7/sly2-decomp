/* include/sly2/light.h -- the per-object light packets (engine/graphics/pipeline/light.cpp builds them, glob.cpp's
 * RenderSoGlobset and BuildModelDrawPackets call them from the model's DMA chain).
 *
 * SOBOUNDS is the record sly2/so.h's SO::psobounds points at (so.h only declares it): the point the lights are ranked
 * for, the light-stamp it was built for, and the DMA chains BuildSoLightPackets / BuildLightPackets append to the
 * frame's VIF1 stream (the four best lights' VU1 payload; the projected-volume packet and its clip matrix). */
#ifndef SLY2_LIGHT_H
#define SLY2_LIGHT_H
#include "sly2/basic.h"

/* SOBOUNDS: one object's light packets (0x30) */
struct SOBOUNDS
{
    int unknown0;
    qword *pposCenter;      /* +0x04 the point the lights are ranked for */
    int unknown8;
    int stamp;              /* +0x0C the world's light stamp it was built for (RefreshSoBoundsRecord) */
    float strength;         /* +0x10 0: no light packets */
    int cqwPacket;          /* +0x14 the light payload chain (four 48-byte VIF packets) */
    uchar *pbPacket;        /* +0x18 */
    float sRadius;          /* +0x1C 0: no projected volume; else its radius */
    float sExtra;           /* +0x20 added to the volume's extent */
    MATRIX4 *pmatClip;      /* +0x24 the clip matrix inside the projected-volume packet (0: none) */
    int cqwVolume;          /* +0x28 the projected-volume chain (a DMA REF of it ends the model's packet) */
    uchar *pbVolume;        /* +0x2C */
};
typedef char assert_SOBOUNDS_cqwPacket[SLY2_OFFSETOF(SOBOUNDS, cqwPacket) == 0x14 ? 1 : -1];
typedef char assert_SOBOUNDS_pmatClip[SLY2_OFFSETOF(SOBOUNDS, pmatClip) == 0x24 ? 1 : -1];
typedef char assert_SOBOUNDS_size[sizeof(SOBOUNDS) == 0x30 ? 1 : -1];

#endif

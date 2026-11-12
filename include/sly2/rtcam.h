/* include/sly2/rtcam.h -- the render-target camera (engine/graphics/pipeline/rtcam.cpp): an XFM that renders a list of
 * objects into one of the two 128x128 render targets (RT). glob.cpp's BuildModelDrawPackets reads the camera's shown
 * list as its light list (RenderRtcam is its only caller). */
#ifndef SLY2_RTCAM_H
#define SLY2_RTCAM_H
#include "sly2/xfm.h"
#include "sly2/so.h"
#include "sly2/shd.h"
#include "qw.h"

struct RTCAM;
/* A render target (g_sw.artgt[2], sly2/sw.h's RENDERTGT, 0x28 bytes; this view reads TEX0 as sly2/shd.h's bit-fields): its size, texture register, the two GIF packets built by
   InitRenderTargets, and the camera object that owns it (0 when free). */
struct RT
{
    int dx, dy;
    TEX0 tex0;
    int cqwClear;      /* +0x10 FRAME/ZBUF/offset/scissor setup and the clearing sprites */
    QW *aqwClear;
    int cqwTex;        /* +0x18 TEX0/TEX1/CLAMP of the target as a texture */
    QW *aqwTex;
    RTCAM *camera;     /* +0x20 */
    int pad24;
};
typedef char assert_RT_size[SLY2_EE_CHECK(sizeof(RT) == 0x28) ? 1 : -1];

/* An object of the shown list (+0x108). */
struct RTO : SO { char pad150[0x15C - 0x150]; int unknown15C; int unknown160; };   /* both set to 1 while shown in a target */
/* The camera object (an XFM, which ends at +0xE0). */
struct RTCAM : XFM
{
    int c;             /* +0xE0 objects hidden while rendering, with their flags (1 shown again, 2 re-enabled) */
    LO *ap[4];
    int af[4];
    int numShown;      /* +0x104 objects shown only in the target */
    RTO *shown[4];
    RTCAM *parent;     /* +0x118 */
    float fov, clipNear, clipFar;
    RT *prt;           /* +0x128 the claimed target */
};
typedef char assert_RTCAM_prt[SLY2_EE_CHECK(SLY2_OFFSETOF(RTCAM, prt) == 0x128) ? 1 : -1];
#endif

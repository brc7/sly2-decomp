/* include/sly2/render.h -- the frame's render list.
 *
 * Objects append RPL entries (AllocRenderListEntry in engine/graphics/pipeline/render.cpp hands them out from
 * phasemem's buffer D_003B3980 through the cursor D_003D4A50); draw.cpp sorts them by pass kind and depth and emits
 * one or two DMA CALL tags per entry into the frame's VIF1 chain. Self-contained (no sly2/basic.h): some includers
 * keep their own basic types. */
#ifndef SLY2_RENDER_H
#define SLY2_RENDER_H

/* RPL: one render-list entry. */
struct RPL
{
    int kind;       /* +0x0 pass kind, 0..25 (selects the begin/end hooks of draw.cpp's pass table) */
    float depth;    /* +0x4 sort depth (squared distance to the camera for the model passes) */
    unsigned char *pbCall;  /* +0x8 first DMA CALL target (the entry's own packet), 0 = none */
    void *pvCall;   /* +0xC second DMA CALL target (a sub-mesh's shared chain), 0 = none */
};
typedef char assert_RPL_size[sizeof(RPL) == 0x10 ? 1 : -1];

#endif

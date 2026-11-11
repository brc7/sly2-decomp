/* include/sly2/frm.h -- FRM, the double-buffered render frame (engine/graphics/backend/frm.cpp: D_002DEA50[2] and
 * the open frame g_pfrmOpen; phasemem.cpp gives the frames their packet buffers, draw.cpp records where the frame's
 * VIF chain starts). The globals are declared by each user: declaring them here would move their first
 * declaration in frm.cpp ahead of its packet builders and reorder its .bss. */
#ifndef SLY2_FRM_H
#define SLY2_FRM_H
#include "sly2/basic.h"

union QW;

/* A render frame (two of them, D_002DEA50). */
struct FRM
{
    int gifsQwordCount;     /* 0x00 finished GIF chain */
    QW *gifsChain;          /* 0x04 */
    QW *gifsBuffer;         /* 0x08 GIF buffer (phasemem.cpp SetPhase) */
    int gifsBufferQwords;   /* 0x0c */
    int vifsQwordCount;     /* 0x10 finished VIF chain */
    QW *vifsChain;          /* 0x14 */
    QW *vifsBuffer;         /* 0x18 VIF buffer (phasemem.cpp SetPhase) */
    int vifsBufferQwords;   /* 0x1c */
    int drawQwordCount;     /* 0x20 VIF qwords from drawChain to the end */
    QW *drawChain;          /* 0x24 start of the frame VIF chain sent to VIF1 (draw.cpp DrawSw sets it) */
    int unused28;           /* 0x28 zero in every snapshot; no code writes it */
    int uploadedTextureBits; /* 0x2c per-frame bits of textures already uploaded (fonts) (inferred) */
    int blendFlags;         /* 0x30 bit 0: blend the previous frame */
    DL releaseList;         /* 0x34 objects to notify when the frame is done (vtable slot 2) */
    DL freeList;            /* 0x40 records to free when the frame is done */
    int cameraVisFlags;     /* 0x4c g_cm.grfvis when the frame was opened */
};
typedef char assert_FRM_size[sizeof(FRM) == 0x50 ? 1 : -1];

#endif /* SLY2_FRM_H */

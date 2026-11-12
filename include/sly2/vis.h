/* include/sly2/vis.h -- VISZONE, the visibility zone (the *_vz objects): a volume whose camera visibility mask the camera
 * takes while it is inside (cm.cpp SetCmVisZone copies VISZONE::grfvis to CM::grfvis; objects render only when their
 * LO::visibilityMask shares a bit with it). Its own header because cm.cpp cannot include classes.h (classes.h brings
 * so.h, which changes cm.cpp's s_rgba numbering); classes.h includes it where the class was defined. */
#ifndef SLY2_VIS_H
#define SLY2_VIS_H
#include "sly2/basic.h"
#include "sly2/xfm.h"

/* 5 VISZONE (was CID5; 0x120): vis zone (all *_vz WEs); vtable 0x2DC070, 50 slots */
struct VISZONE : XFM
{
    int grfvis;                 /* +0xE0 the zone's camera visibility mask (LoadVisFromBrx; SetCmVisZone copies it to CM::grfvis) */
    int wordE4;                 /* +0xE4 an int option (g_aeopid) */
    BSPC bspc;                  /* +0xE8 the zone volume's BSP (CBinaryInputStream_ReadBspc; nodes: the root node) */
    GEOM geom;                  /* +0xF8 the zone volume (ReadGeom) */
    int isInside;               /* +0x110 the camera was inside at the last test (-1 after Init: unknown) */
    float changeTime;           /* +0x114 g_clock.time when isInside last changed */
    int unknown118[2];
    virtual void Init() __asm__("InitVis");   /* slot 7 */
    virtual CLASSINFO *GetClassInfo() __asm__("GetCid5ClassDesc");   /* slot 6 */
    virtual void LoadFromBrx(CBinaryInputStream *pbis) __asm__("LoadVisFromBrx");   /* slot 14 */
};
typedef char assert_CID5_size[sizeof(VISZONE) == 0x120 ? 1 : -1];
typedef char assert_CID5_isInside[SLY2_OFFSETOF(VISZONE, isInside) == 0x110 ? 1 : -1];

#endif

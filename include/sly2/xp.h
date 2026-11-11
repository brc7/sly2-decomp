/* include/sly2/xp.h -- XP, the contact record the narrow phase (mark.cpp) builds between two parts (SOPART) and the
 * solver (solve.cpp, dzg.cpp) and the players' movement code (step.cpp, air.cpp, jt.cpp) read; one per side XPS.
 * SW::slotheapXp holds them (0xC0 each); an SO tree's XPLIST (so.h) chains them through XP::pxpNext. */
#ifndef SLY2_XP_H
#define SLY2_XP_H
#include "sly2/basic.h"

/* A contact (XP, 0xC0; SW::slotheapXp holds them) between two parts: per side the part, its transform, the root
   object of its tree, its object (told about the contact through SO slots 54..56), the contact point in world space,
   the same point in the transform's frame (TransformXfPos) and its velocity (GetXfVelocity); then the normal (side 0
   to side 1), the separation along it, friction and elasticity (defaults D_002BFD30/D_002BFD34), the kind (4..6
   sphere/BSP face, edge, vertex; 7 sphere/sphere; 8/9 swept vertex; 12 inside; 13 BSP/BSP fallback), the two
   features that made it (an EDGE, SURF or vertex per kind), the index in its group, the list link and the relative
   velocity. Field names follow dzg.cpp's and solve.cpp's views of the same record. */
struct XPS
{
    SOPART *ppart;          /* +0x00 */
    XF *pxf;                /* +0x04 */
    SO *psoRoot;            /* +0x08 SO::physicsTree->root */
    SO *pso;                /* +0x0C */
    qword pos;              /* +0x10 */
    qword posLocal;         /* +0x20 */
    qword v;                /* +0x30 */
};
struct XP
{
    XPS axps[2];
    union
    {
        qword normal;       /* +0x80 */
        VECTOR4 normalVec;  /* the same as floats (air.cpp, jt.cpp read .x / .y / .z) */
    };
    float g;                /* +0x90 */
    float gFriction;        /* +0x94 */
    float gElasticity;      /* +0x98 */
    int kind;               /* +0x9C */
    union
    {
        struct
        {
            void *pvFeature0;   /* +0xA0 */
            void *pvFeature1;   /* +0xA4 */
        };
        float sBeam;            /* a LASEN beam's contact: its distance from the beam's first endpoint (sensor.cpp) */
        struct
        {
            float gLimitMin;    /* kind 14: the solver row's limits per second (solve.cpp SolveSwGroup scales them by */
            float gLimitMax;    /* the frame time; a negative minimum makes the row two-sided) */
        };
    };
    int ixp;                /* +0xA8 */
    XP *pxpNext;            /* +0xAC */
    qword vRel;             /* +0xB0 */
};
typedef char assert_XP_size[sizeof(XP) == 0xC0 ? 1 : -1];   /* CloneXpList copies 0xC0 */

#endif

#include "vec.h"
struct R;
typedef struct { short d; short pad; void (*pfn)(void *, float, float, struct VECTOR *, int); } VT;
struct C { VT *vt; };
struct R { int x0; struct C *x4; };
struct O { char pad[0x74]; float x74; };
struct E { int x0; struct O *x4; int x8; float xc; };
struct P { int x0; char pad[0x50-4]; struct E ae[2]; int x70; };
struct R *func_0012FF10(struct O *o, int a, int b, int c);
void EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(int a, int b, int c, void *d, int e, int f, int g, void *h);
/** @sly2-annot addr=0x19DE58
 * Purpose: For the 2 climb clips measure their per-cycle climb speed (0x12FF10 key lookup) then build the
 *   climb blend (EnsureAsegBlendDynamic 0x137A30)
 * Info: proposed name PostJtLoadPipe (T1); TU pipe_jt
 * Confidence: mid (draft) [T1 0.65] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#pipe_jt-0x19de10-0x19f0a8
 * @end-sly2-annot */
void PostJtLoadPipe(struct P *p)
{
    int i;
    struct E *e;
    struct VECTOR v0, v1;
    for (i = 0, e = p->ae; i < 2; i++, e++) {
        struct O *o = e->x4;
        if (o) {
            struct R *r = func_0012FF10(o, p->x0, 1, 1);
            if (r) {
                r->x4->vt[3].pfn((char *)r->x4 + r->x4->vt[3].d, 0.0f, 1.0f, &v0, 0);
                r->x4->vt[3].pfn((char *)r->x4 + r->x4->vt[3].d, o->x74, 1.0f, &v1, 0);
                e->xc = (v0.z - v1.z) / o->x74;
            }
        }
    }
    EnsureAsegBlendDynamic__FP3ALOiiPviPiT5PP6ASEGBL(p->x0, 0x10, 2, p->ae, 0, 0, 0, &p->x70);
}

/* func_00140750 (Claude agent): matched. GMin/GMax as `b < a ? b : a`, block-local dt per case, and the x10C8 state as `if (xDF4 != -1 || (g = x10CC) == 0) -1; else if (g == 1 || (x10C8 == 1 && 0 < g)) 1; else 0`. */
typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; int (*pfn)(void *); };
struct P {
    char pad[0xC]; struct VTE *vt; char pad1[0xDF4 - 0x10]; int xDF4;
    char pad2[0x10C4 - 0xDF8]; void *p10C4; int x10C8; float x10CC; float x10D0;
};
extern struct { float t; float dt; } D_002DDED0;
extern float D_002BCC6C, D_002BCC70, D_002BCC74, D_002BCC78;
int func_001409E8(struct P *p);
qword func_001A62E8(struct P *p);
void func_00156548(void *a, qword pos, struct P *p, int n);

static inline float GClamp(float g, float lo, float hi)
{
    float r;
    if (g < lo) r = lo;
    else if (hi < g) r = hi;
    else r = g;
    return r;
}

static inline float GMin(float a, float b) { return b < a ? b : a; }
static inline float GMax(float a, float b) { return b > a ? b : a; }

/** @sly2-annot addr=0x140750
 * Purpose: If +0x10C4 (a shadow/hide volume set) exists: smooth +0x10CC toward 1/0 by 0x1409E8 result, derive
 *   +0x10C8 state (-1/0/1), combine with vt98, smooth +0x10D0 and at 1.0 register with 0x156548
 * Info: proposed name UpdateStepplayerHideBlend (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00140750(struct P *p)
{
    int k;
    float g;
    if (!p->p10C4) return;
    switch (func_001409E8(p)) {
    case 1:
        { float dt = D_002DDED0.dt; p->x10CC = GMin(p->x10CC + D_002BCC6C * dt, 1.0f); }
        break;
    case 0:
        { float dt = D_002DDED0.dt; p->x10CC = GMax(p->x10CC + D_002BCC70 * dt, 0.0f); }
        break;
    case 2:
        p->x10CC = 0.0f;
        break;
    }
    if (p->xDF4 != -1 || (g = p->x10CC) == 0.0f) p->x10C8 = -1;
    else if (g == 1.0f || (p->x10C8 == 1 && 0.0f < g)) p->x10C8 = 1;
    else p->x10C8 = 0;
    k = p->vt[98].pfn((char *)p + p->vt[98].d);
    if (p->x10C8 != 1 && k == 1) k = 2;
    switch (k) {
    case 0:
        p->x10D0 = 0.0f;
        break;
    case 1:
        { float dt = D_002DDED0.dt; p->x10D0 = GClamp(p->x10D0 + D_002BCC74 * dt, 0.0f, 1.0f); }
        break;
    case -1:
    case 2:
        { float dt = D_002DDED0.dt; p->x10D0 = GClamp(p->x10D0 + D_002BCC78 * dt, 0.0f, 1.0f); }
        break;
    }
    if (p->x10D0 == 1.0f) func_00156548(p->p10C4, func_001A62E8(p), p, 0);
}

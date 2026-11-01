typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(void *, int); };
struct S { char pad[0xC]; struct VTE *vt; void *p10; char pad2[0x50]; float g64; float g68; float g6C; char pad3[0x10]; qword q80; };
extern float D_002BE624, D_002BE628, D_002BE62C;
void StartBhv(struct S *p);
qword func_001A62E8(void *p);
static inline float Max(float a, float b) { return a > b ? a : b; }
static inline float Min(float a, float b) { return a < b ? a : b; }
/** @sly2-annot addr=0x177EF0
 * Purpose: BHVPATH Start: remember start pos, clamp radii from speed, SetState(0)
 * Info: proposed name StartBhvpath (T1); class BHVPATH slot8; TU guardbhv
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void StartBhvpath(struct S *p) {
    struct VTE *e;
    float g;
    StartBhv(p);
    p->q80 = func_001A62E8(p->p10);
    g = Max(p->g64, D_002BE62C);
    p->g64 = g; p->g68 = Min(g * 0.25f, D_002BE624); p->g6C = Min(g, D_002BE628);
    e = &p->vt[0xF];
    e->pfn((char *)p + e->d, 0);
}

struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
struct S { char pad[0x54]; int n; float t; char pad2[0x78 - 0x5C]; float dt; char pad3[4]; struct O *po; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern int D_003E0F9C;
void func_001A5E28(struct S *p);
void func_00165118(struct S *p, int n);
/** @sly2-annot addr=0x163B48
 * Purpose: TASK Update: time-limited available task sends timeout (SendTaskTimeout); shows its marker LO
 *   (+0x80, AddLo) only while available and no job is current, else removes it
 * Info: proposed name UpdateTask (T1); class TASK slot16; TU game
 * Notes: superseded r3_1539A8 proposal: UpdateCid85 (0.65) | runtime6 G3: marker LO +0x80 shown by state == 1
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources: r3b_rest+runtime
 * See: docs/engine/game-progression.md#task-lo-child-0x110, docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void func_00163B48(struct S *p)
{
    struct O *po;
    func_001A5E28(p);
    if (p->n == 1 && p->dt > 0.0f) {
        float dt = D_002DDED0.t - p->t;
        if (dt < p->dt && p->dt + D_002DDED0.dt <= dt)
            func_00165118(p, 0);
    }
    po = p->po;
    if (po) {
        int f = p->n == 1; if (D_003E0F9C) f = 0;
        if (f) {
            struct VTE *e = &po->vt[8];
            e->pfn((char *)po + e->d);
        } else {
            struct VTE *e = &po->vt[9];
            e->pfn((char *)po + e->d);
        }
    }
}

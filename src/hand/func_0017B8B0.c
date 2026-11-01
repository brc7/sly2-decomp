struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x20]; int n30; char pad3[8]; int n3C; int n40; };
struct S { char pad[0x10]; void *p10; int pad2; char a18[0x18]; int n30; float g34; char pad3[8]; int n40; };
void func_001EF0D8(void *p);
struct O *func_00205438(void *p, int n);
void func_00208540(void *p, int n);
/** @sly2-annot addr=0x17B8B0
 * Purpose: BHVTARGETS state: 0 idle and no target, 1 push BHVCOMBAT on the chosen target, 2 idle hands-off
 * Info: proposed name SetBhv178State (T2); TU guardbhv
 * Notes: BHV class word normalised: SetBhvtargetsState -> SetBhv178State
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017B8B0(struct S *p, int n) {
    if (n == p->n30) return;
    func_001EF0D8(p->a18);
    p->n30 = n;
    p->g34 = D_002DDED0.t;
    switch (n) {
    case 0:
        { struct O *o = func_00205438(p->p10, 0x9E); struct VTE *e = &o->vt[8]; e->pfn((char *)o + e->d); }
        func_00208540(p->p10, 0);
        break;
    case 1:
        { struct O *o = func_00205438(p->p10, 0xA5); struct VTE *e; o->n30 = p->n40; o->n40 = 1; e = &o->vt[8]; e->pfn((char *)o + e->d); }
        break;
    case 2:
        { struct O *o = func_00205438(p->p10, 0x9E); struct VTE *e; o->n3C = 1; e = &o->vt[8]; e->pfn((char *)o + e->d); }
        break;
    }
}

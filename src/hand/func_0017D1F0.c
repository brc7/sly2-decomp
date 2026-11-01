typedef int qword __attribute__((mode(TI)));
struct M3 { qword a, b, c; };
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x2C]; int n3C; };
struct T { char pad[0x1B0]; struct M3 m1B0; char pad2[0xF84 - 0x1E0]; void *pF84; };
struct S { char pad[0x10]; struct T *pt; };
extern qword D_002A2DC0;
void func_001EEF70(struct S *p);
struct O *func_00205438(void *p, int n);
qword *func_001A6318(void *p);
struct M3 func_0011B830(qword a, qword b);
/** @sly2-annot addr=0x17D1F0
 * Purpose: BHVATTACH19 Start: push BHVANIM flag 2 and align to the attach object's frame
 * Info: proposed name StartBhv187 (T2); class BHV187 slot8; TU guardbhv
 * Notes: BHV class word normalised: StartBhvattach19 -> StartBhv187
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017D1F0(struct S *p) {
    struct O *o; struct VTE *e; struct T *t;
    func_001EEF70(p);
    o = func_00205438(p->pt, 0x9D); o->n3C = 2; e = &o->vt[8]; e->pfn((char *)o + e->d);
    t = p->pt;
    t->m1B0 = func_0011B830(*func_001A6318(t->pF84), D_002A2DC0);
}

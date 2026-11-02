struct E { int n; int a, b, c; };
struct T { char pad[0x8D0]; struct E ae[1]; };
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; };
struct T2 { char pad[0xF64]; void *pF64; char pad2[0xFEC - 0xF68]; struct O *pFEC; };
struct S { char pad[0x10]; struct T *pt; char pad2[0x2C]; struct E *p40; char pad3[0xC]; int n50; char pad4[8]; void *p5C; };
extern char D_002DEB30[];
int func_001F78A0(void *p);
void StartBhvhit(struct S *p);
void BroadcastGameEvent(void *g, int n, void *p);
void SetDetectorTargetStateById(void *p, int a, int b);
/** @sly2-annot addr=0x179780
 * Purpose: BHVKNOCKDOWN Start: entry from table +0x8D0, BHVHIT Start, event 0x15, start helper +0xFEC
 * Info: proposed name StartBhv162 (T2); class BHV162 slot8; TU guardbhv
 * Notes: no usable identifier; purpose only | BHV class word normalised: StartBhvknockdown -> StartBhv162
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources:
 *   verify:verdicts_B+verdicts_B+r9_bhv_screen
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_00179780(struct S *p) {
    p->p40 = &p->pt->ae[func_001F78A0(p->p5C)];
    if (p->p40->n == -1) p->p40 = 0;
    StartBhvhit(p);
    BroadcastGameEvent(D_002DEB30, 0x15, p->pt);
    SetDetectorTargetStateById(((struct T2 *)p->pt)->pF64, p->n50, 2);
    { struct O *o = ((struct T2 *)p->pt)->pFEC; if (o) { struct VTE *e = &o->vt[8]; e->pfn((char *)o + e->d); } }
}

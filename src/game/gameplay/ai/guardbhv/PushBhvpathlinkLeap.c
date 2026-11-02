typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x40]; qword q50; char pad3[8]; int n68; };
struct S { char pad[0x10]; void *p10; char pad2[0x1C]; char *p30; int n34; };
struct O *func_00205438(void *p, int n);
qword func_001A62E8(void *p);
qword func_001C1EC0(void *pe, qword q);
/** @sly2-annot addr=0x17E628
 * Purpose: Push BHVLEAP to the current segment endpoint
 * Info: proposed name PushBhvpathlinkLeap (T1); TU guardbhv
 * Notes: names_v2 v1 hint not used: BindStepguard__FP9STEPGUARD | runtime2/4 confirmed the BHVPATHLINK role
 *   on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.60] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void PushBhvpathlinkLeap(struct S *p) {
    struct O *o = func_00205438(p->p10, 0xAC);
    struct VTE *e;
    o->n68 = 1;
    o->q50 = func_001C1EC0(p->p30 + 0x60 + p->n34 * 32, func_001A62E8(p->p10));
    e = &o->vt[8];
    e->pfn((char *)o + e->d);
}

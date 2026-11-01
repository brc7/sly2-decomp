typedef int qword __attribute__((mode(TI)));
struct G;
extern struct G D_003DD020;
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0xA8 - 0x10]; int nA8; int nAC; char pad3[0xC]; int nBC; };
struct S { char pad[0x10]; void *p10; char pad2[0x1C]; int n30; int pad3; float g38; int pad4; char a40[0x10]; int n50; int n54; int n58; };
void StartBhv(struct S *p);
qword func_001A62E8(void *p);
void FindShapeClosestPointFromS(int a, float g, qword q, struct G *pg, void *pv, int b, int c, float *pf);
void FindShapeClosestPoint(int a, qword q, struct G *pg, void *pv, int b, int c, float *pf);
struct O *func_00205438(void *p, int n);
void AdvanceBhvpatrolpathGoal(struct S *p, int n);
/** @sly2-annot addr=0x17EB88
 * Purpose: BHVPATROLPATH Start: closest parameter on the path shape, push BHVGOTO, first goal
 * Info: proposed name StartBhvpatrolpath (T1); class BHVPATROLPATH slot8; TU guardbhv
 * Notes: names_v2 v1 hint not used: UpdateStepguardDrive__FP9STEPGUARD | runtime2/4 confirmed the
 *   BHVPATROLPATH role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void StartBhvpatrolpath(struct S *p) {
    struct O *o;
    StartBhv(p);
    if (p->g38 >= 0.0f)
        FindShapeClosestPointFromS(p->n30, p->g38, func_001A62E8(p->p10), &D_003DD020, p->a40, 0, 0, &p->g38);
    else
        FindShapeClosestPoint(p->n30, func_001A62E8(p->p10), &D_003DD020, p->a40, 0, 0, &p->g38);
    o = func_00205438(p->p10, 0xBA);
    o->nA8 = p->n50;
    o->nAC = p->n54;
    o->nBC = p->n58;
    AdvanceBhvpatrolpathGoal(p, 1);
    o->vt[8].pfn((char *)o + o->vt[8].d);
}

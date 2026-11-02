typedef int qword __attribute__((mode(TI)));
struct G;
extern struct G D_003DD020;
struct O { char pad[0xE4]; int nE4; };
struct S { char pad[0x10]; void *p10; char pad2[0x1C]; struct O *po; float g34; int n38; int n3C; int pad3; float g44; float g48; char pad4[0xB0 - 0x4C]; char aB0[0x10]; };
qword func_001A62E8(void *p);
void FindShapeClosestPointFromS(struct O *po, float g, qword q, struct G *pg, void *pv, int b, int c, float *pf);
void FindShapeClosestPoint(struct O *po, qword q, struct G *pg, void *pv, int b, int c, float *pf);
float SMinShape(struct O *po);
float SMaxShape(struct O *po);
void EvaluateShapeS(struct O *po, float g, void *pv, int n);
/** @sly2-annot addr=0x178D90
 * Purpose: Set the SHAPE to follow and direction: find the start parameter (closest point) and, for open
 *   shapes, the end point
 * Info: proposed name SetBhvpathShape (T1); TU guardbhv
 * Notes: unverified names_v2 tier-B alignment hint (tier B verified ~33% precise) | superseded: UpdateAlo
 *   (names_v2:B(unverified))
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources:
 *   names_v2:B(unverified)+r9_bhv_screen+verdicts_followups
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void SetBhvpathShape(struct S *p, struct O *po, int n) {
    p->n38 = n;
    p->po = po;
    if (p->g48 >= 0.0f) {
        FindShapeClosestPointFromS(po, p->g48, func_001A62E8(p->p10), &D_003DD020, 0, 0, 0, &p->g34);
        p->g48 = -1.0f;
    } else {
        FindShapeClosestPoint(po, func_001A62E8(p->p10), &D_003DD020, 0, 0, 0, &p->g34);
    }
    if (po->nE4) {
        p->n3C = 0;
    } else {
        p->n3C = 1;
        if (p->n38) p->g44 = SMinShape(po);
        else p->g44 = SMaxShape(po);
        EvaluateShapeS(po, p->g44, p->aB0, 0);
    }
}

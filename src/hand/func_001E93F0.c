typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void *pfn; };
struct LO { int pad[3]; struct VTE *pvt; };
struct M4 { qword r[3]; qword pos; };
struct P { int pad[4]; struct LO *apo[4]; qword apos[2]; void *ap40[2]; int a48; };
extern char D_002C44F0[];
float UMinShape(int);
float UMaxShape(int);
void EvaluateShapeU(int, qword *, int, float);
/** @sly2-annot addr=0x1E93F0
 * Purpose: Attach chain end i to object (its matrix via vt32) or to the shape's start/end point
 * Info: proposed name SetChainEndObject (T2); TU shape
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
void func_001E93F0(struct P *p, int i, struct LO *po)
{
    p->apo[i] = po;
    if (po) {
        struct M4 m;
        struct VTE *pe = &po->pvt[32];
        p->ap40[i] = ((void *(*)(void *, struct M4 *))pe->pfn)((char *)po + pe->d, &m);
        *(p->apos + i) = m.pos;
    } else {
        float f;
        p->ap40[i] = D_002C44F0;
        if (i == 0)
            f = UMinShape(p->a48);
        else
            f = UMaxShape(p->a48);
        EvaluateShapeU(p->a48, &p->apos[i], 0, f);
    }
}

typedef int qword __attribute__((mode(TI)));
struct E { qword q; int pad[4]; };
struct S { char pad[0x50]; int c; char pad2[0xC]; struct E a[6]; char pad3[4]; int n124; int n128; int pad4; qword q130; qword q140; };
void PostLoLoad(struct S *p);
qword func_001C1F28(struct E *pe);
int PpathzoneFromPos(qword q, int n);
/** @sly2-annot addr=0x1C2170
 * Purpose: CID 124 vt15: PostLoLoad, cache first/last endpoint positions and their pathzones
 * Info: proposed name PostPathlinkLoad (T1); class PATHLINK slot15; TU path
 * Notes: names_v2 v1 proposed UpdateJtBounds__FP2JT (0.005)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void PostPathlinkLoad(struct S *p)
{
    PostLoLoad(p);
    p->q130 = func_001C1F28(&p->a[0]);
    p->n124 = PpathzoneFromPos(p->q130, 0);
    p->q140 = func_001C1F28(/*B*/&p->a[p->c] - 1/*E*/);
    p->n128 = PpathzoneFromPos(p->q140, 0);
}

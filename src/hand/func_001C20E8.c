struct E { void *p0; void *p4; int pad[6]; };
struct S { char pad[0x50]; int c; char pad2[0xC]; struct E a[6]; };
int func_00132B10(void *q, int n);
/** @sly2-annot addr=0x1C20E8
 * Purpose: Append endpoint (shape if class 10 else LO), max 6
 * Info: proposed name AddPathlinkEndpointObject (T2); TU path
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void func_001C20E8(struct S *p, void *q)
{
    if ((unsigned)p->c < 6) {
        struct E *pe = &p->a[p->c++];
        if (func_00132B10(q, 10)) pe->p0 = q; else pe->p4 = q;
    }
}

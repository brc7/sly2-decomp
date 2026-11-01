typedef int qword __attribute__((mode(TI)));
struct S { char pad[0xE0]; char xE0[4]; };
void *func_001BF110(void *p,qword pos,int flags);
int func_001C1700(struct S *p, int a, qword q0, qword q1, int c, void **ap);
int func_001BC9F0(void *pv, qword q);
/** @sly2-annot addr=0x1C09A0
 * Purpose: Point inside pathzone and not inside any registered obstacle (0x1C1700 query + 0x1BC9F0 test)
 * Info: proposed name FPosFreeInPathzone (T2); TU path
 * Notes: names_v2 v1 proposed HandleJtGrfjtsc (0.002)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
int func_001C09A0(struct S *p, qword q,int flags)
{
    void *ap[0x80];
    int n, i;
    if (!func_001BF110(p->xE0,q,flags)) return 0;
    n = func_001C1700(p, 0, q, q, 0x80, ap);
    for (i = 0; i < n; i++) {
        if (func_001BC9F0(ap[i], q)) return 0;
    }
    return 1;
}

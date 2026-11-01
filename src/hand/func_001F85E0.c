struct Q { int a, b, c; void *pvt; int pad[4]; void *p20; void *p24; int pad2[2]; };
extern char D_002CA280[];
struct Pool;
extern struct Pool D_003E15CC;
void *func_001F6030(void *);
struct Q *func_0011CCC0(struct Pool *);
void *func_002333B4(void *, int, int);
/** @sly2-annot addr=0x1F85E0
 * Purpose: Create a 0x30 handle object (vtable 0x2CA280 = CID 0x96) for the SOPART with given oid
 * Info: proposed name PsoparthNewSo (T2); TU so
 * Notes: names_v2 v1 proposed SendSoMessage__FP2SO5MSGIDPv (0.438)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct Q *func_001F85E0(void *pa)
{
    void *r = func_001F6030(pa);
    struct Q *q;
    if (!r) return 0;
    q = func_0011CCC0(&D_003E15CC);
    func_002333B4(q, 0, 0x30);
    q->p20 = pa;
    q->p24 = r;
    q->pvt = D_002CA280;
    q->a = 0;
    q->b = 0;
    return q;
}

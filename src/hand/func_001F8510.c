struct Q { int a, b, c; void *pvt; int pad[4]; void *p20; void *p24; int pad2[2]; };
struct E { char pad[0x94]; int n94; int pad2; int n9C; char pad3[0x100 - 0xA0]; };
struct P { char pad[0x50]; int c; struct E *ae; };
extern char D_002CA2C0[];
struct Pool;
extern struct Pool D_003E15CC;
struct Q *func_0011CCC0(struct Pool *);
void func_002333B4(void *, int, int);
/** @sly2-annot addr=0x1F8510
 * Purpose: Create a 0x30 handle object (pool D_003E15CC, vtable 0x2CA2C0 = CID 0x95) for the XF matching
 *   (oid,kind)
 * Info: proposed name PxfhNewSo (T2); TU so
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
struct Q *func_001F8510(struct P *p, int k, int m)
{
    int i;
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        if (pe->n94 == m && (k == -1 || pe->n9C == k)) {
            struct Q *q = func_0011CCC0(&D_003E15CC);
            func_002333B4(q, 0, 0x30);
            q->pvt = D_002CA2C0;
            q->p20 = p;
            q->p24 = pe;
            q->a = 0;
            q->b = 0;
            return q;
        }
    }
    return 0;
}

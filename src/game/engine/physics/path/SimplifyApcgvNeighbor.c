typedef int qword __attribute__((mode(TI)));
struct CTX { int a[4]; };
struct REQ { int cb; int align; };
void STACKSCOPE_ctor(struct CTX *c);
void STACKSCOPE_ctor_1AE4A0(struct CTX *c, int n);
void *func_001AE318(struct REQ *r);
void func_001AE5A0(void *dst, void *src, int cb);
int FClipEdgeToCbsp(void *ctx, qword a, qword b, int n);
static __inline__ void *PvAllocReq(struct REQ *preq, int cb, int nKind) { preq->cb = cb; preq->align = nKind; return func_001AE318(preq); }
/** @sly2-annot addr=0x1BFC18
 * Purpose: Drop intermediate path vertices with clear line of sight (FClipEdgeToCbsp)
 * Info: proposed name SimplifyApcgvNeighbor (T1); TU path
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920
 * @end-sly2-annot */
void SimplifyApcgvNeighbor(void *ctx, int *pc, qword **ap)
{
    struct CTX c;
    struct REQ req;
    qword **tmp, **prev, **cur, **next, **end;
    int n, k;
    STACKSCOPE_ctor(&c);
    n = *pc;
    end = ap + n;
    tmp = PvAllocReq(&req, n * 4, 0x10);
    tmp[0] = ap[0];
    k = 1;
    prev = ap;
    cur = ap + 1;
    next = ap + 2;
    while (next < end) {
        if (FClipEdgeToCbsp(ctx, **prev, **next, 0)) {
            tmp[k] = *cur;
            prev = cur;
            cur = next++;
            k++;
        } else {
            cur = next++;
        }
    }
    tmp[k] = ap[n - 1];
    k++;
    if (k < n && k >= 3) SimplifyApcgvNeighbor(ctx, &k, tmp);
    func_001AE5A0(ap, tmp, k * 4);
    *pc = k;
    STACKSCOPE_ctor_1AE4A0(&c, 2);
}

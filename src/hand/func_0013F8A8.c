struct ALLOCREQ { int cb; int cbAlign; };
struct A { int f0; int cMax; int c; int *a; };
void *func_001AE318(struct ALLOCREQ *);
void func_001AE5A0(void *, void *, int);
static inline int Max(int a, int b) { return a < b ? b : a; }
/** @sly2-annot addr=0x13F8A8
 * Purpose: Append c LO* to growable array (capacity doubles, min 32, PvAllocStack 0x1AE318 + CopyAb)
 * Info: proposed name CLoadCollectorArray_Add (T1); class CLoadCollectorArray; TU brx
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/world-sw.md#10-key-functions, docs/reference/functions/world-sw.md#brx-0x13e368-0x13ff70
 * @end-sly2-annot */
void func_0013F8A8(struct A *pa, int cAdd, int *aAdd) {
    if (pa->cMax < pa->c + cAdd) {
        int cMax = pa->cMax;
        struct ALLOCREQ req;
        int *a;
        do {
            cMax = Max(cMax * 2, 32);
        } while (cMax < pa->c + cAdd);
        req.cb = cMax * 4;
        req.cbAlign = 0x10;
        a = func_001AE318(&req);
        func_001AE5A0(a, pa->a, pa->c * 4);
        pa->cMax = cMax;
        pa->a = a;
    }
    func_001AE5A0(pa->a + pa->c, aAdd, cAdd * 4);
    pa->c += cAdd;
}

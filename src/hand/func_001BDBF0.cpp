typedef int qword __attribute__((mode(TI)));
struct CTX { int a[4]; CTX(); ~CTX(); };
struct AREQ { int cb; int align; AREQ(int c, int a) { cb = c; align = a; } };
struct P { char pad[0xE8]; int c; char pad1[0xF4 - 0xEC]; qword *a; };
extern "C" {
void *func_001AE318(const AREQ &r);
void func_002338C0(void *base, int n, int cb, int (*cmp)(const void *, const void *));
void func_001AE5A0(void *dst, const void *src, int cb);
int func_001BDBC8(const void *a, const void *b);
}
static inline void *Alloc(int cb) { return func_001AE318(AREQ(cb, 0x10)); }
static inline float CrossZ(qword a, qword b, qword c)
{
    qword d, e, r; float z;
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(d) : "j"(b), "j"(a));
    __asm__("vsub.xyzw %0, %1, %2" : "=j"(e) : "j"(c), "j"(b));
    __asm__("vopmula.xyz $ACC, %1, %2\n\tvopmsub.xyz %0, %2, %1" : "=j"(r) : "j"(d), "j"(e));
    __asm__("qmfc2.ni %0, %1\n\tpextuw %0, $0, %0" : "=r"(z) : "j"(r));
    return z;
}
/** @sly2-annot addr=0x1BDBF0
 * Purpose: 2D convex hull of points (sort + monotone chain)
 * Info: proposed name CposConvexHull (T2); TU tu_1BC050
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#tu_1bc050-0x1bc050-0x1be398
 * @end-sly2-annot */
extern "C" void func_001BDBF0(P *p, int n, qword *pts)
{
    int i, k, t;
    qword *hull, *pv;
    func_002338C0(pts, n, 0x10, func_001BDBC8);
    k = 2;
    {
        CTX ctx;
        hull = (qword *)Alloc(0x800);
        func_001AE5A0(hull, pts, 0x20);
        for (i = 2; i < n; i++) {
            hull[k++] = pts[i];
            while (k >= 3) {
                pv = &hull[k - 2];
                if (CrossZ(pv[-1], pv[0], pv[1]) < 0.0f) {
                    pv[0] = pv[1];
                    k--;
                } else
                    goto LBL;
            }
            LBL:;
        }
        t = k;
        hull[k++] = pts[n - 2];
        for (i = n - 3; i >= 0; i--) {
            hull[k++] = pts[i];
            while (t < k) {
                pv = &hull[k - 2];
                if (CrossZ(pv[-1], pv[0], pv[1]) < 0.0f) {
                    pv[0] = pv[1];
                    k--;
                } else
                    goto LBL2;
            }
            LBL2:;
        }
        { int cMax = 32; p->c = (k > cMax - 1) ? cMax : k; }
        func_001AE5A0(p->a, hull, p->c << 4);
    }
}

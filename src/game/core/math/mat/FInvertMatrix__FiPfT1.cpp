// Gauss-Jordan inverse with column pivoting. Sly 2 takes (stride c, size n, src, dst); the symbol name comes from Sly 1.
extern "C" {
void func_001AE478(void *p);
void func_001AE4A0(void *p, int n);
struct MSG { int cb; int nKind; };
void *func_001AE318(MSG *pmsg);
void func_001AE5A0(void *pvDst, void *pvSrc, int cb);
}
class FRAME { public: FRAME() { func_001AE478(this); } ~FRAME() { func_001AE4A0(this, 2); } int a[4]; };
static inline void *PvAllocMsg(MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE318(pmsg); }
static inline float GAbs(float g) { return __builtin_fabsf(g); }

/** @sly2-annot addr=0x11C500
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/core-library.md#mat-0x11b300-0x11cca8
 * @end-sly2-annot */
extern "C" int FInvertMatrix__FiPfT1(int c, int n, float *pgSrc, float *pgDst)
{
    int fRet = 0;
    FRAME frame;
    int i, j, k;
    int cb = n * c * 4;
    MSG msg;
    float *a = (float *)PvAllocMsg(&msg, cb, 0x10);
    float gMax, g, f, t;
    func_001AE5A0(a, pgSrc, cb);
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            pgDst[i * c + j] = (i == j) ? 1.0f : 0.0f;
    for (i = 0; i < n; i++) {
        j = i;
        gMax = GAbs(a[i * c + i]);
        for (k = i + 1; k < n; k++) {
            f = GAbs(a[i * c + k]);
            if (gMax < f) {
                gMax = f;
                j = k;
            }
        }
        if (gMax < 1e-4f)
            goto LDone;
        if (j != i) {
            for (k = 0; k < n; k++) {
                g = a[k * c + i]; a[k * c + i] = a[k * c + j]; a[k * c + j] = g;
                t = pgDst[k * c + i]; pgDst[k * c + i] = pgDst[k * c + j]; pgDst[k * c + j] = t;
            }
        }
        for (j = i + 1; j < n; j++) {
            f = a[i * c + j] / a[i * c + i];
            for (k = 0; k < n; k++) {
                a[k * c + j] -= f * a[k * c + i];
                pgDst[k * c + j] -= f * pgDst[k * c + i];
            }
        }
    }
    for (i = n - 1; i >= 0; i--) {
        for (j = 0; j < i; j++) {
            f = a[i * c + j] / a[i * c + i];
            for (k = 0; k < n; k++) {
                a[k * c + j] -= f * a[k * c + i];
                pgDst[k * c + j] -= f * pgDst[k * c + i];
            }
        }
    }
    for (i = 0; i < n; i++) {
        f = a[i * c + i];
        for (j = 0; j < n; j++)
            pgDst[j * c + i] /= f;
    }
    fRet = 1;
LDone:
    return fRet;
}

typedef unsigned char uchar;
struct JOY { char pad[0x1018]; float g1018; };
struct LM { float gMin; float gMax; };
struct VEC2 { float x; float y; };
extern VEC2 D_002BE950[4];
static inline float SqrtF(float g) { float r; __asm__ volatile("sqrt.s %0, %1" : "=&f"(r) : "f"(g)); return r; }
static inline float GClamp(float g, float lo, float hi)
{
    if (g < lo) return lo;
    if (g > hi) return hi;
    return g;
}
/** @sly2-annot addr=0x187768
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/main-loop-startup.md#joy-0x187030-0x188488
 * @end-sly2-annot */
void GetJoyXYDeflection(JOY *pjoy, uchar bX, uchar bY, float *px, float *py, float *pgDeflection,
                        uchar *pbXPrev, uchar *pbYPrev, int *pfActive, LM *alm)
{
    float x, y, gMax, g, r;
    unsigned int i;
    if (!*pfActive) {
        int fActive = 0;
        if (pjoy->g1018 != 0.0f &&
            (bX != *pbXPrev || bY != *pbYPrev || bX < 0x60 || bX > 0xA0 || bY < 0x60 || bY > 0xA0))
            fActive = 1;
        *pfActive = fActive;
    }
    if (*pfActive) {
        x = 2.0f * (float)bX / 255.0f + -1.0f;
        y = 1.0f - 2.0f * (float)bY / 255.0f;
    } else {
        x = y = 0.0f;
    }
    gMax = 0.0f;
    for (i = 0; i < 4; i++) {
        g = x * D_002BE950[i].x + y * D_002BE950[i].y;
        if (g >= 0.0f) {
            if (alm[i].gMax < g) alm[i].gMax = g;
            r = g / alm[i].gMax;
        } else {
            if (g < alm[i].gMin) alm[i].gMin = g;
            r = g / alm[i].gMin;
        }
        if (gMax < r) gMax = r;
    }
    r = GClamp((gMax - 0.35f) * 2.0f, 0.0f, 1.0f);
    *pgDeflection = r;
    g = SqrtF(x * x + y * y);
    {
        if (g > 0.0001f) {
            *px = r * x / g;
            *py = r * y / g;
        } else {
            *px = *py = 0.0f;
        }
    }
    *pbXPrev = bX;
    *pbYPrev = bY;
}

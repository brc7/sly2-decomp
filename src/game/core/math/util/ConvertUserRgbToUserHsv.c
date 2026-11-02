typedef struct VECTOR { float x, y, z; } VECTOR;
float func_00244F18(float g);

static __inline__ float GClamp(float g, float lo, float hi)
{
    if (g < lo) return lo;
    if (g > hi) return hi;
    return g;
}

/** @sly2-annot addr=0x11DD90
 * Purpose: RGB -> HSV (floorf), clamps
 * Info: proposed name ConvertUserRgbToUserHsv (T1); TU util
 * Notes: names_v2 C proposed ConvertHsvToRgb__FP6VECTORT0 (0.438)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/core-library.md#util-0x11cf60-0x11e198
 * @end-sly2-annot */
void ConvertUserRgbToUserHsv(VECTOR *phsv, VECTOR *prgb)
{
    float h, s, v, f, p, q, t;
    int i;

    h = (phsv->x >= 0.0f) ? GClamp(phsv->x, 0.0f, 360.0f) : -1.0f;
    s = GClamp(phsv->y, 0.0f, 1.0f);
    v = GClamp(phsv->z, 0.0f, 1.0f);

    if (s == 0.0f) {
        prgb->x = v;
        prgb->y = v;
        prgb->z = v;
        return;
    }

    if (h == 360.0f)
        h = 0.0f;
    h *= 1.0f / 60.0f;
    i = (int)func_00244F18(h);
    f = h - i;
    p = v * (1.0f - s);
    q = v * (1.0f - s * f);
    t = v * (1.0f - s * (1.0f - f));

    switch (i) {
    case 0:
    case 6:
        prgb->x = v; prgb->y = t; prgb->z = p;
        break;
    case 1:
        prgb->x = q; prgb->y = v; prgb->z = p;
        break;
    case 2:
        prgb->x = p; prgb->y = v; prgb->z = t;
        break;
    case 3:
        prgb->x = p; prgb->y = q; prgb->z = v;
        break;
    case 4:
        prgb->x = t; prgb->y = p; prgb->z = v;
        break;
    case 5:
        prgb->x = v; prgb->y = p; prgb->z = q;
        break;
    }
}

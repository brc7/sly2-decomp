extern float D_002C0AB8;
float SMaxShape(void *s);
int func_001BCF18(void *s, void *x, float t, float dt, float *pt, int k);
int func_001BCAC0(void *s, void *x, float a, float b, int k);

int func_001BCCE0(void *s, void *x, float *pt, int mode, float t)
{
    float at[2];
    float tB, d, f;
    int fShort, a, b;

    if (!func_001BCF18(s, x, t, SMaxShape(s) * 0.5f, &at[0], 0) &&
        !func_001BCF18(s, x, t, SMaxShape(s) * 0.5f, &at[0], 1)) return 4;
    t = at[0];
    if (!func_001BCF18(s, x, *pt, SMaxShape(s) * 0.5f, &at[1], 0) &&
        !func_001BCF18(s, x, *pt, SMaxShape(s) * 0.5f, &at[1], 1)) return 4;
    tB = at[1];
    *pt = tB;
    d = tB - t;
    if (d < 0.0f) d += SMaxShape(s);
    switch (mode) {
    case 2: f = D_002C0AB8 + 1.0f; break;
    case 1: f = 1.0f - D_002C0AB8; break;
    default: f = 1.0f; break;
    }
    fShort = (d < SMaxShape(s) * f * 0.5f) ? 1 : 0;
    a = func_001BCAC0(s, x, t, tB, 1);
    b = func_001BCAC0(s, x, t, tB, 0);
    if (b < a) return 1;
    if (a < b) return 2;
    if (a != 2) return fShort ? 2 : 1;
    return 4;
}

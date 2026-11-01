typedef int qword __attribute__((mode(TI)));
typedef struct { qword q[4]; } M4;
typedef union { qword q; float f[4]; } VU;
struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
extern float D_002A4B30[];
struct RANGE { float a; float b; };
extern struct RANGE D_002BBB60;
struct P { char pad[0x170]; VU v170; float x180; int x184; float x188; };
void RenderSoSelf(struct P *);
qword func_001A62E8(void *);
void *func_001A6318(void *);
M4 LoadMatrixFromPosRotScale(qword, void *, VU *);
void func_001A5EE8(struct P *, M4 *, float, float);

static inline float GClamp(float g, float gMin, float gMax)
{
    float r;
    if (g < gMin)
        r = gMin;
    else if (gMax < g)
        r = gMax;
    else
        r = g;
    return r;
}

/** @sly2-annot addr=0x13C000
 * Purpose: cid 0x67 vt19: when carried, scale-pop animation (CLQ over time) and draw at the player; in states
 *   1/5 draw normally
 * Info: proposed name RenderBootySelf (T1); class BOOTY slot19; TU booty
 * Notes: names_v2 v1 proposed PostAlarmLoadCallbackHookup__FP5ALARM5MSGIDPv (0.168)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r2b_rest
 * See: docs/reference/functions/game-progression.md#booty-0x13bdf0-0x13d558
 * @end-sly2-annot */
void RenderBootySelf(struct P *p)
{
    switch (p->x184) {
    case 4: {
        VU v;
        M4 m;
        M4 mT;
        float dt = D_002DDED0.t - p->x188;
        if (p->x180 <= dt) {
            v = p->v170;
        } else {
            float u = dt / p->x180;
            float g = GClamp(D_002A4B30[0] + u * (D_002A4B30[1] + u * D_002A4B30[2]), D_002BBB60.a, D_002BBB60.b);
            int i;
            for (i = 0; i < 3; i++)
                v.f[i] = (1.0f - g) + g * p->v170.f[i];
        }
        mT = LoadMatrixFromPosRotScale(func_001A62E8(p), func_001A6318(p), &v);
        m = mT;
        func_001A5EE8(p, &m, 1.0f, 1.0f);
        break;
    }
    case 1:
    case 5:
        RenderSoSelf(p);
        break;
    case 2:
    case 3:
        break;
    }
}

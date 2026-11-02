extern struct CLOCK { float t; float dt; } D_002DDED0;
typedef struct { float a; float b; float c; float max; float u; } SEG;
typedef struct {
    int pad0;
    int kind;
    char pad8[0x14 - 0x8];
    float x14;
    int c;
    SEG aseg[6];
    int i;
    char pad98[0xA0 - 0x98];
} TRK;
typedef struct { TRK atrk[2]; float x140; float x144; void *x148; char pad14c[4]; } ENT;
typedef struct { char pad[0x70]; unsigned long fl; } W;
typedef struct OBJ {
    char pad[0xEC];
    W *pw;
    char padF0[0x198 - 0xF0];
    int c;
    ENT *ae;
    char pad1a0[0x1C0 - 0x1A0];
    float speed;
} OBJ;
typedef struct { int a[8]; } VPAIR;
extern char D_003E11C4[];
float fmodf(float, float);
void AppendDlEntry__FP2DLPv(void *, W *);
void GetLasenBeamEnds(OBJ *, ENT *, VPAIR *, float *);
/** @sly2-annot addr=0x1DF3D0
 * Purpose: Advance path-driven endpoints along their segment profiles at speed(+0x1C0)*dt (fmodf wrap), mark
 *   the tree dirty, recompute length
 * Info: proposed name MoveLasenEndpoints (T2); TU sensor
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/guards-ai.md#5-perception,
 *   docs/reference/functions/gameplay-objects.md#sensor-0x1de2a0-0x1dfad8
 * @end-sly2-annot */
void func_001DF3D0(OBJ *p)
{
    int i;
    for (i = 0; i < p->c; i++) {
        ENT *e = &p->ae[i];
        int f = 0;
        unsigned int j;
        for (j = 0; j < 2; j++) {
            TRK *q = &e->atrk[j];
            SEG *ps;
            float u;
            if (q->kind != 1)
                continue;
            ps = &q->aseg[q->i];
            ps->u = u = ps->u + p->speed * D_002DDED0.dt;
            if (ps->max < u) {
                float r = fmodf(u, ps->max);
                q->i = (q->i + 1) % q->c;
                ps = &q->aseg[q->i];
                ps->u = r;
            }
            q->x14 = ps->a + ps->b * ps->u + ps->c * ps->u * ps->u * 0.5f;
            if (!(p->pw->fl & 2)) {
                p->pw->fl |= 2;
                AppendDlEntry__FP2DLPv(D_003E11C4, p->pw);
            }
            f = 1;
        }
        if (f && e->x148 == 0) {
            VPAIR v;
            float g;
            GetLasenBeamEnds(p, e, &v, &g);
            e->x140 = e->x144 = g;
        }
    }
}

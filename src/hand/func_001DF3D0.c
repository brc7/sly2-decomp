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
float func_00245A18(float, float);
void AppendDlEntry__FP2DLPv(void *, W *);
void func_001DEF98(OBJ *, ENT *, VPAIR *, float *);
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
                float r = func_00245A18(u, ps->max);
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
            func_001DEF98(p, e, &v, &g);
            e->x140 = e->x144 = g;
        }
    }
}

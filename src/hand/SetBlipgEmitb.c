// MATCH (Claude agent, laneC10 2026-10-23): from v4; p->g120 stored in each branch of the if/else (gcc cross-jumps the two stores into one join block), which lets the g8C load schedule first. One commented register pin (r in InitS).
typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
int func_0011D4D0(float a, float b, float eps);
static __inline__ int FVecsNear(qword a, qword b)
{
    return func_0011D4D0(GetX(a), GetX(b), 1e-4f) && func_0011D4D0(GetY(a), GetY(b), 1e-4f) &&
           func_0011D4D0(GetZ(a), GetZ(b), 1e-4f);
}
extern qword D_002A2DD0;

struct CLR { int c; unsigned char ab[1][4]; };
struct POLY { float a, b, c; };
struct Q {
    char pad0[0x58]; float g58; int pad5C;
    qword v60;
    char pad70[0x80 - 0x70]; float g80; int pad84; int pad88; float g8C;
    char pad90[0xB0 - 0x90]; char abB0[0x10];
    int xC0; int xC4; int xC8; int xCC; void *xD0; int xD4; float gD8; struct CLR *pclrDC;
    char abE0[0x10]; float gF0;
};
struct P {
    char pad0[0x110]; struct POLY poly; int pad11C; float g120; char pad124[0x130 - 0x124];
    char ab130[0x10]; char ab140[4]; float g144; char pad148[8];
    struct S150 { int x0; float g4; int x8; } s150; int pad15C; int x160; int pad164[3]; char ab170[0x30];
    char pad1A0[0x1A0 - 0x1A0];
};
struct P2 { char pad[0x1A0]; int n1A0; char pad1A4[0x3B0 - 0x1A4]; int c3B0; unsigned char ab[1][4]; };
struct P3 { char pad[0x434]; int x434; int x438; int x43C; float g440; };

void PropagateBlipgShader(void *p, void *pv);
void *func_002333B4(void *pv, int b, int cb);
static __inline__ float GEval(struct POLY *pp, float t) { return pp->a + t * (pp->b + t * pp->c); }
static __inline__ void InitS(float g, struct S150 *ps) { register float r asm("$f1") = 1.0f; /* register pin (Claude agent): f0/f1 tie */ ps->x0 = 0; ps->x8 = 0; ps->g4 = r / g; }
static __inline__ float GMax3(float a, float b, float c) { if (b < a && c < a) return a; if (c < b) return b; return c; }
void func_0011D5B0(float g, float h, void *pa, void *pb);
void SetEmitdvEmitb__FP6EMITDVP5EMITB(void *p, struct Q *q);

void SetBlipgEmitb(struct P *p, struct Q *q)
{
    struct CLR *pclr;
    float g;
    int f;
    int i, j, k;

    k = q->xC0;
    g = 1.0f;
    ((struct P3 *)p)->x43C = k;
    if (k == 1)
        g = q->gF0;
    ((struct P3 *)p)->g440 = g;
    pclr = q->pclrDC;
    if (pclr) {
        struct P2 *p2 = (struct P2 *)p;
        p2->c3B0 = pclr->c;
        for (i = 0; i < p2->c3B0; i++)
        {
            int iDst = p2->c3B0 - i - 1;
            for (j = 0; j < 4; j++)
                p2->ab[iDst][j] = (pclr->ab[i][j] + 1) >> 1;
        }
        ((struct P3 *)p)->x434 = q->xCC;
    }
    PropagateBlipgShader(p, q->xD0);
    func_002333B4(p->ab140, 0, 0x10);
    if (((struct P2 *)p)->n1A0 >= 2) {
        int n = ((struct P2 *)p)->n1A0;
        if (q->gD8 != 0.0f)
            p->g144 = (float)n / q->gD8;
        else
            p->g144 = (float)n / q->g8C;
    }
    func_0011D5B0(1.0f / q->g8C, 0.0f, q->abB0, p->ab130);
    func_0011D5B0(1.0f / q->g8C, 0.0f, q->abE0, &p->poly);
    if (p->poly.b == 0.0f && p->poly.c == 0.0f) {
        p->g120 = __builtin_fabsf(p->poly.a);
    } else {
        float ag[3];
        float t = q->g8C;
        float a0, a1, a2;
        ag[0] = p->poly.a;
        ag[1] = 0.0f;
        ag[2] = GEval(&p->poly, t);
        if (p->poly.c != ag[1]) {
            float tm = -p->poly.b / (p->poly.c + p->poly.c);
            if (ag[1] < tm && tm < t)
                ag[1] = GEval(&p->poly, tm);
        }
        p->g120 = GMax3(__builtin_fabsf(ag[0]), __builtin_fabsf(ag[1]), __builtin_fabsf(ag[2]));
    }
    f = 0;
    g = q->g8C;
    InitS(g, &p->s150);
    {
    int kC4 = q->xC4;
    p->x160 = kC4;
    if (!kC4) {
        int fNear = FVecsNear(D_002A2DD0, q->v60);
        if (fNear && q->g58 == 0.0f && q->g80 == 0.0f)
            f = 1;
    }
    }
    if (f)
        p->x160 = 1;
    func_002333B4(p->ab170, 0, 0x30);
    if (!p->x160)
        SetEmitdvEmitb__FP6EMITDVP5EMITB(p->ab170, q);
}

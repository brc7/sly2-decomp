typedef int qword __attribute__((mode(TI)));
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
int func_0011D4D0(float a, float b, float eps);
extern qword D_002A2DD0;
static __inline__ int FVecsNear(qword a, qword b)
{
    return func_0011D4D0(GetX(a), GetX(b), 1e-4f) && func_0011D4D0(GetY(a), GetY(b), 1e-4f) &&
           func_0011D4D0(GetZ(a), GetZ(b), 1e-4f);
}
typedef struct VECTOR3 { float x, y, z; } VECTOR3;
typedef union V3Q { VECTOR3 v; qword q; } V3Q;
typedef struct KEY { VECTOR3 pos; float t; qword dpos; } KEY;
typedef struct T { int pad; int c; KEY *a; } T;
typedef struct OBJ {
    char pad[0x80];
    float t0;
    float t1;
    float *pt;
    char pad8C[0x9A - 0x8C];
    short s9A;
    char pad9C[0xC0 - 0x9C];
    V3Q pos0;
    qword dpos0;
    V3Q pos1;
    qword dpos1;
} OBJ;
int func_0012BB70(T *t, float g);
/** @sly2-annot addr=0x12BBD0
 * Purpose: Evaluate at t
 * Info: proposed name EvaluateAcVecB (T2); class AcVecB; TU act
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/animation.md#act-0x12b3b0-0x12e600
 * @end-sly2-annot */
void func_0012BBD0(T *t, OBJ *po)
{
    int i = func_0012BB70(t, *po->pt);
    int f;
    po->t0 = t->a[i - 1].t;
    po->pos0.v = t->a[i - 1].pos;
    po->dpos0 = t->a[i - 1].dpos;
    po->t1 = t->a[i].t;
    po->pos1.v = t->a[i].pos;
    po->dpos1 = t->a[i].dpos;
    f = FVecsNear(po->pos0.q, po->pos1.q) && FVecsNear(D_002A2DD0, po->dpos0) &&
            FVecsNear(D_002A2DD0, po->dpos1);
    po->s9A = f ? 2 : 1;
}

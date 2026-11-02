// Query the vertical hit span and optional point and normal outputs.
typedef int qword __attribute__((mode(TI)));
typedef struct MSG { int cb; int cbAlign; } MSG;
struct MARK {};
struct LSG { char pad[0x10]; qword pos; char pad1[0x10]; qword normal; float g40; float g44; char pad2[0x18]; };
struct X { char pad[8]; float g8; char pad1[0xC]; float g18; };
struct J { char pad[0xEC]; struct X *px; char pad1[0x208 - 0xF0]; int x208; char pad2[0x238 - 0x20C]; float g238; char pad3[0xC]; float g248; };
struct S { char pad[0xE0]; int c; };
extern qword D_002A2DC0;
extern qword D_002A2DD0;
extern "C" {
MARK *STACKSCOPE_ctor(MARK *pm);
void STACKSCOPE_ctor_1AE4A0(MARK *pm, int n);
void *func_001AE318(MSG *pmsg);
qword func_001A62E8(void *p);
int FIsBasicDerivedFrom__FP5BASIC3CID(struct J *pj, int k);
int func_00229D60(void *,void *);
int func_001F7E08(struct S *p,int (*pfn)(void *,void *),void *context,void **apv);
int ClsgClipEdgeToObjects(qword a, qword b, int cso, void **apso, int cMax, struct LSG *alsg);
int FFloatsNear__Ffff(float a, float b, float eps);
}
static __inline__ float GetX(qword q) { float x; __asm__("mtc1 %1,%0" : "=f"(x) : "r"(q)); return x; }
static __inline__ float GetY(qword q) { float y; __asm__("prot3w %0,%1" : "=r"(y) : "r"(q)); return y; }
static __inline__ float GetZ(qword q) { float z; __asm__("pextuw %0,$0,%1" : "=r"(z) : "r"(q)); return z; }
static inline float Epsilon(){register float f __asm__("$f14");__asm__ __volatile__("li.s %0,%1":"=f"(f):"F"(1e-4f));return f;}
static inline int WordY(qword q){int r;__asm__("prot3w %0,%1":"=r"(r):"r"(q));return r;}
static inline int WordZ(qword q){int r;__asm__("pextuw %0,$0,%1":"=r"(r):"r"(q));return r;}
static inline float FloatWord(int word){float f;__asm__("mtc1 %1,%0":"=f"(f):"r"(word));return f;}
static inline int NearX(qword d,qword n,float e){return FFloatsNear__Ffff(GetX(d),GetX(n),e);}
static inline int NearY(qword d,qword n){int y=WordY(d);float e=Epsilon();return FFloatsNear__Ffff(FloatWord(y),GetY(n),e);}
static inline int NearZ(qword d,qword n){int z=WordZ(d);float e=Epsilon();return FFloatsNear__Ffff(FloatWord(z),GetZ(n),e);}
union VECTOR {qword q;float f[4];};
struct WORK {
 VECTOR a,b;
 MARK mark __attribute__((aligned(16)));
 MSG msg __attribute__((aligned(16)));
 LSG alsg[0x10];
};
/** @sly2-annot addr=0x229468
 * Purpose: vertical hit span and optional surface point/normal
 * Info: proposed name UGetWaterSubmerged (T2); TU water
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/physics-collision.md#water-0x228b48-0x229ef0
 * @end-sly2-annot */
extern "C" float func_00229468(struct S *p, struct J *pj, qword *ppos, qword *pnormal)
{
    WORK work;
    void **apv;
    int c;
    float g;
    work.a.q = work.b.q = func_001A62E8(pj);
    if (FIsBasicDerivedFrom__FP5BASIC3CID(pj, 0x33)) {
        work.a.f[2] = work.a.f[2] + pj->g248;
        if (pj->x208)
            work.a.f[2] = work.a.f[2] > pj->g238 ? work.a.f[2] : pj->g238;
    } else {
        work.a.f[2] = pj->px->g8;
    }
    work.b.f[2] = pj->px->g18;
    STACKSCOPE_ctor(&work.mark);
    {
        MSG *pm = &work.msg;
        pm->cb = p->c * 4;
        pm->cbAlign = 0x10;
        apv = (void **)func_001AE318(pm);
    }
    c = func_001F7E08(p, func_00229D60, 0, apv);
    if (!ClsgClipEdgeToObjects(work.a.q, work.b.q, c, apv, 0x10, work.alsg)) {
        if (pnormal)
            *pnormal = D_002A2DC0;
        STACKSCOPE_ctor_1AE4A0(&work.mark, 2);
        return 0.0f;
    }
    if (ppos)
        *ppos = work.alsg[0].pos;
    qword n = work.alsg[0].normal;
    if (pnormal) {
        qword d;
        qword *comparison=&D_002A2DD0;
        float epsilon=Epsilon();
        int f;
        *pnormal = n;
        d = *comparison;
        f = NearX(d,n,epsilon) && NearY(d,n) && NearZ(d,n);
        if (f)
            *pnormal = D_002A2DC0;
    }
    g = work.alsg[0].g44 - work.alsg[0].g40;
    STACKSCOPE_ctor_1AE4A0(&work.mark, 2);
    return g;
}

typedef int qword __attribute__((mode(TI)));
struct E { int x0; char pad[0x8C]; };
struct T { char pad[0x10C]; void *x10C; int x110; struct E *ae; char pad1[0x12C - 0x118]; int x12C; char pad2[0x140 - 0x130];
    char x140[0x184 - 0x140]; int x184; float g188; };
struct R { char pad[0x5C]; float g5C; };
struct S { char pad[0x10]; struct T *pt; void *pe; float g18; float g1C; float g20; float g24; char pad1[0x34 - 0x28]; int x34; int x38;
    int x3C; char pad2[0x48 - 0x40]; float g48; char pad3[0x60 - 0x4C]; float g60; char pad4[0x70 - 0x64]; qword pos; float g80; };
struct G { char pad[0x24]; float g24; char pad1[0x4C - 0x28]; float g4C; char pad2[0x90 - 0x50]; qword q90; char pad3[0x2E0 - 0xA0]; qword q2E0; };
extern struct G D_002DDF40;
extern char D_002C4120[];
extern float D_002C41DC;
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
void func_0014FCC8(struct S *p, void *pj, struct T *pt);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *pj, int k);
int FIsCidDerivedFrom(int a, int b);
void func_00220D40(void *pe);
void func_0014E968(struct G *pg, void *pj, void *pq);
void func_00220DD8(void *pe, void *pa, qword *pb, struct T *pt);
void FindShapeClosestPoint(void *p, qword q, int a, qword *pq, int b, int c, void *pv);
void func_00221CE8(struct S *p, void *pq);
void GetVectorSphere(qword v, float *pa, float *pb, float *pc);
qword SetVectorSphere(float a, float b, float c);
void func_0014D8C0(struct G *pg);
static __inline__ qword VAdd(qword a, qword b) { qword r; __asm__("vadd.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
static __inline__ qword VSub(qword a, qword b) { qword r; __asm__("vsub.xyzw %0, %1, %2" : "=j"(r) : "j"(a), "j"(b)); return r; }
/** @sly2-annot addr=0x221A18
 * Purpose: base 0x14FCC8; initial focus/angles (0x220D40, 0x220DD8, 0x221CE8)
 * Info: proposed name ActivateCptn (T1); TU tn
 * Notes: agent qualifier: CPTN vt slot2
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
void ActivateCptn(struct S *p, void *pj, struct T *pt)
{
    struct { qword a[5]; } q0;
    qword q50;
    qword q60;
    qword a;
    func_0014FCC8(p, pj, pt);
    p->pt = pt;
    if (!pt) {
        p->pe = D_002C4120;
    } else {
        struct E *pe = pt->ae;
        int i;
        for (i = 1; i < p->pt->x110; i++) {
            struct E *peT = &p->pt->ae[i];
            if (FIsBasicDerivedFrom__FP5BASIC3CID(pj, peT->x0) && FIsCidDerivedFrom(peT->x0, pe->x0))
                pe = peT;
        }
        p->pe = pe;
    }
    p->x38 = 1;
    func_00220D40(p->pe);
    func_0014E968(&D_002DDF40, pj, &q0);
    func_00220DD8(p->pe, &q0, &q50, 0);
    if (p->pt) {
        struct T *ptT = p->pt;
        if (ptT->x10C)
            FindShapeClosestPoint(ptT->x10C, q50, 0, &q60, 0, 0, ptT->x140);
        else if (ptT->x12C)
            func_00221CE8(p, &q0);
        func_00220DD8(p->pe, &q0, &q50, p->pt);
    }
    { qword *pq = &D_002DDF40.q2E0; a = VAdd(q50, *pq);
    GetVectorSphere(VSub(pq[-0x25], a), &p->g18, &p->g1C, &p->g20); }
    p->g20 = p->g20 - D_002DDF40.g4C;
    p->pos = VAdd(a, SetVectorSphere(p->g18, p->g1C, p->g20));
    p->g24 = ((struct R *)p->pe)->g5C;
    p->x34 = 0;
    if (p->pt && p->pt->x184)
        func_0014D8C0(&D_002DDF40);
    p->g80 = D_002DDED0.t;
    p->g60 = D_002DDED0.t;
    p->g48 = D_002DDED0.t - D_002C41DC;
    p->x3C = 0;
    if (p->pt && p->pt->g188 != 0.0f)
        D_002DDF40.g24 = p->pt->g188;
}

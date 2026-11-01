typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f[4];} U;
typedef struct {qword row[4];} M4;
struct LO;
struct PROJECTION {qword direction;int kind;};
typedef struct { short d; short pad; void (*pfn)(void *, int, void *, qword, qword, qword); } VT82;
typedef struct { short d; short pad; qword (*pfn)(void *); } VT33;
typedef union { VT82 v82; VT33 v33; } VTE;
struct LO { char pad[0xC]; VTE *vt; char pad2[0x190 - 0x10]; qword x190; char pad3[0x218 - 0x1A0]; qword *x218; };
struct T { char pad[0xE4]; int xe4; char pad2[0x1BC - 0xE8]; int x1bc; };
struct S { struct LO *x0; int x4; int x8; int xc; int x10; struct T *x14; int x18; float x1c; float x20; };
extern qword D_002A2DB0;
qword func_001A62E8(void *p);
M4 *func_001A6318(struct LO *p);
void func_001E6ED8(struct T *t, qword a, struct PROJECTION *projection, qword *p0, qword *p1, float *pf0, float *pf1, float g);
float func_001E6620(struct T *t);
float func_001E6648(struct T *t);
void func_001E6F10(struct T *t, float g, qword *point, qword *direction);
static __inline__ float Dot(qword a, qword b)
{
    qword d, t;
    float r;
    __asm__("vmul.xyz %0, %2, %0\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(t) : "j"(a), "0"(b));
    __asm__("qmfc2.ni %0, %1" : "=r"(r) : "j"(d));
    return r;
}
static __inline__ float Dot2(qword a, qword b)
{
    qword d, t;
    float r;int bits;
    __asm__("vmul.xyz %0, %0, %2\n\tvaddw.x %1, $vf0, $vf0w\n\tvadday.x $ACC, %0, %0y\n\tvmaddz.x %0, %1, %0z" : "=j"(d), "=&j"(t) : "j"(b), "0"(a));
    __asm__("qmfc2.ni %0,%2\n\tmtc1 %0,%1":"=&r"(bits),"=f"(r):"j"(d));
    return r;
}
/** @sly2-annot addr=0x193DE8
 * Purpose: Add contact constraints for the held object: state 4 -> vt82(0x23, plo, pos), state 3 rail/edge ->
 *   constraint along curve from velocity
 * Info: proposed name UpdateJtInternalXpsHide (T1); TU grab
 * Notes: names_v2 A proposed UpdateJtInternalXpsHide__FP2JT (0.995) [verified CORRECT] | CONFLICT: verified
 *   tier-A name differs from TU name; orchestrator: verified name UpdateJtInternalXpsHide__FP2JT restored
 *   over AddJtGrabXps (verified > unverified) | arbitration adopt verified: Verifier matched Sly1 body; arg
 *   is the hide substate (reads +0x8 first), keep unmangled.
 * Confidence: mid (draft) [T1 0.70] - evidence: Sly 1 / Lilly reference; arbitration; static naming pass;
 *   sources: r4b_rest+verdicts_A2;verified_A+arbitration
 * See: docs/engine/player.md#11-key-functions,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#grab-0x191090-0x195950
 * @end-sly2-annot */
void func_00193DE8(struct S *p)
{
    switch (p->x8) {
    case 4: {
        U pos;
        pos.q = func_001A62E8(p->x14);
        p->x0->vt[82].v82.pfn((char *)p->x0 + p->x0->vt[82].v82.d, 0x23, p->x14, pos.q, D_002A2DB0, *p->x0->x218);
        break;
    }
    case 3: {
        U v0, v1;
        struct T *t = p->x14;
        int k;
        func_001E6ED8(t, *p->x0->x218, 0, &v0.q, &v1.q, &p->x1c, &p->x20, p->x1c);
        k = 0x22;
        if (!t->x1bc && !t->xe4) {
            U vel;
            M4 *m;
            register qword w __asm__("vf2");
            float d0, d1;
            vel.q = p->x0->vt[33].v33.pfn((char *)p->x0 + p->x0->vt[33].v33.d);
            d0 = Dot(vel.q, v1.q);
            m = func_001A6318(p->x0);
            w=p->x0->x190;
            __asm__("vmulax.xyz $ACC,%1,%0x\n\tvmadday.xyz $ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z":"+j"(w):"j"(m->row[0]),"j"(m->row[1]),"j"(m->row[2]));
            d1 = -Dot2(w, v1.q);
            if ((d0 < -2.0f || d1 < 2.0f) && p->x20 < func_001E6620(t) + 50.0f) {
                k = 0x2A;
                func_001E6F10(t, func_001E6620(t) + 50.0f, &v0.q, 0);
            } else if ((2.0f < d0 || -2.0f < d1) && func_001E6648(t) - 50.0f < p->x20) {
                U s;
                k = 0x2A;
                func_001E6F10(t, func_001E6648(t) - 50.0f, &v0.q, 0);
                __asm__("qmtc2.ni %1, %0" : "=j"(s.q) : "r"(-1.0f));
                __asm__("vmulx.xyzw %0, %1, %2x" : "=j"(v1.q) : "j"(v1.q), "j"(s.q));
            }
        }
        p->x0->vt[82].v82.pfn((char *)p->x0 + p->x0->vt[82].v82.d, k, t, v0.q, v1.q, *p->x0->x218);
        break;
    }
    }
}

typedef char offset_0_vt[(unsigned)&((struct LO*)0)->vt==12?1:-1];
typedef char offset_0_x190[(unsigned)&((struct LO*)0)->x190==400?1:-1];
typedef char offset_0_x218[(unsigned)&((struct LO*)0)->x218==536?1:-1];
typedef char offset_1_xe4[(unsigned)&((struct T*)0)->xe4==228?1:-1];
typedef char offset_1_x1bc[(unsigned)&((struct T*)0)->x1bc==444?1:-1];
typedef char offset_2_x0[(unsigned)&((struct S*)0)->x0==0?1:-1];
typedef char offset_2_x8[(unsigned)&((struct S*)0)->x8==8?1:-1];
typedef char offset_2_x14[(unsigned)&((struct S*)0)->x14==20?1:-1];
typedef char offset_2_x1c[(unsigned)&((struct S*)0)->x1c==28?1:-1];
typedef char offset_2_x20[(unsigned)&((struct S*)0)->x20==32?1:-1];
typedef char offset_3_direction[(unsigned)&((struct PROJECTION*)0)->direction==0?1:-1];
typedef char offset_3_kind[(unsigned)&((struct PROJECTION*)0)->kind==16?1:-1];
typedef char offset_4_d[(unsigned)&((VT82*)0)->d==0?1:-1];
typedef char offset_4_pfn[(unsigned)&((VT82*)0)->pfn==4?1:-1];
typedef char offset_5_d[(unsigned)&((VT33*)0)->d==0?1:-1];
typedef char offset_5_pfn[(unsigned)&((VT33*)0)->pfn==4?1:-1];
typedef char offset_6_row[(unsigned)&((M4*)0)->row==0?1:-1];
typedef char size_0[sizeof(qword)==16?1:-1];
typedef char size_1[sizeof(U)==16?1:-1];
typedef char size_2[sizeof(M4)==64?1:-1];
typedef char size_3[sizeof(VTE)==8?1:-1];
typedef char size_4[sizeof(VT82)==8?1:-1];
typedef char size_5[sizeof(VT33)==8?1:-1];
typedef char size_6[sizeof(struct PROJECTION)==32?1:-1];
typedef char size_7[sizeof(struct LO)==544?1:-1];
typedef char size_8[sizeof(struct T)==448?1:-1];
typedef char size_9[sizeof(struct S)==36?1:-1];
typedef char size_10[sizeof(int)==4?1:-1];
typedef char size_11[sizeof(float)==4?1:-1];
typedef char size_12[sizeof(short)==2?1:-1];
typedef char size_13[sizeof(void*)==4?1:-1];
typedef char alignment_0[__alignof__(U)==16?1:-1];
typedef char alignment_1[__alignof__(qword)==16?1:-1];
typedef char alignment_2[__alignof__(M4)==16?1:-1];
typedef char alignment_3[__alignof__(VTE)==4?1:-1];
typedef char alignment_4[__alignof__(struct PROJECTION)==16?1:-1];
typedef char alignment_5[__alignof__(struct S)==4?1:-1];

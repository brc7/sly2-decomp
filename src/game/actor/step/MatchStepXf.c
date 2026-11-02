// Set the parent matrix, position and inherited motion.
typedef int qword __attribute__((mode(TI)));
typedef union U { qword q; float f; } U;
typedef union {float value;int word;} FLOATWORD;
typedef struct M4 { qword row[4]; } M4;
typedef struct {short delta,unused; void (*fn)();} VTE;
typedef struct B {int a,b,c; VTE *vt;} B;
typedef struct P {
    B base;
    char pad[0x208 - 0x10]; int count;
    char pad2[0x240 - 0x20C]; qword offset;
    char pad3[0x1048 - 0x250]; FLOATWORD amount;
    char pad4[0x1060 - 0x104C]; qword adjusted;
} P;
extern qword D_002A2DD0, D_002A2DC0;
extern int FIsBasicDerivedFrom__FP5BASIC3CID(B *, int);
extern void func_001F49C8(P *, B *);
static inline qword Transform(const M4*m,qword v,qword last,qword*published){
 register qword result __asm__("vf7"),argument __asm__("$5"),origin __asm__("vf1")=last;
 __asm__ volatile("vmulax.xyz $ACC,%3,%7x\n\tvmadday.xyz $ACC,%4,%7y\n\tvmaddaz.xyz $ACC,%5,%7z\n\tvmaddw.xyz %0,%6,$vf0w\n\tqmfc2.ni %1,%0\n\tsqc2 %6,%2"
  :"=&j"(result),"=&r"(argument),"=m"(*published):"j"(m->row[0]),"j"(m->row[1]),"j"(m->row[2]),"j"(origin),"j"(v));
 return argument;
}
static inline qword Broadcast(float f){register qword r __asm__("vf6");__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(f));return r;}
static inline qword MulX(qword v,qword s){register qword r __asm__("vf2");__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(v),"j"(s));return r;}
/** @sly2-annot addr=0x2052E0
 * Purpose: STEP vt38: copy pose of another object (vt31 matrix) incl. offset -+0x240
 *   (TranslateToPos/RotateToMat), reset velocities; else SO::MatchXf
 * Info: proposed name MatchStepXf (T1); class STEP slot38; TU step
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7b_guard+verdicts_followups
 * See: docs/engine/guards-ai.md#9-key-functions, docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
extern void MatchStepXf(P *p, B *other)
{
    if (!other) return;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(other, 3)) {
        M4 m;
        U scale;
        register qword v __asm__("vf2");
        register VTE *set __asm__("a2");
        VTE *get = &other->vt[31];
        const M4 *source = ((M4 *(*)(B *))get->fn)((B *)((char *)other+get->delta));
        m.row[0] = source->row[0];
        set = p->base.vt;
        {qword sc = Broadcast(-1.0f);
        m.row[1] = source->row[1];
        v = MulX(p->offset, sc);
        m.row[2] = source->row[2];
        {qword t = Transform(&m,v,source->row[3],&m.row[3]);
        P *self = (P *)((char *)p+set[29].delta);
        register void (*positionFn)(P*,qword) __asm__("v1")=(void(*)(P*,qword))set[29].fn;
        scale.q = sc;
        positionFn(self, t);}}
        { VTE *s=p->base.vt; ((void (*)(P *,const M4 *))s[30].fn)((P *)((char *)p+s[30].delta), &m); }
        if (FIsBasicDerivedFrom__FP5BASIC3CID(&p->base,0x34)) {
            qword r;
            __asm__("qmtc2.ni %1,%0" : "=j"(scale.q) : "r"(p->amount.word));
            r = m.row[3];
            __asm__("vadda.xyzw $ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x" : "+j"(r) : "j"(D_002A2DC0), "j"(scale.q));
            p->adjusted = r;
        }
        { VTE *s=p->base.vt; ((void (*)(P *,qword))s[35].fn)((P *)((char *)p+s[35].delta), D_002A2DD0); }
        { VTE *s=p->base.vt; ((void (*)(P *,qword))s[36].fn)((P *)((char *)p+s[36].delta), D_002A2DD0); }
        p->count = 0;
    } else {
        func_001F49C8(p,other);
    }
}

typedef char layout_qword[(sizeof(qword)==16)?1:-1];
typedef char layout_scalar[(sizeof(U)==16)?1:-1];
typedef char layout_word[(sizeof(FLOATWORD)==4)?1:-1];
typedef char layout_word_value[((unsigned)&((FLOATWORD*)0)->value==0)?1:-1];
typedef char layout_word_bits[((unsigned)&((FLOATWORD*)0)->word==0)?1:-1];
typedef char layout_matrix[(sizeof(M4)==0x40)?1:-1];
typedef char layout_matrix_0[((unsigned)&((M4*)0)->row[0]==0)?1:-1];
typedef char layout_matrix_1[((unsigned)&((M4*)0)->row[1]==0x10)?1:-1];
typedef char layout_matrix_2[((unsigned)&((M4*)0)->row[2]==0x20)?1:-1];
typedef char layout_matrix_3[((unsigned)&((M4*)0)->row[3]==0x30)?1:-1];
typedef char layout_vte[(sizeof(VTE)==8)?1:-1];
typedef char layout_delta[((unsigned)&((VTE*)0)->delta==0)?1:-1];
typedef char layout_function[((unsigned)&((VTE*)0)->fn==4)?1:-1];
typedef char layout_base[(sizeof(B)==0x10)?1:-1];
typedef char layout_vtable[((unsigned)&((B*)0)->vt==0xC)?1:-1];
typedef char layout_count[((unsigned)&((P*)0)->count==0x208)?1:-1];
typedef char layout_offset[((unsigned)&((P*)0)->offset==0x240)?1:-1];
typedef char layout_amount[((unsigned)&((P*)0)->amount==0x1048)?1:-1];
typedef char layout_adjusted[((unsigned)&((P*)0)->adjusted==0x1060)?1:-1];

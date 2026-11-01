/* (Claude agent) clock dt word loaded via ClockPage/ClockWord asm (same form as src/hand/func_001CF0A0.cpp, lw into the lui register); volatile on the lw keeps it after the v1E0 lqc2. */
typedef int qword __attribute__((mode(TI)));
typedef union { qword q; float f; } U;
typedef struct { float x,y,z; } VECTOR3;
typedef struct { qword row[3]; } M3;
union FLOATWORD { float value; int bits; };
extern struct { float t; union FLOATWORD delta; } D_002DDED0;
struct O { char pad[0xB0]; qword angularVelocity; char pad2[0x30]; qword angularForce; };
struct S { char pad[0x5C]; struct O *po; char pad2[0x1A4 - 0x60]; int k; char pad3[8]; M3 desired; qword v1E0; char pad4[0x1F4 - 0x1F0]; float f1F4, f1F8, f1FC, f200; };
void func_001F9B20(struct S *p);
static inline int ClockWord(char*page){int word;__asm__ volatile("lw %0,%%lo(%2)(%1)":"=r"(word):"r"(page),"i"(&D_002DDED0.delta.bits),"m"(*(const int*)(page-0x212C)));return word;}
static inline char*ClockPage(){register char*page __asm__("$2");__asm__("lui %0,%%hi(%1)":"=r"(page):"i"(&D_002DDED0.delta.bits));return page;}
void func_001F59A0(struct S *,M3 *,VECTOR3 *);
static inline void InitScalar(U *value,int bits) {
    register qword scalar __asm__("vf4");
    __asm__("qmtc2.ni %1,%0" : "=j"(scalar) : "r"(bits));
    value->q=scalar;
}
/** @sly2-annot addr=0x204108
 * Purpose: STEP vt70: base accel (0x1F9B20) then steering: mode +0x1A4==0 accelerate toward matTarget with
 *   smpSpin (0x1F59A0), mode 1 angular force from desired w +0x1E0
 * Info: proposed name PresetStepAccel (T1); class STEP slot70; TU step
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540,
 *   docs/reference/functions/player.md#murray-0x1b4728-0x1b9f80
 * @end-sly2-annot */
void PresetStepAccel(struct S *p)
{
    func_001F9B20(p);
    switch (p->k) {
    case 0: {
        VECTOR3 v;
        float s = p->f200;
        v.z = p->f1FC;
        v.x = p->f1F4 * s;
        v.y = p->f1F8 * s;
        func_001F59A0(p, &p->desired, &v);
        break;
    }
    case 1: {
        register struct O *po __asm__("$3") = p->po;
        U u;
        register qword d __asm__("vf3");         register qword vel __asm__("vf2");
        char*page=ClockPage();
        int word;
        d=p->v1E0;
        word=ClockWord(page);
        vel = po->angularForce;
        __asm__("vsub.xyzw %0,%0,%1" : "+j"(d) : "j"(po->angularVelocity));
        InitScalar(&u,word);
        __asm__("vdiv $Q, $vf0w, %1x\n\tvwaitq\n\tvmulq.xyzw %0, %0, $Q" : "+j"(d) : "j"(u.q));
        __asm__("vadd.xyzw %0,%0,%1" : "+j"(vel) : "j"(d));
        po->angularForce=vel;
        break;
    }
    case -1:
        break;
    }
}

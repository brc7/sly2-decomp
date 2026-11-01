typedef int qword __attribute__((mode(TI)));
struct M4 {qword r[4];};
union V {qword q;float f[4];};
struct JOB {int kind;void *owner;};
union SOURCE {void *object;float deadline;};
struct O {M4 local,world;float t0,t1;float *time,*dt;JOB *hook;int kind;short mode,state;unsigned int tag;O *parent;int priority;SOURCE source;unsigned short force,damping;qword velocity,v0,v1,v2,v3;};
extern qword D_002A2DD0;
extern "C" {
M4 func_0011C318(const M4 *);
M4 func_0022CDC0(O *);
void func_0022DD00(void *,O *);
void func_00117120(float,float,float,qword,qword,qword,qword,qword *,qword *,qword *);
}
static inline qword Zero() {qword r;__asm__("vsub.xyzw %0,$vf0,$vf0":"=j"(r));return r;}
static inline qword Add(qword a,qword b) {qword r;__asm__("vadd.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Transform(const M4 &m,qword v) {qword r=m.r[0];
    __asm__("vmulax.xyz ACC,%0,%4x\n\tvmadday.xyz ACC,%1,%4y\n\tvmaddaz.xyz ACC,%2,%4z\n\tvmaddw.xyz %0,%3,$vf0w":"+j"(r):"j"(m.r[1]),"j"(m.r[2]),"j"(m.r[3]),"j"(v));return r;}
static inline qword InverseTransform(const M4 &m,qword v) {register qword r __asm__("vf4")=m.r[0];
    __asm__("vmulax.xyz ACC,%0,%4x\n\tvmadday.xyz ACC,%1,%4y\n\tvmaddaz.xyz ACC,%2,%4z\n\tvmaddw.xyz %0,%3,$vf0w":"+j"(r):"j"(m.r[1]),"j"(m.r[2]),"j"(m.r[3]),"j"(v));return r;}
static inline qword BasisRow1(const M4 *m){register qword r __asm__("vf1");__asm__("lqc2 %0,16(%1)":"=j"(r):"r"(m),"m"(m->r[1]));return r;}
static inline qword BasisRow2(const M4 *m){qword r;__asm__("lqc2 %0,32(%1)":"=j"(r):"r"(m),"m"(m->r[2]));return r;}
static inline qword RotatePointerRows(const M4 &m,qword v){
qword r1=BasisRow1(&m);
qword r2=BasisRow2(&m);
__asm__("vmulax.xyz ACC,%1,%0x\n\tvmadday.xyz ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z":"+j"(v):"j"(m.r[0]),"j"(r1),"j"(r2));return v;}
static inline qword Rotate(const M4 &m,qword v) {
    __asm__("vmulax.xyz ACC,%1,%0x\n\tvmadday.xyz ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z":"+j"(v):"j"(m.r[0]),"j"(m.r[1]),"j"(m.r[2]));return v;}
static inline qword ParentRotate(const M4 &m,qword v) {register qword row0 __asm__("vf6")=m.r[0];
    __asm__("vmulax.xyz ACC,%1,%0x\n\tvmadday.xyz ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z":"+j"(v):"j"(row0),"j"(m.r[1]),"j"(m.r[2]));return v;}
static inline qword RotateRow(const M4 &m,qword v) {qword r=m.r[0];
    __asm__("vmulax.xyz ACC,%0,%3x\n\tvmadday.xyz ACC,%1,%3y\n\tvmaddz.xyz %0,%2,%3z":"+j"(r):"j"(m.r[1]),"j"(m.r[2]),"j"(v));return r;}
static inline void Eval(O *p,qword *v) {func_00117120(p->t1-p->t0,*p->time-p->t0,*p->dt,p->v0,p->v1,p->v2,p->v3,0,v,0);}
/** @sly2-annot addr=0x22D348
 * Purpose: linear velocity of an XF
 * Info: proposed name GetXfVelocity (T1); TU xf
 * Notes: runtime4: XF +0xB0 velocity; SO velocity = XF at SO+0x58 (plausible live values)
 * Confidence: mid (draft) [T1 0.70] - evidence: runtime (save-state RAM); sources: r8_21BB88+runtime
 * See: docs/engine/animation.md#7-key-functions, docs/engine/player.md#3-step-stepcpp-0x203948,
 *   docs/reference/functions/animation.md#xf-0x22c818-0x231100
 * @end-sly2-annot */
extern "C" qword func_0022D348(O *p,O *stop,qword in) {
    qword sum;
    qword delta;
    M4 m;
    qword input=in;
    sum=Zero();
    for(;p!=stop;p=p->parent) {
        switch(p->mode) {
        case 0:case 3:case 10:case 12:case 15:case 19:break;
        default:
            switch(p->kind) {
            case 1:case 4:case 5:case 9:case 10: {
                switch(p->mode) {
                case 1:case 2:case 7:case 8:case 9:case 11:case 17:case 18:case 20:case 21:delta=p->velocity;break;
                case 5:case 6:
                    if(p->hook)func_0022DD00(p->hook->owner,p);
                    switch(p->state) {
                    case 3:case 4:delta=p->v1;break;
                    case 1:{V tmp;Eval(p,&tmp.q);delta=tmp.q;break;}
                    case 0:case 2:case 5:default:delta=D_002A2DD0;break;
                    }
                    break;
                }
                if(p->parent)sum=Add(sum,RotateRow(p->parent->world,delta));
                else sum=Add(sum,delta);
                break;
            }
            default: {
                if(p->hook)func_0022DD00(p->hook->owner,p);
                m=func_0022CDC0(p);
                O *parent=p->parent;
                if(parent) {
                    M4 inverse=func_0011C318(&p->world);
                    sum=Add(sum,ParentRotate(parent->world,Transform(m,InverseTransform(inverse,input))));
                } else {
                    M4 inverse=func_0011C318(&p->world);
                    sum=Add(sum,Transform(m,InverseTransform(inverse,input)));
                }
                break;
            }
            }
            break;
        }
    }
    if(stop) {M4 inverse=func_0011C318(&stop->world);sum=RotatePointerRows(inverse,sum);}
    return sum;
}

// Apply queued translation and rotation, then refresh changed objects.
typedef int qword __attribute__((mode(TI)));
union U {qword q;int i[4];float f[4];};
struct M3 {qword r[3];};
struct LO {M3 matrix;U position;char pad40[0x60];LO *parent;int stamp;};
struct VE {short delta,pad;void(*fn)(void*);};
struct P {char padC[0xC];VE *vt;char pad10[0x48];LO *translation,*rotation;char pad60[0x88];int enabled;char padEC[4];U shift,turn;};
extern qword D_002A2DD0;
extern struct GAME {char pad3D58[0x3D58];int stamp;} D_003DD490;
extern "C" int func_0011D4D0(float,float,float);
extern "C" void SlotUnsettle(P *,LO *);
extern "C" qword ConvertXfVec(void *,LO *,qword);
extern "C" qword *VECTOR_opAssign(qword *,qword);
extern "C" M3 func_0011B570(qword);
extern "C" void func_001F4BF8(P *);
static inline float X(qword q){float f;__asm__("mtc1 %1,%0":"=f"(f):"r"(q));return f;}
static inline float Y(qword q){float f;__asm__("prot3w %0,%1":"=r"(f):"r"(q));return f;}
static inline float Z(qword q){float f;__asm__("pextuw %0,$zero,%1":"=r"(f):"r"(q));return f;}
static inline void Near(int*out,qword a,qword b){if(func_0011D4D0(X(a),X(b),0.0001f))if(func_0011D4D0(Y(a),Y(b),0.0001f))*out=func_0011D4D0(Z(a),Z(b),0.0001f)!=0;}
static inline qword Add(qword a,qword b){__asm__("vadd.xyzw %0,%1,%0":"+j"(b):"j"(a));return b;}
static inline M3 Mul(const M3&a,const M3&b) return r {
    qword a0=a.r[0],a1=a.r[1],a2=a.r[2];
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[0]):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[0]));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[1]):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[1]));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[2]):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[2]));
}
static inline qword Consume(P*p,LO*object,U&pending){SlotUnsettle(p,object);qword world=ConvertXfVec(0,object->parent,pending.q);pending.i[2]=0;pending.i[1]=0;return world;}
/** @sly2-annot addr=0x1FA528
 * Purpose: Apply queued translation and rotation (shift/turn) to SO then refresh changed objects
 * Info: proposed name ApplySoQueuedMotion (T2); class SO slot67; TU solve
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/physics-collision.md#solve-0x1f8dd8-0x1faf70
 * @end-sly2-annot */
extern "C" void func_001FA528(P*p)
{
    M3 final;
    int changed=0;
    if(p->enabled){
        int near=0;
        Near(&near,D_002A2DD0,p->shift.q);
        if(!near){
            LO *object=p->translation;
            changed=1;
            qword shift=Consume(p,object,p->shift);
            p->shift.i[0]=0;
            VECTOR_opAssign(&object->position.q,Add(object->position.q,shift));
            object->position.f[3]=1.0f;
            object->stamp=++D_003DD490.stamp;
        }
        near=0;Near(&near,D_002A2DD0,p->turn.q);
        if(!near){
            LO *object=p->rotation;
            changed=1;
            qword turn=Consume(p,object,p->turn);
            p->turn.i[0]=0;
            final=Mul(func_0011B570(turn),object->matrix);
            object->matrix=final;
            ((int*)&object->matrix.r[1])[3]=0;
            ((int*)&object->matrix.r[2])[3]=0;
            ((int*)&object->matrix.r[0])[3]=0;
            object->stamp=++D_003DD490.stamp;
        }
        if(changed){p->vt[18].fn((char*)p+p->vt[18].delta);func_001F4BF8(p);}
    }
}

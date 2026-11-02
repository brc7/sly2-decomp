typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];int i[4];};
struct M3 {U r[3];M3(){} M3(const M3&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];} M3&operator=(const M3&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];return *this;}};
struct OWNER {M3 matrix;U position;char pad40[0x60];OWNER *parent;int stamp;char padA8[8];U velocity;};
struct NODE {char pad10[0x10];unsigned long flags;char pad18[0x48];NODE *next;};
struct E {char pad34[0x34];NODE *first;};
struct VT {short delta,pad;void (*fn)(void *);};
struct P {char padC[0xC];VT *vt;char pad10[0x48];OWNER *position,*rotation;char pad60[0x8C];E *e;};
struct G {char pad3D58[0x3D58];int stamp;};
struct CLOCK {float time,dt;};
extern CLOCK D_002DDED0;
extern G D_003DD490;
extern "C" void func_00230630(P *,OWNER *);
extern "C" qword *VECTOR_opAssign(qword *,qword);
extern "C" M3 *func_0011B570(M3 *,qword);
static inline M3 BuildRotation(qword vector) return matrix {func_0011B570(&matrix,vector);}
extern "C" void NormalizeRotateMatrix3__FP7MATRIX3(M3 *);
extern "C" void UpdateSoPartMatrices(P *);
extern "C" void UpdateSoPartGeom(NODE *);
extern "C" void func_001F4D50(P *);
static inline qword Scalar(float f,U *out){qword q;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(f));out->q=q;return out->q;}
static inline qword AddScaled(qword p,qword v,qword k){qword r;__asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=&j"(r):"j"(p),"j"(v),"j"(k));return r;}
static inline qword Scale(qword v,qword k){qword r;__asm__("vmulx.xyzw %0,%1,%2x":"=&j"(r):"j"(v),"j"(k));return r;}
static inline void Clear(U *v){v->i[2]=0;v->i[1]=0;v->i[0]=0;}
static inline M3 Mul(const M3 &a,const M3 &b) return r
{
    qword a0=a.r[0].q,a1=a.r[1].q,a2=a.r[2].q;
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[0].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[0].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[1].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[1].q));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[2].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[2].q));
}
/** @sly2-annot addr=0x1F9C48
 * Purpose: Integrates position/rotation of SOs in a solved group (NormalizeRotateMatrix3, UpdateSoPartGeom)
 *   stamped with SW frame counter +0x3D58
 * Info: proposed name IntegrateSoGroupXf (T2); TU solve
 * Notes: names_v2 v1 proposed EnsureSoLvo__FP2SO (0.073) | runtime: SW+0x3D58 is a stamp counter
 *   (~145/frame), frame counter is SW+0x3D60
 * Confidence: low (draft) [T2 0.25] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r7_1F8DD8+runtime-corrected
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#solve-0x1f8dd8-0x1faf70
 * @end-sly2-annot */
extern "C" void func_001F9C48(P *p)
{
    U scalar;
    M3 frame;
    OWNER *position=p->position,*rotation=p->rotation;
    U *positionVelocity=&position->velocity,*rotationVelocity=&rotation->velocity;
    float coefficient=-0.5f;
    CLOCK *clock;
    G *global;
    NODE *node;
    func_00230630(p,position);
    func_00230630(p,rotation);
    clock=&D_002DDED0;
    {qword point=position->position.q;
    qword k=Scalar(clock->dt*coefficient,&scalar);
    VECTOR_opAssign(&position->position.q,AddScaled(point,position->velocity.q,k));}
    position->position.f[3]=1.0f;
    global=&D_003DD490;
    position->stamp=++global->stamp;
    Clear(positionVelocity);
    frame=Mul(BuildRotation(Scale(rotation->velocity.q,Scalar(clock->dt*coefficient,&scalar))),rotation->matrix);
    NormalizeRotateMatrix3__FP7MATRIX3(&frame);
    {M3 *dest=&rotation->matrix;*dest=frame;dest->r[1].i[3]=0;dest->r[2].i[3]=0;dest->r[0].i[3]=0;}
    rotation->stamp=++global->stamp;
    Clear(rotationVelocity);
    p->vt[18].fn((char*)p+p->vt[18].delta);
    UpdateSoPartMatrices(p);
    if(p->e){
        for(node=p->e->first;node;node=node->next){
            if(!(node->flags&(1UL<<50)))UpdateSoPartGeom(node);
        }
    }
    func_001F4D50(p);
}

typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct M4 {U rows[4];M4(){}M4(const M4 &m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];rows[3]=m.rows[3];}M4 &operator=(const M4 &m){rows[0]=m.rows[0];rows[1]=m.rows[1];rows[2]=m.rows[2];rows[3]=m.rows[3];return *this;}};
struct SPACE {char pad0[0x40];M4 matrix;};
union FLAGS {unsigned long value;unsigned char bytes[8];};
struct P {char pad0[0x28];FLAGS flags;char pad30[0x30];SPACE *space;char pad64[0xC8];float opacity;int unused130;void *material;char pad138[0x108];float pivot[3];char pad24C[0x114];float start;};
struct CAMERA {char pad0[0x14];int mask;char pad18[0x78];U position;};
struct PROFILE {float a,b,c,unused,min,max;};
struct CLOCK {float time,dt;};
struct EFFECT;
extern CAMERA D_002DDF40;
extern PROFILE D_003E1A60;
extern CLOCK D_002DDED0;
extern float D_002C3D4C;
extern float D_002C3D50[3],D_002C3D60[2],D_002BBB60[2];
extern "C" {
void func_001F3C10(P *);
int FActiveCplcy__FP5CPLCY(void *);
void RenderSoGlobset(EFFECT *,const M4 *,void *,float,float);
void *func_002333B4(void *,int,unsigned int);
qword func_001A62E8(P *);
float sinf(float);
qword *VECTOR_opAssign(qword *,qword);
M4 *__ml__FRC7MATRIX4T0(M4 *,const M4 *,const M4 *);
M4 *InvertMatrix4Rigid(M4 *,const M4 *);
}
static inline qword Subtract(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline float Length(qword a){qword one;float f;__asm__("vmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(a),"=&j"(one));__asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(a));__asm__("mtc1 %1,%0":"=f"(f):"r"(a));return f;}
/** @sly2-annot addr=0x207690
 * Purpose: STEPGUARD vt19: draw the globset; when the camera policy is active, apply a 0.85 s squash/stretch
 *   pulse after +0x360 (pivot +0x240) and distance-based scale
 * Info: proposed name RenderStepguardSelf (T1); class STEPGUARD slot19; TU stepguard
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
extern "C" void RenderStepguardSelf(P *p)
{
    M4 local;
    U offset;
    M4 basis,matrix,product,inverse;
    CAMERA *camera=&D_002DDF40;
    if(p->flags.bytes[5]&camera->mask){
        func_001F3C10(p);
        if(!FActiveCplcy__FP5CPLCY((char *)camera+0x3B0)){
            RenderSoGlobset((EFFECT *)((char *)p+0x80),0,p->material,p->opacity,p->opacity);
        }else{
            func_002333B4(&local,0,sizeof(M4));
            local.rows[3].f[3]=1.0f;
            local.rows[2].f[2]=1.0f;
            local.rows[1].f[1]=1.0f;
            local.rows[0].f[0]=1.0f;
            float distance=Length(Subtract(func_001A62E8(p),camera->position.q));
            PROFILE *profile=&D_003E1A60;
            float gain=profile->a+distance*(profile->b+distance*profile->c);
            float min=profile->min,max=profile->max;
            if(gain<min)gain=min;else if(max<gain)gain=max;
            float elapsed=D_002DDED0.time-p->start;
            float along=gain,across=gain;
            if(elapsed<0.85f){
                float wave=sinf(elapsed*D_002C3D4C);
                float u=elapsed*1.1764705f;
                float mix=(wave+1.0f)*0.5f;
                float magnitude=D_002C3D50[0]+u*(D_002C3D50[1]+u*D_002C3D50[2]);
                float blend=(1.0f-mix)*D_002C3D60[0]+mix*D_002C3D60[1];
                float min=D_002BBB60[0],max=D_002BBB60[1];
                if(magnitude<min)magnitude=min;else if(max<magnitude)magnitude=max;
                float strength=blend*magnitude;
                across=1.0f-strength;
                along=1.0f+strength;
            }
            local.rows[0].f[0]=across;
            local.rows[1].f[1]=across;
            local.rows[2].f[2]=along;
            offset.f[0]=(1.0f-across)*p->pivot[0];
            offset.f[2]=(1.0f-along)*p->pivot[2];
            offset.f[1]=(1.0f-across)*p->pivot[1];
            VECTOR_opAssign(&local.rows[3].q,offset.q);
            local.rows[3].f[3]=1.0f;
            M4 *productPointer=&product;
            M4 *basisPointer=&basis;
            M4 *matrixPointer=&matrix;
            basis.rows[0]=p->space->matrix.rows[0];
            basis.rows[1]=p->space->matrix.rows[1];
            basis.rows[2]=p->space->matrix.rows[2];
            basis.rows[3]=p->space->matrix.rows[3];
            __ml__FRC7MATRIX4T0(productPointer,basisPointer,&local);
            M4 *inversePointer=&inverse;
            InvertMatrix4Rigid(inversePointer,basisPointer);
            __ml__FRC7MATRIX4T0(matrixPointer,productPointer,inversePointer);
            RenderSoGlobset((EFFECT *)((char *)p+0x80),matrixPointer,p->material,p->opacity,p->opacity);
        }
    }
}

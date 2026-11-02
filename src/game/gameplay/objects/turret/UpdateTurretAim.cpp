// Update target-facing angles or integrate angular input.
typedef int qword __attribute__((mode(TI)));
struct M3 {qword r[3];};
union V {qword q;float f[4];};
struct BOUND {float lo,hi;};
struct CONTROL {char pad[0x10];float x,y;char pad18[8];float z;};
struct S {char pad[0x150];CONTROL *control;char pad154[0x2C];void *target;int aim;int pad188;float pitch,yaw,vpitch,vyaw;BOUND yawRange,pitchRange;char pad1ac[0x10];float strength;char pad1c0[0xC];int output;char pad1d0[8];void *rotation;};
struct CLOCK {float t,dt;};
extern CLOCK D_002DDED0;
extern int D_003E0FE0;
extern char D_003D4A60[];
extern float D_002C4408[];
extern "C" {
qword func_001A62E8(void *);
M3 LoadLookAtMatrix(qword);
void func_0011BC88(const M3 *,float *,float *);
float func_0011CFE0(float,float);
float GSmooth__FfffP3SMPPf(float,float,float,const float *,float *);
float func_0011D948(const BOUND *,float);
void SetActPositionGoal(void *,qword);
void func_00129EC8(void *);
}
struct CRef {int kind,value;CRef(const CRef &) __asm__("func_001291A8");~CRef(){func_00129EC8(this);}};
extern "C" CRef RefGetGlobalSaveVar(int,void *);
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Make(float x,float y,float z) {
    qword q,t;
    __asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0":"=&r"(q),"=&r"(t):"f"(x),"f"(y),"f"(z));
    return q;
}
/** @sly2-annot addr=0x225BA0
 * Purpose: pan/tilt toward target or from stick, limited by lmPan/lmTilt
 * Info: proposed name UpdateTurretAim (T1); TU turret
 * Notes: names_v2 C proposed AdjustStepXpVelocityBase__FP4STEPP2XPi (0.438)
 * Confidence: mid (draft) [T1 0.50] - evidence: static naming pass; sources: r8_21BB88
 * See: docs/engine/minigames.md#4-mounted-turret-turret-0x45-turret-tu-0x2259d8-0x226550-conf-high-ccode,
 *   docs/reference/functions/gameplay-objects.md#turret-0x2259d8-0x226550
 * @end-sly2-annot */
extern "C" void UpdateTurretAim(S *p){
    if(p->target && p->aim) {
        M3 m;
        float pitch,yaw;
        qword target;
        m=LoadLookAtMatrix((target=func_001A62E8(p->target),Sub(target,func_001A62E8(p))));
        func_0011BC88(&m,&pitch,&yaw);
        float dp=pitch-p->pitch;
        float dy=yaw-p->yaw;
        p->pitch=pitch;p->yaw=yaw;
        p->vpitch=dp;p->vyaw=dy;
    } else {
        if(p->control) {
            float x=func_0011CFE0(p->control->x+p->control->z,1.0f);
            float y=func_0011CFE0(p->control->y,1.0f);
            float strength=p->strength;
            float gain=(1.0f-strength)*0.5f+strength;
            float cubic=-x*x;
            cubic=cubic*x;
            float accel=gain*10.0f;
            float ax=cubic*accel;
            float limit=strength*1.3f;
            if(ax*p->vpitch<=0.0f)p->vpitch=0.0f;
            float cubeY=y*y;
            cubeY=cubeY*y;
            float ay=cubeY*accel;
            if(!RefGetGlobalSaveVar(D_003E0FE0,D_003D4A60).value)ay=-ay;
            if(ay*p->vyaw<=0.0f)p->vyaw=0.0f;
            CLOCK *clock=&D_002DDED0;
            const float *damping=D_002C4408;
            p->vpitch=GSmooth__FfffP3SMPPf(p->vpitch,0.0f,clock->dt,damping,0);
            p->vyaw=GSmooth__FfffP3SMPPf(p->vyaw,0.0f,clock->dt,damping,0);
            p->vpitch=func_0011CFE0(p->vpitch+ax*clock->dt,limit*0.9f);
            p->vyaw=func_0011CFE0(p->vyaw+ay*clock->dt,limit*0.8f);
            p->pitch+=p->vpitch*clock->dt;
            p->yaw+=p->vyaw*clock->dt;
        }
    }
    p->pitch=func_0011D948(&p->pitchRange,p->pitch);
    p->yaw=func_0011D948(&p->yawRange,p->yaw);
    if(p->output){V v;v.q=Make(0.0f,-p->yaw,p->pitch);SetActPositionGoal(p->rotation,v.q);}
}

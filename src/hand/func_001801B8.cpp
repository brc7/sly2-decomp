typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct M3 {
 U r[3];
 M3(){}
 M3(const M3&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];}
 M3&operator=(const M3&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];return *this;}
};
struct VTE {short delta,pad;void*fn;};
struct P {char pad[0xC];VTE*vt;char pad10[0x17C];int flag;char pad190[0x30];float time;char pad1C4[0xC];P*target;float turnRate;};
struct CLOCK{float time,dt;};
extern "C" CLOCK D_002DDED0;
extern "C" qword D_002A2DC0;
extern "C" void UpdateSo(P*);
extern "C" qword func_001A62E8(P*);
extern "C" float func_002458F0(float,float);
extern "C" float RadNormalize__Ff(float);
extern "C" float func_0011CFE0(float,float);
extern "C" M3 func_0011B500(qword,float);
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline float X(qword v){float f;__asm__("mtc1 %1,%0":"=f"(f):"r"(v));return f;}
static inline float Y(qword v){float f;__asm__("prot3w %0,%1":"=r"(f):"r"(v));return f;}
static inline qword Transform(const M3&m,qword v){qword r;__asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r):"j"(m.r[0].q),"j"(m.r[1].q),"j"(m.r[2].q),"j"(v));return r;}
static inline M3 Mul(const M3&a,const M3&b) return r
{
 qword a0=a.r[0].q,a1=a.r[1].q,a2=a.r[2].q;
 __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[0].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[0].q));
 __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[1].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[1].q));
 __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[2].q):"j"(a0),"j"(a1),"j"(a2),"j"(b.r[2].q));
}
/** @sly2-annot addr=0x1801B8
 * Purpose: vt16: UpdateSo; self-destruct (vt82) when its timer expires; turn toward target with limited turn
 *   rate
 * Info: proposed name UpdateHackbullet (T2); class HACKBULLET slot16; TU tu_17F090
 * Notes: Cid4A->Cid74 (CID 0x4A=74, hex-literal) | class identity from whole_game WE names/object strings:
 *   UpdateCid74 -> UpdateHackbullet
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/engine/minigames.md#23-enemies-walls-lock,
 *   docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
extern "C" void func_001801B8(P*p){
 UpdateSo(p);
 if(!p->flag&&p->time<=D_002DDED0.time)((void(*)(void*,P*))p->vt[82].fn)((char*)p+p->vt[82].delta,0);
 if(!p->target||p->turnRate==0.0f)return;
 M3 rotation,matrix;
 qword position;
 position=func_001A62E8(p->target);
 qword displacement=Sub(position,func_001A62E8(p));
 float desired=func_002458F0(Y(displacement),X(displacement));
 qword direction=((qword(*)(void*))p->vt[33].fn)((char*)p+p->vt[33].delta);
 float current=func_002458F0(Y(direction),X(direction));
 float angle=func_0011CFE0(RadNormalize__Ff(desired-current),p->turnRate*D_002DDED0.dt);
 rotation=func_0011B500(D_002A2DC0,angle);
 ((void(*)(void*,qword))p->vt[35].fn)((char*)p+p->vt[35].delta,Transform(rotation,direction));
 // The original reads this local matrix before initialization.
 matrix=Mul(rotation,matrix);
 ((void(*)(void*,M3*))p->vt[30].fn)((char*)p+p->vt[30].delta,&matrix);
}

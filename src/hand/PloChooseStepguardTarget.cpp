// Choose an eligible actor using lock state, priority and weighted distance.
typedef unsigned long ulong;
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct O {char pad28[0x28];ulong flags;};
struct ENTRY {O *object;int available;int x8,xC;};
struct GROUP {char padFC[0xFC];int count;ENTRY entries[1];};
struct P {char pad38C[0x38C];O *previous,*current;char pad394[0xBD0];GROUP *group;char padF68[0x54];int mode;};
extern float D_002A886C,D_002C3D70[3],D_002C3D80[3];
extern "C" qword func_001A62E8(void *);
extern "C" int FIsBasicDerivedFrom__FP5BASIC3CID(O *,int);
extern "C" int func_002089F0(P *,O *);
extern "C" int func_00208B38(P *,O *);
extern "C" int func_002087E8(P *,O *);
static inline qword Sub(const qword&a,qword b){
    qword d,bv,av;
    __asm__("lqc2 %2,%4\n\tqmtc2.ni %3,%1\n\tvsub.xyzw %0,%2,%1":"=&j"(d),"=&j"(bv),"=&j"(av):"r"(b),"m"(a));
    return d;
}
static inline float Length(qword d){
    qword one;float f;
    __asm__ __volatile__("vmul.xyz %0,%0,%0":"+j"(d));
    __asm__ __volatile__("vaddw.x %0,$vf0,$vf0w":"=j"(one));
    __asm__("vadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"+j"(d):"j"(one));
    __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(d));
    __asm__("mtc1 %1,%0":"=f"(f):"r"(d));
    return f;
}
static inline float Weight(float distance,const float *k){return k[0]+distance*(k[1]+distance*k[2]);}
/** @sly2-annot addr=0x2082D0
 * Purpose: STEPGUARD vt88: keep forced target +0x390 if in world; else among detector entries pick: forced
 *   classes (+0x3C0) first, skip ignored classes (+0x3A8), prefer classes +0x394, hysteresis for current
 *   target (+0x38C, CLQ -400+0.8d), rage weighting for guards
 * Info: proposed name PloChooseStepguardTarget (T1); class STEPGUARD slot88; TU stepguard
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
extern "C" O *PloChooseStepguardTarget(P*p)
{
    O *best=0;
    if(p->current&&(((unsigned int)(p->current->flags>>60))&1)){best=p->current;goto done;}
    {
        float bestDistance=D_002A886C;
        U origin;origin.q=func_001A62E8(p);
        for(int i=0;i<p->group->count;i++){
            ENTRY *entry=&p->group->entries[i];
            O *candidate=entry->object;
            if(!candidate)continue;
            if(!entry->available&&(!(p->mode==0x22||p->mode==0x25)||!FIsBasicDerivedFrom__FP5BASIC3CID(candidate,0x38)))continue;
            if(func_002089F0(p,candidate))continue;
            if(func_00208B38(p,candidate))return candidate;
            qword delta=Sub(origin.q,func_001A62E8(candidate));O *previous=p->previous;
            float distance=Length(delta);
            if(candidate==previous)distance=Weight(distance,D_002C3D70);
            if((p->mode==0x22||p->mode==0x25)&&FIsBasicDerivedFrom__FP5BASIC3CID(candidate,0x38))distance=Weight(distance,D_002C3D80);
            if(func_002087E8(p,candidate)){
                if(best&&!func_002087E8(p,best)){bestDistance=distance;goto take;}
            }else if(best&&func_002087E8(p,best))continue;
            if(distance<bestDistance){bestDistance=distance;take:best=candidate;}
        }
    }
    done:return best;
}

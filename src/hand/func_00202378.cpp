/* func_00202378 (Lilly / Claude agent, T1560): four-ray visibility of an actor from the selector's origin (p+0x20, the caller
 * func_00202540 raises it by 20). Collects the objects between the two points (func_0020FAA0 with the func_00202350 filter),
 * then casts rays from the origin to the actor's centre (pulled radius 0xE4 towards the viewer) and to that centre moved by
 * the radius along the camera's up row (D_002DDFA0[1]) and along the side direction up x view. Every ray that hits an object
 * counts: none -> state 1, all four -> state 2 (func_00202540 drops those), some -> state 3. No register pins.
 * What made it match: the actor position is a plain qword across the call (spilled to 0x70 by the compiler), the scale-by-radius
 * multiplies are in place on direction/up (so they wait for the cross product), the side vector's dot product names its
 * operand twice, and the case-4 constant is a variable set before the switch (the game has it in v1 in the beqz delay slot). */
typedef int qword __attribute__((mode(TI)));
union V {qword q;float f[4];};
struct LO {char padE4[0xE4];float radius;char padE8[0x20];int state;};
struct P {char pad20[0x20];qword position;};
struct QUERY {int count;LO **objects;};
// Stack-scope object (sp+0): func_001AE478 opens it, func_001AE4A0(this,2) frees the stack memory taken since.
struct FRAME;
extern "C" FRAME *func_001AE478(FRAME *);
extern "C" void func_001AE4A0(FRAME *,int);
struct FRAME {FRAME(){func_001AE478(this);}~FRAME(){func_001AE4A0(this,2);}};
extern "C" qword func_001A62E8(LO *);
extern "C" int func_00202350(void *,LO *);
extern "C" void func_0020FAA0(void *,qword,qword,int (*)(void *,LO *),void *,int *,LO ***);
extern "C" LO *func_0014CC58(int,qword,qword,int,LO **,void *);
extern char D_003DD490[];
extern qword D_002DDFA0[3];
static inline qword Add(qword a,qword b){qword r;__asm__("vadd.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Sub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Cross(qword a,qword b){qword r;__asm__("vopmula.xyz $ACC,%1,%2\n\tvopmsub.xyz %0,%2,%1":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Scale(qword v,qword k){qword r;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(v),"j"(k));return r;}
// v / |v|: the dot product names v twice (the side vector's registers only match that way)
static inline qword Normalize(qword v)
{
    qword d,one,l,r;
    __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=j"(one):"j"(v),"j"(v));
    __asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(l):"j"(d));
    __asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q":"=j"(r):"j"(l),"j"(v));
    return r;
}
// v *= k.x in place
#define SCALE_IN_PLACE(v,k) __asm__("vmulx.xyzw %0,%0,%1x":"+j"(v):"j"(k))
/** @sly2-annot addr=0x202378
 * Purpose: 4-ray occlusion test from origin to target -> state 1 clear / 2 blocked / 3 partial
 * Info: proposed name UpdateReconTargetVisibility (T1); TU tu_201D78
 * Notes: names_v2 C proposed PostBrkLoadCallbackHookup__FP3BRK5MSGIDPv (0.438)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r7c_hud+verdicts_followups
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
extern "C" void func_00202378(P *p,LO *other)
{
    int cHit=0;
    unsigned i=0;
    FRAME frame;
    V points[4],scale;
    QUERY query;
    float radius;
    V *point;
    qword origin=func_001A62E8(other);
    radius=other->radius;
    func_0020FAA0(D_003DD490,origin,p->position,func_00202350,0,&query.count,&query.objects);
    {
        qword up=D_002DDFA0[1];
        qword direction=Normalize(Sub(p->position,origin));
        qword k,side;
        __asm__("qmtc2.ni %1,%0":"=j"(k):"r"(radius));
        scale.q=k;
        side=Normalize(Cross(up,direction));
        SCALE_IN_PLACE(direction,scale.q);
        origin=Add(origin,direction);
        SCALE_IN_PLACE(up,scale.q);
        points[0].q=origin;
        points[1].q=Add(origin,up);
        points[2].q=Sub(origin,up);
        points[3].q=Add(origin,Scale(side,scale.q));
    }
    point=points;
    for(;i<4;++i){
        if(func_0014CC58(0,p->position,point->q,query.count,query.objects,0))++cHit;
        ++point;
    }
    {
        int cRay=4; // the game loads this 4 before the switch (v1, beqz delay slot); a case-label constant alone is loaded in v0 after it
        switch(cHit){case 0:other->state=1;break;case 4:other->state=2;break;case 1:case 2:case 3:other->state=3;break;}
        i=cRay;
    }
}

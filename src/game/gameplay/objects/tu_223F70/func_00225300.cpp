// State-5 update: fade the actor in, hand it control of the source's basis, then drop a prize and leave state 3.
// No actor -> state 3; actor without a live 0xB0 behaviour -> state 2. Otherwise copy the source's (or own) world basis
// into the actor's control record (+0xFD0), set the behaviour to state 4 once within 100 units, fade the actor's colour
// with the quadratic D_002C43C0 {1.2, 0, -1} over D_002C43B0 (3 s) clamped to D_002BBB60 {0, 1}, and after the fade
// spawn prize 0x5D at the source, call actor slot 9, post game event 0x18 for the actor (BroadcastGameEvent) and set state 3.
// Matching notes (notes/220000.md, 2026-10-24):
//  - Clamp reads both bounds before testing: gMax is loaded in the bc1f delay slot, and &D_002BBB60 becomes a loop
//    invariant (fp) while gMin goes through the shared %hi (s5).
//  - The game event's &D_002DEB30 is built straight into $a0 (the one register pin). As a normal argument it goes through
//    an argument pseudo; loop.c's single-use substitution then gives %hi(D_002DEB30) life 2 in loop pass 2, so it is
//    hoisted, wins s7 (a later hoist has the shortest live length) and pushes &D_002BBB60 out of the registers.
//  - The colour broadcast is one asm for both pextlw (t ties to the mfc1 register, the lw of the colour sits between).
typedef int qword __attribute__((mode(TI)));
typedef struct { short d,pad; void (*fn)(void *,...); } VE;
struct M3 { qword r[3]; };
struct M4 {M3 basis;qword position;};
struct O { char pad[0xC]; VE *vt; };
struct CONTROL {int id,state,pad;void *object;};
struct A { char pad[0xC]; VE *vt; char pad10[0xFB4]; void *color; char padFC8[8]; struct CONTROL basis; };
struct R { char pad[0x30]; int state; };
struct S { char pad[0xC]; VE *vt; char pad10[0x140]; int state; float time; char pad158[0x58]; struct O *source; struct A *actor; };
extern "C" {
extern struct CLOCK { float t,dt; } D_002DDED0;
struct LM { float gMin, gMax; };
extern float D_002C43B0,D_002C43C0[3];
extern LM D_002BBB60;
extern char D_003DD490[];
struct GAME;
extern GAME D_002DEB30;
void func_002240B0(struct S *);
struct R *func_00205458(void *,int);
int func_0017A9C0(struct R *);
struct M4 *func_001A6318(void *);
void func_00209840(struct CONTROL *,const struct M3 *);
qword func_001A62E8(void *);
void func_0017A8C8(struct R *,int);
void SetActPositionGoal(void *,qword);
void SprayDprizes(void *,int,qword,void *,int);
void BroadcastGameEvent(GAME *,int,void *);
}
static inline qword Sub(qword a,qword b) { qword r; __asm__("vsub.xyzw %0,%1,%2" : "=j"(r) : "j"(a),"j"(b)); return r; }
static inline float Length(qword d) {
    qword one; float f;
    __asm__("vmul.xyz %0,%0,%0\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z" : "+j"(d),"=j"(one));
    __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q" : "+j"(d));
    __asm__("qmfc2.ni %0,%1" : "=r"(f) : "j"(d)); return f;
}
static inline void Clamp(float *pg,LM *plm){float gMax=plm->gMax,gMin=plm->gMin;if(*pg<gMin)*pg=gMin;else if(gMax<*pg)*pg=gMax;}
static inline qword Splat(float f) {
    int b; qword r,t;
    __asm__("mfc1 %0,%1" : "=r"(b) : "f"(f));
    __asm__("pextlw %0,%2,%2\n\tpextlw %1,%0,%0" : "=r"(t),"=r"(r) : "r"(b));
    return r;
}
/** @sly2-annot addr=0x225300
 * Purpose: state-5 update: fade actor in, hand control, drop prize 0x5D, post game event 0x18, state 3
 * Info: proposed name UpdateCid102 (T2); class CID102 slot16; TU tu_223F70
 * Notes: names_v2 C proposed UpdateCamsen__FP6CAMSENf (0.290)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r8_21BB88
 * See: docs/reference/functions/gameplay-objects.md#tu_223f70-0x223f70-0x2259d8
 * @end-sly2-annot */
extern "C" void func_00225300(struct S *p) {
    int old;
    func_002240B0(p);
    do {
        old=p->state;
        if(old==5) {
            struct R *r;
            if(!p->actor) {
                p->vt[76].fn((char*)p+p->vt[76].d,3);
            } else if(!(r=func_00205458(p->actor,0xB0)) || func_0017A9C0(r)) {
                p->vt[76].fn((char*)p+p->vt[76].d,2);
            } else {
                M3 m((p->source?func_001A6318(p->source):func_001A6318(p))->basis);
                func_00209840(&p->actor->basis,&m);
                if(r->state!=4) {
                    qword pos=p->source?func_001A62E8(p->source):func_001A62E8(p);
                    qword other;
                    other=func_001A62E8(p->actor);
                    if(Length(Sub(pos,other))<100.0f)func_0017A8C8(r,4);
                }
                if(p->actor->color) {
                    float t=(D_002DDED0.t-p->time)/D_002C43B0;
                    float g=D_002C43C0[0]+t*(D_002C43C0[1]+t*D_002C43C0[2]);
                    Clamp(&g,&D_002BBB60);
                    SetActPositionGoal(p->actor->color,Splat(g));
                }
                if(D_002C43B0<D_002DDED0.t-p->time) {
                    SprayDprizes(D_003DD490,0x5D,func_001A62E8(p->source),p,1);
                    p->actor->vt[9].fn((char*)p->actor+p->actor->vt[9].d);
                    // PIN: &D_002DEB30 straight into $a0 keeps %hi(D_002DEB30) inside the loop (see the header).
                    register GAME *pgame asm("$4")=&D_002DEB30;
                    BroadcastGameEvent(pgame,0x18,p->actor);
                    p->vt[76].fn((char*)p+p->vt[76].d,3);
                }
            }
        }
    } while(p->state!=old);
}

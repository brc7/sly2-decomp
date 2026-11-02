typedef unsigned int uint;
struct VT {short adjustment,unused;void *function;};
struct ACTOR {char pad0[0xF4];int id;int unusedF8,key;};
struct X {char pad0[0xC];VT *vtable;char pad10[0x30];uint flags;char pad44[0x14];void *shown;char pad5C[0x98];ACTOR *actor;};
struct P {int state;char pad4[0x70];int specialCount;char pad78[0x14];ACTOR *actor;char pad90[0x10];uint savedFlags;void *resource;X **model;X *primary,*secondary;char padB4[8];int selected,pending;};
extern char D_003DD490[],D_003DD040[],D_002DDED0[],D_002DE390[],D_002DEB30[];
extern "C" {
void IncrementSwHandsOff(void *,int);
void DecrementSwHandsOff(void *,int);
void func_002020C8(P *);
void RefreshReconPendingTargets(P *);
void UpdateReconChecklist(P *);
void StopWidgetAnim(X *,int);
void PlayWidgetAnim(X *,int);
void func_00214300(X *,int);
void func_0014D1D0(void *,int,float);
void func_001FE5B8(void *,int,int);
void func_0014FE28(void *);
void *StartDialog(void *,int,int);
void func_00147C20(void *,int,int);
void func_002036E8(ACTOR *,int);
void BroadcastGameEvent(void *,int,void *);
int func_002022D8(P *);
void func_002022A0(P *);
void func_00201E88(P *);
void func_00201F90(P *);
void func_00201F00(P *);
void *PsndStartSound(int);
void SetSndSourceLo(void *,void *);
void SetWidgetAnimPhase(X *,int,float);
void SetWidgetState(X *,int);
void SetWidgetRequest(X *,int);
}
static inline void Post(X *x,int n){VT *entry=&x->vtable[49];((void (*)(void *,int))entry->function)((char *)x+entry->adjustment,n);}
static inline void Clear(X *x){VT *entry=&x->vtable[50];((void (*)(void *))entry->function)((char *)x+entry->adjustment);}
static inline int Untag(int word)
{
    register int result __asm__("v0");
    __asm__("and %0,%1,%2" : "=r"(result) : "r"(word),"r"(~1));
    return result;
}
static inline int IndirectWord(int word)
{
    register int result __asm__("v1");
    __asm__("xori %0,%1,1\n\tandi %0,%0,1" : "=r"(result) : "r"(word));
    return result;
}
static inline void Restore(X *x,uint mask){uint f=x->flags;if((f^1)&1)*(uint *)f=mask|(*(uint *)f&1);else x->flags=mask|(f&1);}
/** @sly2-annot addr=0x202768
 * Purpose: state machine -1..10; 8 = snapshot (clock & sound paused, player flags 0x102), 9 = photo taken
 *   (StartDialog, BroadcastGameEvent 10, mark target done)
 * Info: proposed name SetReconSelState (T1); TU tu_201D78
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
extern "C" void SetReconSelState(P *p,int nextState)
{
    if(p->state==nextState)return;
    if(!p->primary)return;
    if(!p->secondary)return;
    if(((uint)(p->state-8)<2)!=((uint)(nextState-8)<2)){
        if((uint)(nextState-8)<2)IncrementSwHandsOff(D_003DD490,0);
        else DecrementSwHandsOff(D_003DD490,0);
    }
    switch(p->state){
    case -1:case 0:case 2:case 3:case 10:break;
    case 1:
        func_002020C8(p);
        RefreshReconPendingTargets(p);
        PlayWidgetAnim(p->primary,0x35D);
        if(p->specialCount>0)PlayWidgetAnim(p->primary,0x35E);
        UpdateReconChecklist(p);
        break;
    case 4:
        StopWidgetAnim(p->secondary,0x359);
        break;
    case 5:case 6:
        StopWidgetAnim(p->secondary,0x35A);
        break;
    case 7:
        StopWidgetAnim(p->primary,0x356);
        StopWidgetAnim(p->secondary,0x356);
        StopWidgetAnim(p->secondary,0x357);
        break;
    case 8:
        Restore(*p->model,p->savedFlags);
        func_0014D1D0(D_002DDED0,0,1.0f);
        func_001FE5B8(D_003DD040,1,1);
        StopWidgetAnim(p->primary,0x358);
        break;
    case 9:
        func_0014FE28(D_002DE390);
        func_00214300(p->primary,-1);
        Post(p->primary,0x310);
        if(p->resource){
            if(p->actor->key!=-1)StartDialog(D_003DD040,p->actor->key,0);
            func_00147C20(p->resource,0x608,p->actor->key);
        }
        func_002036E8(p->actor,1);
        BroadcastGameEvent(D_002DEB30,10,0);
        p->actor=0;
        if(func_002022D8(p)){
            PlayWidgetAnim(p->primary,0x35D);
            if(p->specialCount>0)PlayWidgetAnim(p->primary,0x35E);
        }
        UpdateReconChecklist(p);
        break;
    }
    int previous=p->state;
    p->state=nextState;
    switch(nextState){
    case 0:
        p->actor=0;p->selected=-1;p->pending=-1;
        func_002022A0(p);
        Post(p->primary,-1);
        func_00201E88(p);
        func_00201F90(p);
        BroadcastGameEvent(D_002DEB30,9,0);
        break;
    case 1:
        StopWidgetAnim(p->primary,0x35D);
        Post(p->secondary,0x339);
        break;
    case 5:
        PlayWidgetAnim(p->secondary,0x35A);
        SetWidgetAnimPhase(p->secondary,0x35A,0.0f);
        break;
    case 6:
        PlayWidgetAnim(p->secondary,0x35A);
        SetWidgetAnimPhase(p->secondary,0x35A,0.0f);
        break;
    case 7:
        SetSndSourceLo(PsndStartSound(0x52),0);
        p->selected=p->actor->id;
        PlayWidgetAnim(p->primary,0x356);
        PlayWidgetAnim(p->secondary,0x356);
        PlayWidgetAnim(p->secondary,0x357);
        break;
    case 8: {
        StopWidgetAnim(p->primary,0x35B);
        PlayWidgetAnim(p->primary,0x358);
        register X **model __asm__("a1");
        register X *widget __asm__("v0");
        register int flags __asm__("a0");
        __asm__("lw %0,%1":"=r"(model):"m"(p->model));
        __asm__("lw %0,0(%1)":"=r"(widget):"r"(model),"m"(*model));
        __asm__("lw %0,64(%1)":"=r"(flags):"r"(widget),"m"(widget->flags));
        if(IndirectWord(flags))flags=*(uint *)flags;
        p->savedFlags=Untag(flags);
        register X *updated __asm__("a0");
        register uint raw __asm__("v1");
        __asm__("lw %0,0(%1)":"=r"(updated):"r"(model),"m"(*model));
        __asm__("lw %0,64(%1)":"=r"(raw):"r"(updated),"m"(updated->flags));
        if((raw^1)&1)*(uint *)raw|=0x102;
        else {
            register uint inlineWord __asm__("v0")=raw|0x102;
            __asm__("sw %1,%0" : "=m"(updated->flags) : "r"(inlineWord));
        }
        func_001FE5B8(D_003DD040,1,0);
        func_0014D1D0(D_002DDED0,0,0.0f);
        break;
    }
    case 9:
        Post(p->secondary,0x339);
        Post(p->primary,0x35C);
        func_00214300(p->primary,0);
        break;
    case 3:case 4:
        p->secondary->actor=p->actor;
        Post(p->secondary,0x337);
        PlayWidgetAnim(p->secondary,0x359);
        SetWidgetAnimPhase(p->secondary,0x359,0.0f);
        break;
    case 2:
        Post(p->secondary,0x339);
        break;
    case 10:
        p->actor=0;p->selected=-1;p->pending=-1;
        Clear(p->secondary);
        Clear(p->primary);
        func_00201F00(p);
        BroadcastGameEvent(D_002DEB30,11,0);
        break;
    case -1:
        if(previous!=10){
            BroadcastGameEvent(D_002DEB30,11,0);
            if(p->primary->shown){SetWidgetState(p->primary,1);SetWidgetRequest(p->primary,1);}
            if(p->secondary->shown){SetWidgetState(p->secondary,1);SetWidgetRequest(p->secondary,1);}
        }
        break;
    }
}

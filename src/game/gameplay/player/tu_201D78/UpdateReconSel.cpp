// UpdateReconSel (0x202D38, 524 instructions): per-frame update of the Sly-only interaction prompt (screen projection of the
// actor's head, proximity/size gain, state machine, input check). MATCH (Lilly/Claude T1613), no register pins.
// Case 9 flag is `!link || link->state == 1` (the game's `beqz link` jumps TO the `li a0,1`); the label before that set
// keeps jump.c from turning it into movn.
typedef int qword __attribute__((mode(TI)));
struct M4 {qword r[4];};
struct CM {char pad0[0x60];M4 mat;};
struct LINK {char pad0[0x54];int state;};
struct ACTOR {char pad0[0xE4];float headOffset,threshold;LINK *link;char padF0[0x18];int state,mode,choice;};
struct DISPLAY {char pad0[0x110];int actorChoice,risingChoice,fallingChoice,nearChoice,emptyChoice;};
struct MODEL {char pad0[0x20];int state;};
struct P {int state;char pad4[0xC];qword origin;char pad20[0x10];int count;ACTOR *items[22];ACTOR *actor;float proximity,sizeGain;int rising;float choiceTime;int savedFlags;void *resource;MODEL *model;DISPLAY *primary,*secondary;int refreshPending,visible,selected,pending;};
struct CLOCK {float time,dt;};
struct PLAYER {char pad0[0x150];void *control;};
struct VU_FLOAT {qword qw;};
extern CM D_002DDF40;
extern CLOCK D_002DDED0;
extern PLAYER *D_002E1E40;
extern char D_002DE2E0[];
extern "C" {
void EndJtMode(MODEL *);
void StopWidgetAnim(DISPLAY *,int);
void SelectReconTarget(P *);
void func_002020C8(P *);
int func_002022D8(P *);
void SetReconSelState(P *,int);
qword func_001A62E8(ACTOR *);
qword ConvertCmWorldToScreen(CM *,qword);
void SetWidgetAnimPhase(DISPLAY *,int,float);
int FWidgetAnimPlaying(DISPLAY *,int);
void PlayWidgetAnim(DISPLAY *,int);
int FReconTargetDone(ACTOR *);
int FActiveCplcy__FP5CPLCY(void *);
int GrfConsumePressed__5POCTLUif(void *,int,float);
}
static inline VU_FLOAT VuFloatFromMemWord(int n){VU_FLOAT v;__asm__("qmtc2 %1,%0":"=j"(v.qw):"rV"(n));return v;}
static inline qword QwScaleX(qword a,qword s){qword r;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(a),"j"(s));return r;}
static inline qword QwAdd(qword a,qword b){qword r;__asm__("vadd.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword QwSub(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword QwLengthXY(qword v){qword r;__asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y":"=j"(r):"j"(v));__asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(r):"0"(r));return r;}
// camera up row; the pointer parameter gives the game's &cm+0x60 base register (lqc2 0x10(s1), then s1-0x60 for the calls)
static inline qword QwUp(const M4 *m){return m->r[1];}
static inline float GXOfQw(qword qw){float x;__asm__("mtc1 %1,%0":"=f"(x):"r"(qw));return x;}
static inline float GYOfQw(qword qw){float y;__asm__("prot3w %0,%1":"=r"(y):"r"(qw));return y;}
static inline float GZOfQw(qword qw){float z;__asm__("pextuw %0,$0,%1":"=r"(z):"r"(qw));return z;}
// screen position of a projected point: x and y from [-1,1] to pixels, z kept
static inline VU_FLOAT ScreenFromProjected(qword q)
{
    VU_FLOAT v;
    int t;
    float x=GXOfQw(q)*320.0f+320.0f;
    float y=GYOfQw(q)*-246.40000915527344f+246.40000915527344f;
    float z=GZOfQw(q);
    __asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0":"=&r"(v.qw),"=&r"(t):"f"(x),"f"(y),"f"(z));
    return v;
}
static inline float Min(float a,float b){return a<b?a:b;}
static inline float Max(float a,float b){return a<b?b:a;}
static inline float GClamp(float g,float gMin,float gMax){return Max(gMin,Min(g,gMax));}
/** @sly2-annot addr=0x202D38
 * Purpose: per frame: project target head to screen, proximity to centre / size gain, drive states, shutter
 *   on button 10 (250 when locked)
 * Info: proposed name UpdateReconSel (T1); TU tu_201D78
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/player.md#mode-0x19cbf8-0x19de10,
 *   docs/reference/functions/player.md#tu_201d78-0x201d78-0x203948
 * @end-sly2-annot */
extern "C" void UpdateReconSel(P *p)
{
    if(!p->primary)return;
    if(!p->secondary)return;
    if(p->refreshPending){p->refreshPending=0;EndJtMode(p->model);}
    if(p->choiceTime+5.0f<D_002DDED0.time)StopWidgetAnim(p->primary,0x35B);
    if(p->state<8)if(p->state>=2){
        SelectReconTarget(p);
        func_002020C8(p);
        if(!func_002022D8(p))SetReconSelState(p,1);
    }
    if(p->actor){
        qword world=func_001A62E8(p->actor);
        VU_FLOAT offset=VuFloatFromMemWord(*(int *)&p->actor->headOffset);
        qword head=QwAdd(world,QwScaleX(QwUp(&D_002DDF40.mat),offset.qw));
        qword base=ConvertCmWorldToScreen(&D_002DDF40,world);
        qword top=ConvertCmWorldToScreen(&D_002DDF40,head);
        VU_FLOAT screen=ScreenFromProjected(base);
        float distance=GXOfQw(QwLengthXY(QwSub(screen.qw,p->origin)));
        VU_FLOAT screenTop=ScreenFromProjected(top);
        float size=GXOfQw(QwLengthXY(QwSub(screen.qw,screenTop.qw)));
        float proximity=1.0f-(distance-128.0f)/128.0f;
        size=size/120.0f;
        p->rising=1;
        p->proximity=GClamp(proximity,0.0f,1.0f);
        p->sizeGain=size;
        if(size>1.0f){p->rising=0;p->sizeGain=2.0f-size;}
        p->sizeGain=Max(p->sizeGain,0.0f);
    }
    switch(p->state){
    case -1:
        break;
    case 2:
        if(p->actor&&p->actor->state!=2)SetReconSelState(p,4);
        break;
    case 4:
        if(!p->actor)SetReconSelState(p,2);
        else if(p->actor->state==3)SetReconSelState(p,3);
        else if(p->proximity>=1.0f){
            if(p->rising)SetReconSelState(p,5);
            else SetReconSelState(p,6);
        }
        break;
    case 3:
        if(!p->actor)SetReconSelState(p,2);
        else if(p->actor->state!=3)SetReconSelState(p,4);
        break;
    case 5:
        if(!p->actor)SetReconSelState(p,2);
        else if(p->proximity<1.0f)SetReconSelState(p,4);
        else if(p->sizeGain>=1.0f-p->actor->threshold)SetReconSelState(p,7);
        else SetWidgetAnimPhase(p->secondary,0x35A,p->sizeGain);
        break;
    case 6:
        if(!p->actor)SetReconSelState(p,2);
        else if(p->proximity<1.0f)SetReconSelState(p,4);
        else if(p->sizeGain>=1.0f-p->actor->threshold)SetReconSelState(p,7);
        else SetWidgetAnimPhase(p->secondary,0x35A,p->sizeGain);
        break;
    case 7:
        if(p->actor){
            if(p->proximity<1.0f||p->sizeGain<1.0f-p->actor->threshold){
                if(p->rising)SetReconSelState(p,5);
                else SetReconSelState(p,6);
            }
        }
        else SetReconSelState(p,2);
        break;
    case 8:
        if(!FWidgetAnimPlaying(p->primary,0x358)){
            int changed=0;
            if(p->actor&&p->actor->state==1&&p->proximity>=1.0f&&p->sizeGain>=1.0f-p->actor->threshold){
                if(!p->actor->mode)SetReconSelState(p,9);
                else{p->pending=p->actor->choice;SetReconSelState(p,7);changed=1;}
            }
            else{
                if(p->actor&&p->actor->state!=1){p->pending=p->primary->actorChoice;SetReconSelState(p,3);}
                else if(p->proximity>=1.0f&&p->rising){p->pending=p->primary->risingChoice;SetReconSelState(p,5);}
                else if(p->proximity>=1.0f&&!p->rising){p->pending=p->primary->fallingChoice;SetReconSelState(p,6);}
                else if(p->actor){p->pending=p->primary->nearChoice;SetReconSelState(p,4);}
                else{p->pending=p->primary->emptyChoice;SetReconSelState(p,2);}
                changed=1;
            }
            if(changed){PlayWidgetAnim(p->primary,0x35B);p->choiceTime=D_002DDED0.time;}
        }
        break;
    case 9:
        if(!FWidgetAnimPlaying(p->primary,0x35C)){
            int ready=1;
            SetReconSelState(p,2);
            for(int i=0;i<p->count;i++){
                ACTOR *actor=p->items[i];
                int done=FReconTargetDone(actor);
                // an actor whose link is missing or in state 1 must be done before the refresh
                int linkReady=!actor->link||actor->link->state==1;
                if(!done&&linkReady){ready=0;break;}
            }
            if(ready)p->refreshPending=1;
        }
        break;
    case 10:
        if(!FWidgetAnimPlaying(p->primary,0x314))SetReconSelState(p,-1);
        break;
    case 0:
        if(FWidgetAnimPlaying(p->primary,0x314))SetReconSelState(p,1);
        break;
    case 1:
        func_002020C8(p);
        if(func_002022D8(p))SetReconSelState(p,2);
        break;
    }
    if(p->model->state!=1){
        switch(p->state){
        case 2:case 3:case 4:case 5:case 6:
            if(!FActiveCplcy__FP5CPLCY(D_002DE2E0)&&GrfConsumePressed__5POCTLUif(D_002E1E40->control,10,0.0f))SetReconSelState(p,8);
            break;
        case 7:
            if(!FActiveCplcy__FP5CPLCY(D_002DE2E0)&&GrfConsumePressed__5POCTLUif(D_002E1E40->control,250,0.0f))SetReconSelState(p,8);
            break;
        }
    }
}

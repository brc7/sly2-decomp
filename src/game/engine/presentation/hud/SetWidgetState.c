struct S {
    char pad[0x58]; int state; float time; int tick; int mode; float update;
    char pad1[4]; int id0; int id1; int id2; int selected; int x80; void *sprite; float animtime; float duration;
    char pad2[0xA4-0x90]; int flag; char pad3[8]; int pauses;
};
struct G { char pad[0x3BEC]; int count; };
struct Clock { char pad[0x3C]; float time; float dt; };
extern struct G D_003DD490;
extern struct Clock D_002DDED0;
extern int D_002DD318;
extern int D_002BE538;
void func_0014D1D0(struct Clock *,int,float);
void LoadWidgetSprites(struct S *);
void UnloadWidgetSprites(struct S *);
void StopWidgetMainAnim(struct S *);
void StartWidgetMainAnim(struct S *,int);
/** @sly2-annot addr=0x212A70
 * Purpose: state machine 0 unloaded,1 hidden,2 visible(loop),3 appearing,4 disappearing; counts SW+0x3BEC,
 *   pauses clock when fPause
 * Info: proposed name SetWidgetState (T1); TU hud
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r7c_hud
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void SetWidgetState(struct S *p,int state) {
    if(p->state==state) return;
    if(p->flag>=16) {
        if(p->state<2 && state>=2) D_003DD490.count++;
        else if(state<2 && p->state>=2) D_003DD490.count--;
    }
    if(p->pauses) {
        if(p->state<2 && state>=2) {
            if(++D_002DD318==1) func_0014D1D0(&D_002DDED0,7,0.0f);
        } else if(state<2 && p->state>=2) {
            if(--D_002DD318==0) func_0014D1D0(&D_002DDED0,7,1.0f);
        }
    }
    if(p->state==0) LoadWidgetSprites(p);
    p->state=state;
    p->time=D_002DDED0.time;
    p->tick=D_002BE538;
    switch(state) {
    case 0: UnloadWidgetSprites(p); break;
    case 1: StopWidgetMainAnim(p); break;
    case 2: StartWidgetMainAnim(p,p->id0); break;
    case 3: StartWidgetMainAnim(p,p->id1); break;
    case 4: StartWidgetMainAnim(p,p->id2); break;
    }
}

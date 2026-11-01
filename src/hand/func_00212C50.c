struct S {
    char pad[0x58]; int state; float time; int tick; int mode; float update;
    char pad1[4]; int id0; int id1; int id2; int selected; int x80; void *sprite; float animtime; float duration;
    char pad2[0xA4-0x90]; int flag; char pad3[8]; int pauses;
};
struct Clock { char pad[0x3C]; float time; float dt; };
extern struct Clock D_002DDED0;
extern float D_002DDF10;
void func_001A5E28(struct S *);
int func_002121E8(struct S *,int);
void func_00212A50(struct S *,int);
void func_00212A70(struct S *,int);
void func_00218220(void *,float);
/** @sly2-annot addr=0x212C50
 * Purpose: WIDGET Update: drives SetWidgetState from request/anim completion and duration, advances main
 *   sprite
 * Info: proposed name UpdateWidget (T1); class WIDGET slot16; TU hud
 * Notes: names_v2 v1 proposed FUN_001abe70 (0.000) | runtime: WIDGET +0x58 state 1 hidden / 2 visible
 *   confirmed
 * Confidence: high [T1 0.90] - evidence: runtime (save-state RAM); static naming pass; sources:
 *   r7c_hud+runtime
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00212C50(struct S *p) {
    int state;
    func_001A5E28(p);
    state=p->state;
    if(state==1) {
        if(p->mode==2) state=p->id1==-1 ? 2:3;
    } else if(p->update != D_002DDED0.time) {
        switch(state) {
        case 2:
            if(!func_002121E8(p,p->selected)) func_00212A50(p,1);
            if(p->duration>0.0f && p->duration<D_002DDED0.time-p->time) func_00212A50(p,1);
            if(p->mode==1) state=p->id2==-1 ? 1:4;
            break;
        case 3:
            if(!func_002121E8(p,p->id1)) {
                switch(p->mode) {
                case 1: state=p->id2==-1 ? 1:4; break;
                case 2: state=2; break;
                }
            }
            break;
        case 4:
            if(!func_002121E8(p,p->id2)) {
                switch(p->mode) {
                case 1: state=1; break;
                case 2: state=p->id1==-1 ? 2:3; break;
                }
            }
            break;
        }
    }
    func_00212A70(p,state);
    if(p->selected!=-1) {
        float dt=D_002DDF10;
        p->animtime+=dt;
        func_00218220(p->sprite,dt);
    }
}

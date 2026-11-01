struct S {
    char pad[0x58]; int state; float time; int tick; int mode; float update;
    char pad1[4]; int id0; int id1; int id2; int selected; int x80; void *sprite; float animtime; float duration;
    char pad2[0xA4-0x90]; int flag; char pad3[8]; int pauses;
};
struct Clock { char pad[0x3C]; float time; float dt; };
extern struct Clock D_002DDED0;
extern float D_002DDF10;
void UpdateLo(struct S *);
int func_002121E8(struct S *,int);
void func_00212A50(struct S *,int);
void func_00212A70(struct S *,int);
void func_00218220(void *,float);
void func_00212C50(struct S *p) {
    int state;
    UpdateLo(p);
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

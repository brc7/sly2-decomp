// func_00164DF0: object state change (applies overrides to gathered children, then notifies)
struct P{char pad[0x54];int state;char pad58[0x24];void*link;char pad80[0x14];int enabled;char pad98[8];int count;P**children;char padA8[0x48];int pending;int x,y;};
struct CONTEXT{char pad[0xF0];int x,y;};
struct REQUEST{int bytes,alignment;REQUEST(int cb,int a):bytes(cb),alignment(a){}};
struct G{char pad[0x3B7C];void*players[9];};
struct CRef;
struct HEAP_SCOPE;
extern "C"{
extern int D_002DEB28,D_003E0F8C;
extern char D_002DEB30[],D_002E65E0[],D_003D4A60[];
extern G D_003DD490;
HEAP_SCOPE*func_001AE478(HEAP_SCOPE*);
void*func_001AE378(const REQUEST&);
void func_001AE4A0(HEAP_SCOPE*,int);
void func_00164CE8(P*,int,int*,P**);
void func_00165A60(P*,int);
int func_00210AE0(void);
int func_00161660(void*);
int func_001637E8(P*);
void func_00162CD0(void*,P*);
void func_001ABD98(void*,int);
CRef*func_001291A8(CRef*,const CRef*);
void func_00129EC8(CRef*);
}
static inline void*Allocate(int n){return func_001AE378(REQUEST(n,16));}
struct HEAP_SCOPE{HEAP_SCOPE(){func_001AE478(this);}~HEAP_SCOPE(){func_001AE4A0(this,2);}};
struct OVERRIDE{CONTEXT*context;P*object;int oldX,oldY;
    OVERRIDE(P*p,int x,int y):context(0),object(p){if(object){oldX=object->x;object->x=x;oldY=object->y;object->y=y;}}
    ~OVERRIDE(){if(context){context->x=oldX;context->y=oldY;}if(object){object->x=oldX;object->y=oldY;}}
};
struct CRef{int kind,value;CRef(int v){kind=0x15;value=v;}CRef(const CRef&r){func_001291A8(this,&r);}~CRef(){func_00129EC8(this);}};
extern "C" CRef func_00258200(void*,const CRef&,void*);
static inline void SetPv(void*object,const CRef&ref,void*){CRef arg(ref);const CRef&result=func_00258200(object,arg,D_003D4A60);}
static inline void SetValue(void*object,int n){CRef r(n);SetPv(object,r,D_003D4A60);}
/** @sly2-annot addr=0x164DF0
 * Purpose: Main task state change: propagates to collected successors, sets the state, updates the episode
 *   percent save var, handles job start/end tasks, autosaves
 * Info: proposed name SetTaskStateCascade (T1); TU game
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
extern "C" void func_00164DF0(P*p,int state){
    int old=p->state;
    if(state==old)return;
    if(D_002DEB28)return;
    D_002DEB28=1;
    int filter=-1;
    switch(state){case 0:break;case 1:case 2:filter=2;break;case 3:filter=3;break;}
    if(filter!=-1){
        int i=0;
        HEAP_SCOPE scope;
        P**list;
        int count=0;
        list=(P**)Allocate(D_003E0F8C*4);
        p->pending=state;
        func_00164CE8(p,filter,&count,list);
        for(;i<count;i++){
            P*object=list[i];
            OVERRIDE temporary(object,p->x,p->y);
            func_00165A60(object,object->pending);
        }
        for(int j=0;j<count;j++)list[j]->pending=-1;
        p->pending=-1;
    }
    func_00165A60(p,state);
    D_002DEB28=0;
    int mode=func_00210AE0();
    if((unsigned)mode<9){
        void*object=D_003DD490.players[mode];
        if(object){SetValue(object,func_00161660(D_002DEB30));}
    }
    if(func_001637E8(p)&&old!=-1&&old<2&&p->state>=2){
        if(p->link){func_00162CD0(p->link,p);if(p->link)return;}
        if(p->enabled){
            int all=1;
            for(int i=0;i<p->count;i++){if(!p->children[i]->link){all=0;break;}}
            if(!all)return;
        }
        func_001ABD98(D_002E65E0,0);
    }
}

// Load prioritized sprite files.
struct FRAME {};
typedef struct {int cb,alignment;} MSG;
struct BIS {int grf;char unknown4[0x2C];void *vt;int cbSpool;char *abSpool;int x3c,x40,x44,x48,x4c,x50,x54,x58,x5c,x60;};
typedef struct {int a,b,kind;char filename[0x104];} KEY;
typedef union {MSG msg;char magic[4];} TMP;
typedef struct S S;
typedef struct N {int a,b;char pad8[0x2C];int c;int pad38;int d;S *owner;int priority,last;struct N *next;} N;
struct S {char pad[0x50];int count;void *data;int state;char pad5c[4];int tick;char pad64[0x5C];N *file;};
typedef struct {char pad[0xC];int priority;} POOL;
extern "C" FRAME *STACKSCOPE_ctor(FRAME *);
extern "C" void STACKSCOPE_ctor_1AE4A0(FRAME *,int);
extern "C" BIS *func_001848A8(BIS *,int,void *,int);
extern "C" void func_00184950(BIS *,int);
extern "C" void func_00258248(KEY *);
extern "C" void *func_001AE318(MSG *);
extern "C" int func_00184D60(BIS *,KEY *);
extern "C" void func_001179D8(BIS *,int,void *);
extern "C" unsigned char func_00117BD0(BIS *);
extern "C" unsigned short func_00117C20(BIS *);
extern "C" int strncmp(const void *,const void *,unsigned int);
extern "C" void SetWidgetState(S *,int);
extern "C" void LoadSpriteTable(S *,BIS *,S *,int *,void **);
extern "C" void LoadSpriteGroups(S *,BIS *,S *);
extern N *D_003E106C;
extern POOL D_003E1C70;
extern int D_002BE538;
extern char D_002A8AE8[];
static inline int Max(int a,int b) {return a<b?b:a;}
static inline int SetKey(KEY *k,N *n) {
    if(!n)return 0;
    {int a=n->c,b=n->a,c=n->d,d=n->b;
    k->kind=2;
    k->a=(b^a)+0xCA0;
    k->b=d^c;}
    return k->b!=0;
}
struct WORK {
 FRAME frame;
 TMP tmp __attribute__((aligned(16)));
 BIS stream __attribute__((aligned(16)));
 KEY key __attribute__((aligned(16)));
};
/** @sly2-annot addr=0x2124F0
 * Purpose: deferred (streamed) sprite file load when a widget leaves state 0
 * Info: proposed name LoadWidgetSprites (T1); TU hud
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7c_hud
 * See: docs/formats/textures.md#7-open, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
extern "C" void LoadWidgetSprites(S *p) {
    WORK work;
    register BIS *bs asm("$17");asm("la %0,%a1":"=r"(bs):"p"(&work.stream));
    int priority=0;
    N *n;
    S *owner;
    int version;
    for(n=D_003E106C;n;n=n->next) {
        owner=n->owner;
        if(owner->state==0)continue;
        if(owner->state==1 && D_002BE538-owner->tick>=2)continue;
        priority=Max(priority,n->last);
    }
    for(n=D_003E106C;n;n=n->next) {
        owner=n->owner;
        if(n->priority>=priority && owner->state!=0)SetWidgetState(owner,0);
    }
    D_003E1C70.priority=priority;
    STACKSCOPE_ctor(&work.frame);
    KEY *pk=&work.key;
    MSG *msg=&work.tmp.msg;

    msg->cb=0x10000;msg->alignment=0x40;
    func_001848A8(bs,0x10000,func_001AE318(msg),0);
    pk->kind=2;
    func_00258248(pk);
    if(!SetKey(&work.key,p->file)||!func_00184D60(bs,pk))goto fail;
    func_001179D8(bs,0x4000,(void *)0x70000000);
    work.tmp.magic[0]=func_00117BD0(bs);
    work.tmp.magic[1]=func_00117BD0(bs);
    work.tmp.magic[2]=func_00117BD0(bs);
    work.tmp.magic[3]=func_00117BD0(bs);
    version=func_00117C20(bs);
    if(!strncmp(work.tmp.magic,D_002A8AE8,4)&&version==6)goto load;
fail:
    func_00184950(bs,2);
    STACKSCOPE_ctor_1AE4A0(&work.frame,2);
    return;
load:
    p->file->priority=D_003E1C70.priority;
    LoadSpriteTable(p,bs,p,&p->count,&p->data);
    LoadSpriteGroups(p,bs,p);
    p->file->last=D_003E1C70.priority;
    func_00184950(bs,2);
    STACKSCOPE_ctor_1AE4A0(&work.frame,2);
}

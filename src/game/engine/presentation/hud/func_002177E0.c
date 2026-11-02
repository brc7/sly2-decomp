// Queue transformed sprite parameters and a direct GIF payload.
typedef unsigned int uint;
typedef unsigned long ulong;
struct CAMERA {char pad[0x9C];float x,y;};
struct OWNER {char pad[0xA4];int priority;char pada8[0x20];struct CAMERA *camera;};
struct GEO {char pad[0x18];int count;void *data;};
struct S {int pad,type;struct OWNER *owner;char padc[8];union {struct GEO *geo;int raw;};int rawtag;union {int tag;int count;};void *buffer;};
struct T {float a,b,c,d,x,y;};
struct M4 {float a[16];};
struct D {int a,mode;float priority;uint limit;char pad10[0x1C];float fade;};
struct R {char pad[0xC];unsigned char *cur;};
struct E {int kind;float priority;uint address;int tag;};
extern struct R D_002DEA28;
void *func_002333B4(void *,int,int);
struct E *PrplAllocRpl(void);
static inline void Word(struct R *r,uint word){*(uint *)r->cur=word;r->cur+=4;}
static inline void Align(struct R *r){while((uint)r->cur&15){*(uint *)r->cur=0;r->cur+=4;}}
static inline void CopyWords(int *d,const int *s,int n){int rem=n&3,cBulk=n-rem,i;for(i=0;i<rem;i++)*d++=*s++;for(i=0;i<cBulk;i+=4){int a=s[0],b=s[1],c=s[2],e=s[3];s+=4;d[0]=a;d[1]=b;d[2]=c;d[3]=e;d+=4;}}
static inline void Unpack(struct R *r,const int *s,int n,uint header){Word(r,header);CopyWords((int *)r->cur,s,n);r->cur+=n*4;}

void func_001AE528(void *,void *,int);
static inline void AlignTo(struct R *r,int word){while(((uint)r->cur&15)!=4*word)Word(r,0);}
static inline void Direct(struct R *r,int n,void *source){AlignTo(r,3);Word(r,0x50000000|n);func_001AE528(r->cur,source,n);r->cur+=n*16;}
/** @sly2-annot addr=0x2177E0
 * Purpose: transformed sprite parameters + direct GIF payload
 * Info: proposed name RenderSpritePicture (T2); TU hud
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_002177E0(struct S *p,struct T *t,struct D *data,int unused){
    struct M4 m;
    unsigned char *tag;
    struct E *e;
    switch(p->type){
    case 6:if(!p->geo)return;break;
    case 7:if(!p->count)return;break;
    default:return;
    }
    func_002333B4(&m,0,0x40);
    m.a[0]=t->a*0.8f;m.a[1]=t->b*0.45454547f;
    m.a[4]=t->c*0.8f;m.a[5]=t->d*0.45454547f;
    m.a[8]=t->x*0.8f+p->owner->camera->x+786432.0f;
    m.a[9]=t->y*0.45454547f+p->owner->camera->y+786432.0f;
    m.a[15]=m.a[10]=1.0f;
    data->mode=0;data->limit=0x7FFF;
    tag=D_002DEA28.cur;
    *(ulong *)tag=0x10000000UL;D_002DEA28.cur+=16;
    Unpack(&D_002DEA28,(const int *)&m,16,0x6C04C000);
    Unpack(&D_002DEA28,(const int *)data,12,0x6C03C004);
    if(p->type==6)Direct(&D_002DEA28,p->geo->count,p->geo->data);
    else Direct(&D_002DEA28,p->count,p->buffer);
    Align(&D_002DEA28);
    *(uint *)tag|=((D_002DEA28.cur-tag)>>4)-1;
    *(ulong *)D_002DEA28.cur=0x60000000UL;D_002DEA28.cur+=16;
    e=PrplAllocRpl();
    e->kind=0x15;e->priority=data->priority+(p->owner->priority<<18);
    e->address=(uint)tag&0x0FFFFFFF;
    if(p->type==6)e->tag=p->tag;else e->tag=p->rawtag;
}

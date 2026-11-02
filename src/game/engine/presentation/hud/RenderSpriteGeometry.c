// Queue transformed sprite parameters with optional fade limiting.
typedef unsigned int uint;
typedef unsigned long ulong;
struct CAMERA {char pad[0x9C];float x,y;};
struct OWNER {char pad[0xA4];int priority;char pada8[0x20];struct CAMERA *camera;};
struct S {char pad[8];struct OWNER *owner;char padc[6];unsigned short mode;int count;char pad18[4];int tag;};
struct T {float a,b,c,d,x,y;};
struct M4 {float a[16];};
struct D {int a,mode;float priority;uint limit;char pad10[0x1C];float fade;};
struct R {char pad[0xC];unsigned char *cur;};
struct E {int kind;float priority;uint address;int tag;};
extern struct R D_002DEA28;
void *func_002333B4(void *,int,int);
struct E *PrplAllocRpl(void);
static inline int Min(int a,int b){return a<b?a:b;}
static inline void Word(struct R *r,uint word){*(uint *)r->cur=word;r->cur+=4;}
static inline void Align(struct R *r){while((uint)r->cur&15){*(uint *)r->cur=0;r->cur+=4;}}
static inline void CopyWords(int *d,const int *s,int n){int rem=n&3,cBulk=n-rem,i;for(i=0;i<rem;i++)*d++=*s++;for(i=0;i<cBulk;i+=4){int a=s[0],b=s[1],c=s[2],e=s[3];s+=4;d[0]=a;d[1]=b;d[2]=c;d[3]=e;d+=4;}}
static inline void Unpack(struct R *r,const int *s,int n,uint header){Word(r,header);CopyWords((int *)r->cur,s,n);r->cur+=n*4;}
/** @sly2-annot addr=0x2174C8
 * Purpose: transformed sprite parameters with optional fade
 * Info: proposed name RenderSpriteGeometry (T1); TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void RenderSpriteGeometry(struct S *p,struct T *t,struct D *data,int fade){
    struct M4 m;
    unsigned char *tag;
    struct E *e;
    func_002333B4(&m,0,0x40);
    m.a[0]=t->a*0.8f;m.a[1]=t->b*0.45454547f;
    m.a[4]=t->c*0.8f;m.a[5]=t->d*0.45454547f;
    m.a[8]=t->x*0.8f+p->owner->camera->x+786432.0f;
    m.a[9]=t->y*0.45454547f+p->owner->camera->y+786432.0f;
    m.a[15]=m.a[10]=1.0f;
    if(p->mode>=2)data->mode=1;else data->mode=0;
    data->limit=0x7FFF;
    if(fade){
        float old=data->fade;
        data->fade=1.0f;
        if(old<1.0f){
            int n=Min((int)(p->count*old),p->count);
            data->limit=n>=3?n:3;
            if(data->limit>0x7FFFU)data->limit=0x7FFF;
        }
    }
    tag=D_002DEA28.cur;
    *(ulong *)tag=0x10000000UL;D_002DEA28.cur+=16;
    Unpack(&D_002DEA28,(const int *)&m,16,0x6C04C000);
    Unpack(&D_002DEA28,(const int *)data,12,0x6C03C004);
    Align(&D_002DEA28);
    *(uint *)tag|=((D_002DEA28.cur-tag)>>4)-1;
    *(ulong *)D_002DEA28.cur=0x60000000UL;D_002DEA28.cur+=16;
    e=PrplAllocRpl();
    e->kind=0x15;e->priority=data->priority+(p->owner->priority<<18);
    e->address=(uint)tag&0x0FFFFFFF;
    e->tag=p->tag;
}

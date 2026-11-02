// Configure rich-text rendering and queue the standard render payloads.
typedef unsigned int uint;
typedef unsigned long ulong;
typedef int qword __attribute__((mode(TI)));
typedef struct {unsigned char r,g,b,a;} RGBA;
struct RT {char pad[0x6C];float offsetX,offsetY,width,height,sx,sy,lineHeight,fontScale;int vAlign,hAlign,priority;};
struct CONFIG {float offsetX,offsetY,width,height;RGBA color;int hAlign,vAlign;};
extern "C" void *func_002333B4(void *,int,int);
struct M23 {float a[8];enum INIT {Uninitialized,Zero};M23(INIT init=Uninitialized){if(init==Zero)func_002333B4(this,0,sizeof(*this));}} __attribute__((aligned(16)));
struct M4 {qword r[4];};
struct DATA {RGBA color;int mode;float priority;int limit;qword v10,v20;};
struct R {char pad[0xC];unsigned char *cur;};
struct E {int kind;float priority;uint address;int tag;};
extern struct R D_002DEA28;
extern char D_00442210[],D_002DF010[];
extern "C" void CopyAqw__FPvT0i(void *,void *,int);
extern "C" struct E *PrplAllocRpl(void);
static inline void Word(struct R *r,uint word){*(uint *)r->cur=word;r->cur+=4;}
static inline void Align(struct R *r,int word){while(((uint)r->cur&15)!=4*word)Word(r,0);}
static inline void Bulk(struct R *r,int n,void *source){CopyAqw__FPvT0i(r->cur,source,n);r->cur+=n*16;}
static inline void Direct(struct R *r,int n,void *source){Align(r,3);Word(r,0x50000000|n);Bulk(r,n,source);}

extern "C" void func_00215D00(struct RT *,RGBA *);
extern "C" void func_002162D8(struct RT *,struct M4 *,struct M23 *);
extern "C" void func_00216750(struct RT *,struct M4 *,struct DATA *);
extern qword D_002A2E10,D_002A2E20;
/** @sly2-annot addr=0x2163B0
 * Purpose: configure box/alignment/colour and queue rich text
 * Info: proposed name CRichText2_Draw (T2); class CRichText; TU hud
 * Notes: names_v2 B proposed DrawTitle__FP5TITLE (0.932) | COLLISION: CRichText_Draw already used at 0x15FFD0
 *   (existing symbol); renamed CRichText_Draw_2163B0 | v3: superseded CRichText_Draw_2163B0 | top50 finding
 *   8: hud-TU rich text (colours +0x14/+0x18) is a second class; Reset__9CRichText 0x15F970 keeps CRichText.
 *   CRichText2 is a placeholder class word (T2); resolves the v2 T3 collision | T2: placeholder token
 *   CRichText2
 * Confidence: low (draft) [T2 0.50] - evidence: semantic review; static naming pass; sources:
 *   r7c_hud+top50_round(CRichText2 class split)
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
extern "C" void func_002163B0(struct RT *p,struct CONFIG *config,int priority){
    RGBA color;
    unsigned char *tag;
    struct E *e;
    if(!config)return;
    p->offsetX=config->offsetX;p->offsetY=config->offsetY;p->width=config->width;p->height=config->height;
    p->hAlign=config->hAlign;p->vAlign=config->vAlign;p->priority=priority;
    color=config->color;func_00215D00(p,&color);
    struct M23 matrix(M23::Zero);
    struct M4 world;
    struct DATA style;
    matrix.a[0]=p->sx*p->fontScale;
    matrix.a[3]=p->sy*p->fontScale;
    matrix.a[4]=p->offsetX;
    matrix.a[5]=p->offsetY+p->lineHeight*p->sy;
    func_002162D8(p,&world,&matrix);
    style.v10=D_002A2E10;style.v20=D_002A2E20;style.priority=16.0f;
    func_00216750(p,&world,&style);
    tag=D_002DEA28.cur;
    *(ulong *)tag=0x10000000UL;D_002DEA28.cur+=16;
    Word(&D_002DEA28,0x11000000);
    Direct(&D_002DEA28,3,D_00442210);
    Direct(&D_002DEA28,0x25,D_002DF010);
    Word(&D_002DEA28,0x11000000);
    Align(&D_002DEA28,0);
    *(uint *)tag|=((D_002DEA28.cur-tag)>>4)-1;
    *(ulong *)D_002DEA28.cur=0x60000000UL;D_002DEA28.cur+=16;
    e=PrplAllocRpl();
    e->kind=0x15;e->priority=p->priority<<18;
    e->address=(uint)tag&0x0FFFFFFF;e->tag=0;
}

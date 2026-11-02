// Queue the render initialization payloads in a DMA packet.

typedef unsigned int uint;
typedef unsigned long ulong;
struct S {char pad[0xA4];int priority;};
struct R {char pad[0xC];unsigned char *cur;};
struct E {int kind;float priority;uint address;int tag;};
extern struct R D_002DEA28;
extern char D_00442210[],D_002DF010[];
void func_001AE528(void *,void *,int);
struct E *PrplAllocRpl(void);
static inline void Word(struct R *r,uint word){*(uint *)r->cur=word;r->cur+=4;}
static inline void Align(struct R *r,int word){while(((uint)r->cur&15)!=4*word)Word(r,0);}
static inline void Bulk(struct R *r,int n,void *source){func_001AE528(r->cur,source,n);r->cur+=n*16;}
static inline void Direct(struct R *r,int n,void *source){Align(r,3);Word(r,0x50000000|n);Bulk(r,n,source);}
/** @sly2-annot addr=0x2130C0
 * Purpose: DMA packet with render init payloads (frame/mask GIF data) before widget sprites
 * Info: proposed name QueueWidgetRenderSetup (T1); TU hud
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r7c_hud+verdicts_followups
 * See: docs/engine/hud-ui.md#5-key-functions, docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void QueueWidgetRenderSetup(struct S *p){
    unsigned char *tag=D_002DEA28.cur;
    struct E *e;
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

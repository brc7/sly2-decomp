// Queue the selected VU microprogram pair in a DMA packet.
typedef unsigned int uint;
typedef unsigned long ulong;
struct R {char pad[0xC];unsigned char *cur;};
struct S {char pad[0xA4];int priority;};
struct E {int kind;float priority;uint address;int pad;};
extern struct R D_002DEA28;
extern char D_00000000[],D_00000030[],D_00000060[],D_00000090[],D_000000C0[],D_00000100[];
struct E *func_001CD998(void);
static inline void Word(struct R *r,uint word){*(uint *)r->cur=word;r->cur+=4;}
static inline void Mscal(struct R *r,void *entry){Word(r,0x14000000|((uint)entry>>3));}
static inline void Align(struct R *r){while((uint)r->cur&15){*(uint *)r->cur=0;r->cur+=4;}}
/** @sly2-annot addr=0x217298
 * Purpose: queue VU microprogram pair for render mode
 * Info: proposed name QueueSpriteVuProgram (T2); TU hud
 * Notes: names_v2 C proposed DrawLogo__FP4LOGO (0.638)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00217298(struct S *p,int mode,float priority){
    unsigned char *tag=D_002DEA28.cur;
    struct E *e;
    *(ulong *)tag=0x10000000UL;
    D_002DEA28.cur+=16;
    switch(mode){
    case 1:
        Word(&D_002DEA28,0x13000000);Mscal(&D_002DEA28,D_00000000);Mscal(&D_002DEA28,D_000000C0);Word(&D_002DEA28,0x13000000);break;
    case 2:
        Word(&D_002DEA28,0x13000000);Mscal(&D_002DEA28,D_00000030);Mscal(&D_002DEA28,D_000000C0);Word(&D_002DEA28,0x13000000);break;
    case 3:
        Word(&D_002DEA28,0x13000000);Mscal(&D_002DEA28,D_00000100);Mscal(&D_002DEA28,D_00000060);Word(&D_002DEA28,0x13000000);break;
    case 4:
        Word(&D_002DEA28,0x13000000);Mscal(&D_002DEA28,D_00000100);Mscal(&D_002DEA28,D_00000090);Word(&D_002DEA28,0x13000000);break;
    }
    Align(&D_002DEA28);
    *(uint *)tag|=((D_002DEA28.cur-tag)>>4)-1;
    *(ulong *)D_002DEA28.cur=0x60000000UL;
    D_002DEA28.cur+=16;
    e=func_001CD998();
    e->kind=0x15;e->priority=priority+(p->priority<<18);e->address=(uint)tag&0x0FFFFFFF;e->pad=0;
}

// Build a matrix/parameter DMA packet ending in the supplied call tag.
struct R { char pad[0xC]; unsigned char *cur; };
struct S { char pad[0x12]; unsigned short mode; char pad1[8]; void *address; };
struct D { int a; int mode; int b; int xC; char pad[0x20]; };
extern struct R D_002DEA28;
static inline void CopyWords(int *d,const int *s,int n){
    int rem=n&3,cBulk=n-rem,i;
    for(i=0;i<rem;i++)*d++=*s++;
    for(i=0;i<cBulk;i+=4){int a=s[0],b=s[1],c=s[2],e=s[3];s+=4;d[0]=a;d[1]=b;d[2]=c;d[3]=e;d+=4;}
}
static inline void Word(struct R *r,unsigned int word) {
    *(unsigned int *)r->cur=word;
    r->cur+=4;
}
static inline void Align(struct R *r) {
    while((int)r->cur&15) { *(int *)r->cur=0; r->cur+=4; }
}
static inline void Unpack(struct R *r,const int *s,int n,unsigned int header){Word(r,header);CopyWords((int *)r->cur,s,n);r->cur+=n*4;}
/** @sly2-annot addr=0x217058
 * Purpose: matrix/parameter DMA packet ending in a call tag
 * Info: proposed name BuildGlyphPacket (T2); TU hud
 * Notes: names_v2 C proposed DrawTotals__FP6TOTALS (0.438)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r7c_hud
 * See: docs/reference/functions/hud-ui.md#hud-0x210eb0-0x21bb88
 * @end-sly2-annot */
void func_00217058(struct S *p,const int *matrix,struct D *data,int flags) {
    unsigned char *tag=D_002DEA28.cur;
    *(unsigned long *)tag=0x10000000UL;
    D_002DEA28.cur+=16;
    if(flags&1) {
        Unpack(&D_002DEA28,matrix,16,0x6C04C000);
    } else {
        Unpack(&D_002DEA28,matrix+8,4,0x6C01C002);
    }
    data->xC=0x7FFF;
    if(p->mode>=2)data->mode=1;else data->mode=0;
    if((flags&2) || data->mode) {
        Unpack(&D_002DEA28,(const int *)data,12,0x6C03C004);
    }
    Align(&D_002DEA28);
    *(int *)tag |= ((D_002DEA28.cur-tag)>>4)-1;
    Align(&D_002DEA28);
    *(unsigned long *)D_002DEA28.cur=0x50000000UL | ((unsigned long)p->address<<32);
    D_002DEA28.cur+=16;
}

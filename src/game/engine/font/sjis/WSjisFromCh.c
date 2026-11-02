struct GM { unsigned short a; unsigned short b; };
extern struct GM D_002C36A0[];
extern struct GM D_002C3728[];
static inline void Addresses(struct GM*base,unsigned int offset,struct GM**first,struct GM**second){
 register struct GM*a __asm__("$5"),*b __asm__("$3");register unsigned int scaled __asm__("$4")=offset;register struct GM*origin __asm__("$3")=base;
 __asm__("addu %0,%2,%3\n\taddu %1,%3,%2":"=&r"(a),"=r"(b):"r"(scaled),"r"(origin));*first=a;*second=b;
}
static inline int GroupBase(int group){register int r __asm__("$2");__asm__("addiu %0,%1,31":"=r"(r):"r"(group));return r;}
static inline int CharacterOffset(unsigned char ch,int origin){register int r __asm__("$2")=origin;__asm__("subu %0,%1,%0":"+r"(r):"r"(ch));return r;}
static inline struct GM*Element(struct GM*base,int index){register int scaled __asm__("$2")=index;__asm__("sll %0,%0,2\n\taddu %0,%0,%1":"+r"(scaled):"r"(base));return (struct GM*)scaled;}
/** @sly2-annot addr=0x1EE988
 * Purpose: Map one ASCII byte to a 16-bit SJIS code via table
 * Info: proposed name WSjisFromCh (EXISTING); TU sjis
 * Notes: agent/verifier proposes WSjisFromCh for this existing name (see existing_name_changes.csv) | RENAME
 *   proposed -> WSjisFromCh (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.55] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r6_1D5150+verdicts_tu_r4r6+arbitration
 * See: docs/engine/hud-ui.md#3-fonts-language-letterbox-loading-screen, docs/engine/hud-ui.md#4-source-files,
 *   docs/engine/hud-ui.md#5-key-functions
 * @end-sly2-annot */
unsigned short WSjisFromCh(unsigned char ch)
{
    register int i __asm__("$3") = 0;
    register int j __asm__("$5");
    if ((unsigned)(ch - 0x20) < 0x10) i = 1;
    else if ((unsigned)(ch - 0x30) < 10) j = 0;
    else if ((unsigned)(ch - 0x3A) < 7) i = 0xB;
    else if ((unsigned)(ch - 0x41) < 0x1A) j = 1;
    else if ((unsigned)(ch - 0x5B) < 6) i = 0x25;
    else if ((unsigned)(ch - 0x61) < 0x1A) j = 2;
    else if ((unsigned)(ch - 0x7B) < 4) i = 0x3F;
    else return 0;
    if (i == 0) { register unsigned int offset __asm__("$4")=(unsigned)j<<2;struct GM*a,*b;Addresses(D_002C3728,offset,&a,&b);{register unsigned int first __asm__("$2")=a->a;register unsigned int second __asm__("$4")=b->b;return first+ch-second;} } { int origin=GroupBase(i);struct GM*table=D_002C36A0;int index=CharacterOffset(ch,origin);return Element(table,index)->a; }
}

typedef char GM_Size[sizeof(struct GM)==4?1:-1];typedef char GM_B[(unsigned)&((struct GM*)0)->b==2?1:-1];

struct S {int n;};
enum {TABLE_PAGE=0x003E0000};
/** @sly2-annot addr=0x1ED560
 * Purpose: Insert into the clut hash
 * Info: proposed name AddClutHash (T2); TU shd
 * Notes: names_v2 C proposed PropagateSurs__Fv (0.638)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r6b_rest
 * See: docs/formats/textures.md#2-clut-record-0x10-byte-header-cstatic,
 *   docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8
 * @end-sly2-annot */
void func_001ED560(struct S *input){
    struct S *p;
    register int nextIndex __asm__("$3");

    register int initial __asm__("$5");
    register int i __asm__("$4");
    register struct S **volatile *firstPage __asm__("$4");
    register struct S **volatile *loopPage __asm__("$8");
    register struct S **firstTable __asm__("$6");
    register struct S **slot __asm__("$2");
    p=input;
    {register int key __asm__("$3")=p->n;initial=key%0x3FFD;}
    firstPage=(struct S **volatile *)TABLE_PAGE;
    __asm__("lw %0,%3\n\tdaddu %1,%2,$zero":"=&r"(firstTable),"=r"(loopPage):"r"(firstPage),"m"(*(struct S ***)((char*)firstPage-0x2FE8)));
    __asm__("daddu %0,%1,$zero":"=r"(i):"r"(initial));
    slot=(struct S **)(((unsigned)i<<2)+(unsigned)firstTable);
    goto probe;
next:
    i=nextIndex%0x3FFD;
    {
        register struct S **table __asm__("$5");
        __asm__("lw %0,%1":"=r"(table):"m"(*(struct S ***)((char*)loopPage-0x2FE8)));
        slot=(struct S **)(((unsigned)i<<2)+(unsigned)table);
    }
probe:
    if(!*slot)goto insert;
    nextIndex=i+1;goto next;
insert:
    *slot=p;
}

typedef char S_Size[sizeof(struct S)==4?1:-1];
typedef char Table_Pointer_Size[sizeof(struct S**)==4?1:-1];
typedef char Key_Offset[(unsigned)&((struct S*)0)->n==0?1:-1];
typedef char Unsigned_Size[sizeof(unsigned)==4?1:-1];

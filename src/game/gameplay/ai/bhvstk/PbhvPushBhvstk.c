struct VE {short delta,pad;void (*init)(void *);};
struct O {int pad[3];struct VE *vt;int tag;struct O *previous,*next;};
struct STK {int tag,count;void *slots[1];};
int CbBhvMax(void);
void *func_002333B4(void *,int,int);
struct O *PbhvNew(int,void *);
static inline int Count(const int*p){
    register int value __asm__("$2");
    __asm__("lw %0,%1":"=r"(value):"m"(*p));
    return value;
}
/** @sly2-annot addr=0x1EEE00
 * Purpose: Push a new behaviour of class cid: take slot apbhv[cpbhv++], memset CbBhvMax, construct via
 *   PbhvNew, set pstep and pbhvPrev (prev->pbhvNext=new), call vt7 (Init); returns it
 * Info: proposed name PbhvPushBhvstk (T1); TU bhvstk
 * Notes: names_v2 C proposed SetTnTns__FP2TN3TNS (0.438)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r6b_rest
 * See: docs/reference/functions/guards-ai.md#bhvstk-0x1eec60-0x1ef850,
 *   docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
struct O *PbhvPushBhvstk(struct STK *stack,int selector){
    struct O *previous,*object;
    void *memory;
    if(stack->count==0)previous=0;
    else previous=(struct O *)stack->slots[stack->count-1];
    {
        int index=Count(&stack->count);
        memory=stack->slots[index];
        stack->count=index+1;
    }
    func_002333B4(memory,0,CbBhvMax());
    object=PbhvNew(selector,memory);
    {
        register int tag __asm__("$3")=stack->tag;
        object->tag=tag;
        object->previous=previous;
    }
    if(previous)previous->next=object;
    object->vt[7].init((char *)object+object->vt[7].delta);
    return object;
}

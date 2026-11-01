typedef int qword __attribute__((mode(TI)));
typedef struct {qword a[4];} M4;
union V {qword q;float f[4];};
struct X {char pad40[0x40];M4 m;};
struct B {union V *v;};
struct M {struct X *px;char pad4[0xC];struct B *pb;char pad14[8];int count;char pad20[0x5C];union V *points;};
struct S {char pad258[0x258];struct M *pm0,*pm1;};
struct CLK {float time,dt;};
extern struct CLK D_002DDED0;
extern float D_002C3C68[];
M4 *func_0011C318(M4 *,const M4 *);
M4 *func_0011C168(M4 *,const M4 *,const M4 *);
float func_0011D018(float,float,float,float *,float *);
static __inline__ void CopyMatrix(M4 *destination,const M4 *source) {
    register qword row0 __asm__("$2");
    register qword row1 __asm__("$3");
    register qword row2 __asm__("$4");
    register qword row3 __asm__("$5");
    __asm__("lq %2,%6\n\tlq %1,%5\n\tlq %3,%7\n\tlq %0,%4"
      :"=r"(row0),"=r"(row1),"=r"(row2),"=r"(row3)
      :"m"(source->a[0]),"m"(source->a[1]),"m"(source->a[2]),"m"(source->a[3]));
    destination->a[1]=row1;destination->a[2]=row2;
    destination->a[3]=row3;destination->a[0]=row0;
}
/** @sly2-annot addr=0x203E08
 * Purpose: Per point: smooth the body shape toward the phys shape with GSmooth(old,new,dt,s_smpPhys
 *   D_002C3C68) and store (new-old)/dt as point velocity
 * Info: proposed name UpdateStepPhys (T1); TU step
 * Confidence: high [T1 0.80] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70
 * @end-sly2-annot */
void func_00203E08(struct S *input)
{
    void *context=input;
    M4 m;
    union V v;
    M4 mm,mi;
    register M4 *pmi __asm__("$16");
    register M4 *pmm __asm__("$17");
    register struct B *velocityContainer __asm__("$2");
    register struct X *firstBasis __asm__("$5");
    union V *old;
    register union V *velocity __asm__("$20");
    int i,j;
    register struct M *firstSource __asm__("$3");
    register struct M *secondSource __asm__("$4");
    struct CLK *clock;
    float *pv;
    float *pt,*ps,*pd;
    __asm__("lw %0,%1":"=r"(firstSource):"m"(((struct S*)context)->pm0));
    if(!firstSource)return;
    __asm__("lw %0,%1":"=r"(secondSource):"m"(((struct S*)context)->pm1));
    pmi=&mi;
    if(!secondSource)return;
    __asm__("lw %0,%2\n\tlw %1,%3":"=r"(velocityContainer),"=r"(firstBasis):"m"(firstSource->pb),"m"(firstSource->px));
    pmm=&mm;
    {register float *nextPoints __asm__("$18");
     register union V *oldPoints __asm__("$19");
     __asm__("lw %0,%4\n\tlw %1,%5\n\taddiu %2,%2,64\n\tlw %3,%6"
       :"=r"(nextPoints),"=r"(oldPoints),"+r"(firstBasis),"=r"(velocity)
       :"m"(secondSource->points),"m"(firstSource->points),"m"(velocityContainer->v));
     pd=nextPoints;old=oldPoints;}
    func_0011C318(pmi,(M4 *)firstBasis);
    {register struct M *currentSource __asm__("$2");
     register M4 *destination __asm__("$4");
     register M4 *inverse __asm__("$5");
     register struct X *basis __asm__("$6");
     __asm__("lw %0,%1":"=r"(currentSource):"m"(((struct S*)context)->pm1));
     __asm__("daddu %0,%3,$zero\n\tdaddu %1,%4,$zero\n\tlw %2,%5"
       :"=&r"(destination),"=&r"(inverse),"=r"(basis)
       :"r"(pmm),"r"(pmi),"m"(currentSource->px));
     func_0011C168(destination,inverse,&basis->m);}

    CopyMatrix(&m,&mm);
    i=((struct S*)context)->pm1->count;
    if(--i>=0){
    pv=v.f;
    clock=&D_002DDED0;
    do{
        register float *nextGoal __asm__("$22");
        int nextIndex=i-1;
        context=old+1;
        __asm__("addiu %0,%1,16":"=r"(nextGoal):"r"(pd));
        __asm__("vmulax.xyz $ACC, %1, %5x\n\tvmadday.xyz $ACC, %2, %5y\n\tvmaddaz.xyz $ACC, %3, %5z\n\tvmaddw.xyz %0, %4, $vf0w"
            : "=j"(v.q) : "0"(m.a[0]),"j"(m.a[1]),"j"(m.a[2]),"j"(m.a[3]),"j"(*(qword*)pd));
        pd=velocity->f;
        __asm__("addiu %0,%0,16":"+r"(velocity));
        pt=pv;ps=old->f;
        j=3;
        while(--j>=0){
            float next=func_0011D018(*ps,*pt,clock->dt,D_002C3C68,0);
            *pd=(next-*ps)/clock->dt;
            ++pt;++ps;++pd;
        }
        pd=nextGoal;old=context;
        i=nextIndex;
    }while(i>=0);
    }
}

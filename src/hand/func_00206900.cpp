extern "C" {
typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct P;
struct ENTRY {struct P *object;int mode;float stamp;int unusedC;};
struct MANAGER {char pad0[0xFC];int count;struct ENTRY entries[8];};
struct ANCHOR {int unused0;float stamp;int mode;struct P *object;};
struct P {char pad0[0x2C0];ulong flags;char pad2C8[0xCC];int kindCount;int kinds[3];int generated;char pad3A8[0x24];struct ANCHOR *anchor;char pad3D0[0xB94];struct MANAGER *manager;};
struct CANDIDATE {float distance;int *source;struct P *object;int mode;float stamp;int unused14;};
struct GAME;
extern struct GAME D_003DD490;
extern int D_003DD488;
extern ulong D_003DD480;
extern float D_002A8614;
extern float D_002C3D04,D_002C3D08,D_002C3D0C;
ulong func_00207A20(int);
int func_0015E9C8(struct GAME *,int,int,int,int,struct P **);
qword func_001A62E8(struct P *);
void func_002338C0(void *,unsigned int,unsigned int,int (*)(const void *,const void *));
int func_002068D8(const void *,const void *);
int func_002085D0(struct P *,struct P *,int);
void func_00153DE0(struct MANAGER *);
struct ENTRY *func_00154180(struct MANAGER *,struct P *);
void func_0020A9A8(struct ANCHOR *,int);
static __inline__ qword Subtract(qword a,qword b){qword r;__asm__("vsub.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static __inline__ float LengthXY(qword a){qword r;float f;__asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y":"=j"(r):"j"(a));__asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(r));__asm__("mtc1 %1,%0":"=f"(f):"r"(r));return f;}
static __inline__ float Z(qword q){float f;__asm__("pextuw %0,$zero,%1":"=r"(f):"r"(q));return f;}
static __inline__ float Abs(float f){return __builtin_fabsf(f);}
static __inline__ int Near(qword difference){return LengthXY(difference)<D_002C3D08 && Abs(Z(difference))<D_002C3D0C;}
/* Two empty-asm tricks for the nearby-object collection loop (T1592); neither emits an instruction:
   OPAQUE_COUNT: an empty asm that "sets" the candidate count. loop.c then sees no basic induction variable, so
     &candidates[n] stays a real `mult` by 24 instead of a strength-reduced pointer, and the increment is not PRE-copied.
   LOOP_SIZE_PAD: 24 empty volatile asms at the end of the loop body. They emit nothing, but loop.c's move_movables only
     hoists a constant out of a loop when threshold(95)*savings*lifetime >= insn count; the original loop body has ~96 RTL
     insns, so only the D_002C3D04 high part is hoisted (to $fp) and the `li 24` / `li -1` stay inside the loop. */
#define OPAQUE_COUNT(n) __asm__("":"+r"(n))
#define PAD1 __asm__ volatile("");
#define PAD4 PAD1 PAD1 PAD1 PAD1
#define LOOP_SIZE_PAD PAD4 PAD4 PAD4 PAD4 PAD4 PAD4
/** @sly2-annot addr=0x206900
 * Purpose: Unless in a blind state, collect objects of the watched classes (+0x394) within 1500 xy, sort by
 *   distance, re-add them as detector targets keeping previous state; UpdateDetectorTargets; update shared
 *   target record from detector (seen=1, lost-and-near=2)
 * Info: proposed name UpdateStepguardDetectTargets (T1); TU stepguard
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r7b_guard
 * See: docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_00206900(struct P *p)
{
    struct CANDIDATE candidates[64];
    struct P *objects[256];
    qword anchorPosition,ownerPosition;
    if(!D_003DD488){
        ulong a=func_00207A20(5);
        ulong b=func_00207A20(6);
        ulong c=func_00207A20(23);
        ulong d=func_00207A20(7);
        ulong e=func_00207A20(16);
        ulong f=func_00207A20(1);
        ulong g=func_00207A20(22);
        ulong h=func_00207A20(42);
        D_003DD488=1;
        D_003DD480=a|b|c|d|e|f|g|h;
    }
    if(p->flags&D_003DD480)return;
    if(p->kindCount){
        int candidateCount=0;
        int kind;
        for(kind=0;kind<p->kindCount;kind++){
            int *source=&p->kinds[kind];
            int objectCount=func_0015E9C8(&D_003DD490,0x505,*source,0,256,objects);
            int i=0;
            if(objectCount>0){
                float initialStamp=D_002A8614;
                do{
                    struct P *object=objects[i];
                    if(object!=p){
                        qword otherPosition;
                        float distance;
                        ownerPosition=func_001A62E8(p);
                        otherPosition=func_001A62E8(object);
                        distance=LengthXY(Subtract(ownerPosition,otherPosition));
                        if(distance<D_002C3D04 && (unsigned int)candidateCount<64){
                            struct CANDIDATE *record;
                            int old;
                            record=&candidates[candidateCount];
                            record->distance=distance;
                            record->mode=-1;
                            record->source=source;
                            record->object=object;
                            record->stamp=initialStamp;
                            candidateCount++;
                            for(old=p->manager->count-p->generated;old<p->manager->count;old++){
                                struct ENTRY *entry=&p->manager->entries[old];
                                if(entry->object==record->object){
                                    record->mode=entry->mode;
                                    record->stamp=entry->stamp;
                                    break;
                                }
                            }
                        }
                        OPAQUE_COUNT(candidateCount);
                        LOOP_SIZE_PAD
                    }
                }while(++i<objectCount);
            }
        }
        func_002338C0(candidates,candidateCount,sizeof(struct CANDIDATE),func_002068D8);
        p->manager->count-=p->generated;
        p->generated=0;
        if(candidateCount>0){
            int absentMode=-1;
            struct CANDIDATE *record=candidates;
            do{
                if(func_002085D0(p,record->object,(int)record->source)){
                    p->generated++;
                    if(record->mode!=absentMode){
                        struct ENTRY *entry=&p->manager->entries[p->manager->count-1];
                        entry->mode=record->mode;
                        entry->stamp=record->stamp;
                    }
                }
                record++;
            }while(--candidateCount);
        }
    }
    func_00153DE0(p->manager);
    if(p->anchor){
        struct ENTRY *entry=func_00154180(p->manager,p->anchor->object);
        if(entry){
            switch(entry->mode){case 1:func_0020A9A8(p->anchor,1);break;
            case 2:{
                qword ownPosition,difference;
                anchorPosition=func_001A62E8(p->anchor->object);
                ownPosition=func_001A62E8(p);
                difference=Subtract(anchorPosition,ownPosition);
                if(Near(difference))func_0020A9A8(p->anchor,2);
            }break;
            }
        }
    }
}
}

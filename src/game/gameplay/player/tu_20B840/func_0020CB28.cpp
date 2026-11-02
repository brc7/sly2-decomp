// Select the best animation direction and update its velocity.
typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f[4];} V;
union FLOATWORD {float value;int word;};
struct ASEG;struct ALO;
struct ASEGA {char pad[0x14];struct ASEG *owner;};
struct TAG {float value;};
struct E {struct ASEG *animation;int pad[3];qword velocity;FLOATWORD speed;int pad20[3];};
struct P {char pad[0x10];qword direction;struct ALO *root;struct ASEGA *active;struct E *entries,*best;int flags,forward,backward;};
extern const float D_002A8904;
extern float D_002C3DE4;
extern "C" struct TAG *func_0012F938(struct ASEG*,int);
void ApplyAsegCur(struct ASEG*,struct ALO*,float,float,int,struct ASEGA**);
extern "C" void SubscribeLoObject(struct ASEGA*,int);
static inline qword Dot(qword a,qword b){qword r,one;
__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=&j"(r),"=&j"(one):"j"(a),"j"(b));return r;}
static inline qword DotReuse(qword a,qword b){qword one;
__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(b),"=&j"(one):"j"(a),"0"(b));return b;}
static inline qword Sqrt(qword q){__asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(q):"0"(q));return q;}
static inline void Normalize(V*dst,qword v){qword length=Sqrt(Dot(v,v));register qword result __asm__("vf3");
__asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q":"=&j"(result):"j"(length),"j"(v));dst->q=result;}
static inline qword NormalizeEntry(qword v){qword length=Sqrt(Dot(v,v));
__asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q":"=j"(length):"0"(length),"j"(v));return length;}
static inline float Float(qword v){float f;__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(v));return f;}
static inline qword Scalar(const FLOATWORD*p){register qword v __asm__("vf1");register int w __asm__("$2")=p->word;__asm__("qmtc2.ni %1,%0":"=j"(v):"r"(w));return v;}
static inline qword Scale(qword v,qword s){register qword r __asm__("vf2");__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(v),"j"(s));return r;}
/** @sly2-annot addr=0x20CB28
 * Purpose: choose best of 4 directional anims vs desired direction
 * Info: proposed name SelectDirAnim (T2); TU tu_20B840
 * Notes: T2 forced: verifier: coined class word without descriptor
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r7c_hud
 * See: docs/reference/functions/animation.md#tu_20b840-0x20b840-0x20ce28
 * @end-sly2-annot */
extern "C" void func_0020CB28(struct P*p,qword v)
{
 V scalar,direction;
 register qword finalDirection __asm__("vf3");
 struct E *best=0;
 register struct E *current __asm__("v0");
 Normalize(&direction,v);
 float maximum=D_002A8904;
 if(p->forward||p->backward){
   struct E *entry;
   unsigned int i;float bias;
   register qword scoringDirection __asm__("vf4");
   __asm__("lqc2 %0,%4\n\tlw %1,%5\n\tdaddu %2,%z6,$zero\n\tlwc1 %3,%7":"=j"(scoringDirection),"=&r"(entry),"=&r"(i),"=f"(bias):"m"(direction.q),"m"(p->entries),"rJ"(0),"m"(D_002C3DE4));
   do{
     struct ASEG *animation=entry->animation;
     if(animation){
       float score=Float(DotReuse(scoringDirection,NormalizeEntry(entry->velocity)));
       if(animation==p->active->owner)score*=bias;
       if(maximum<score){maximum=score;best=entry;}
     }
   }while(++entry,++i<4);
 current=p->best;
 finalDirection=direction.q;
 if(best!=current){
   float time=0.0f;
   struct TAG*tag;
   struct ASEG*animation;
   int id;
   if(p->forward){id=0x17D;animation=best->animation;}
   else {id=0x17F;animation=best->animation;}
   tag=func_0012F938(animation,id);
   if(tag)time=tag->value;
   ApplyAsegCur(best->animation,p->root,time,1.0f,p->flags,&p->active);
   SubscribeLoObject(p->active,(int)p);
   p->best=best;
 }
 }
 current=p->best;finalDirection=direction.q;
 scalar.q=Scalar(&current->speed);
 p->direction=Scale(finalDirection,scalar.q);
}

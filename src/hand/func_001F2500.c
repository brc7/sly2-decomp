typedef int qword __attribute__((mode(TI)));
typedef unsigned long ulong;
struct Node;
struct Origin {char pad[0x70];qword pos;};
struct Stats {struct Node *owner;qword *origin;int count,unusedC;float low;int unused14,unused18;float high,scale;};
struct Node {
 char pad[0x10];struct Node *parent;
 char pad14[0x60-0x14];struct Origin *reference;
 char pad64[0xA0-0x64];float radius;
 char padA4[0xD0-0xA4];ulong flags;
 char padD8[0x134-0xD8];struct Stats *stats;
 int unused138;float scale;
 char pad140[8];ulong state;
};
struct Stats *func_0011CCC0(void *);
qword func_001A62E8(struct Node *);
static inline float Distance(qword a,qword b) {
 qword d,one;
 float result;
 __asm__("vsub.xyzw %0,%1,%2":"=j"(d):"j"(a),"j"(b));
 __asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"0"(d));
 __asm__("vsqrt Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(d));
 __asm__("mtc1 %1,%0":"=f"(result):"r"(d));
 return result;
}
static inline ulong LoadState(struct Node *p)
{
    register ulong value __asm__("$2");
    __asm__("ld %0,%1":"=r"(value):"m"(p->state));
    return value;
}
/** @sly2-annot addr=0x1F2500
 * Purpose: Attach SO to the bounds record (+0x134) of its root (alloc from pool if none), then grow record
 *   low/high radius by radius+distance from root origin per flag bits 32/33 of +0xD0
 * Info: proposed name EnsureSoBounds (T1); TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void func_001F2500(struct Node *p) {
 struct Node *top;
 struct Stats *attached;
 if (!(p->flags & (3UL<<32))) return;
 top=p;
 attached=p->stats;
 if(!attached) {
  do {
   register struct Node *next __asm__("$4")=top->parent;
   register int one __asm__("$3")=1;
   if(!next || ((int)((unsigned int)(int)LoadState(top)<<8)>>28)==one) {
     register char*pool __asm__("$4");
     struct Stats *s;
     __asm__("lui %0,0x3e":"=r"(pool));
     s=func_0011CCC0(pool+0x12D4);
     top->stats=s;
     p->stats=s;
     s->origin=&top->reference->pos;
     ++p->stats->count;
     p->stats->scale=p->scale;
     p->stats->owner=p;
     if(top==p)goto position;
     attached=p->stats;
     goto count;
   }
   top=next;
   attached=top->stats;
  } while(!attached);
  p->stats=attached;
 }
count:
 ++attached->count;
position:

 {
   qword position=func_001A62E8(p);
   float radius=p->radius+Distance(position,*p->stats->origin);
   if(p->flags&(1UL<<32)) {
     if(p->stats->low<radius) p->stats->low=radius;
   }
   if(p->flags&(1UL<<33)) {
     if(p->stats->high<radius) p->stats->high=radius;
   }
 }
}

#define OFF(T,F) ((unsigned long)&((T*)0)->F)
typedef char ancestor_layout[(sizeof(qword)==16 && __alignof__(qword)==16 && sizeof(void*)==4 && sizeof(float)==4 && sizeof(int)==4 && sizeof(unsigned)==4 && sizeof(ulong)==8 && sizeof(struct Origin)==0x80 && __alignof__(struct Origin)==16 && sizeof(struct Stats)==0x24 && __alignof__(struct Stats)==4 && sizeof(struct Node)==0x150 && __alignof__(struct Node)==8 && OFF(struct Origin,pos)==0x70 && OFF(struct Stats,owner)==0x0 && OFF(struct Stats,origin)==0x4 && OFF(struct Stats,count)==0x8 && OFF(struct Stats,low)==0x10 && OFF(struct Stats,high)==0x1c && OFF(struct Stats,scale)==0x20 && OFF(struct Node,parent)==0x10 && OFF(struct Node,reference)==0x60 && OFF(struct Node,radius)==0xa0 && OFF(struct Node,flags)==0xd0 && OFF(struct Node,stats)==0x134 && OFF(struct Node,scale)==0x13c && OFF(struct Node,state)==0x148)?1:-1];

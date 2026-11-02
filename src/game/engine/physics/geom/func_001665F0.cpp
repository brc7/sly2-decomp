typedef int qword __attribute__((mode(TI)));
struct E {unsigned short a,b;int pad;struct E*next;};
struct Poly {char pad[0x10];struct E*edges;};
struct Entry {int a,b;struct Poly*poly;};
struct P {int pad;qword *points;};
struct G {int pad,count;void *pad8;void *tree;};
struct REQ {int cb,kind;REQ(int cb_,int kind_):cb(cb_),kind(kind_) {}};
struct FRAME {int pad[4];};
extern "C" struct FRAME *STACKSCOPE_ctor(struct FRAME*);
extern "C" void STACKSCOPE_ctor_1AE4A0(struct FRAME*,int);
extern "C" void *func_001AE318(struct REQ*);
extern "C" int func_00118A88(void*,int,qword,int*,struct Entry**,float);
static inline int Min(int a,int b){return a<b?a:b;}
// Collect unique edges incident to a geometry point.
/** @sly2-annot addr=0x1665F0
 * Purpose: Collects up to max unique edges incident to a given GEOM point, using a BSP query around the point
 * Info: proposed name CpedgeFromGeomPoint (T2); TU geom
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/physics-collision.md#geom-0x166270-0x166920
 * @end-sly2-annot */
extern "C" int func_001665F0(struct P*p,struct G*g,int index,int max,struct E**out){
 struct FRAME frame;int capacity,limit,count,i;struct Entry*first,*end;
 STACKSCOPE_ctor(&frame);
 capacity=Min(g->count,0x1555);limit=capacity;struct REQ req(capacity*12,16);
 end=first=(struct Entry*)func_001AE318(&req);
 if(func_00118A88(g->tree,0x7FFFFFFF,p->points[index],&limit,&end,1.0f)!=2){STACKSCOPE_ctor_1AE4A0(&frame,2);return 0;}
 int ne=end-first;
 count=0;
 for(i=0;i<ne;i++){
  struct E*e=first[i].poly->edges;struct E**dst;
  if(!e)continue;
  dst=(struct E**)((count<<2)+(int)out);
  for(;e;e=e->next){
   int j;
   if(index!=e->a && index!=e->b)continue;
   for(j=0;j<count;j++){if(e==out[j])break;}
   if(j>=count && count<max){*dst++=e;count++;}
  }
 }
 STACKSCOPE_ctor_1AE4A0(&frame,2);
 return count;
}

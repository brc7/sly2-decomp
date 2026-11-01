typedef int qword __attribute__((mode(TI)));
union U {qword q;unsigned int words[4];};
struct C {qword x,z;int version;int xmin,xmax,zmin,zmax;};
struct S {char pad[0xFC];int count,active;void*xs,*zs;void**objects;int pad110,version;};
void func_001C1440(struct S*,struct C*);
void func_001C15F8(void*,int*,int*,qword*,float,float);
static inline qword Max(qword a,qword b){qword r;__asm__("vmax.xyz %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline qword Min(qword a,qword b){qword r;__asm__("vmini.xyz %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r;}
static inline float X(qword q){float f;__asm__("mtc1 %1,%0":"=f"(f):"r"(q));return f;}
static inline float Z(qword q){float f;__asm__("prot3w %0,%1":"=r"(f):"r"(q));return f;}
static inline qword And(qword a,qword b){qword r;__asm__("pand %0,%1,%2":"=r"(r):"r"(a),"r"(b));return r;}
/** @sly2-annot addr=0x1C1700
 * Purpose: Sweep-and-prune query: obstacles whose x and y intervals overlap the segment box
 * Info: proposed name CploFindPathzoneObstacles (T2); TU path
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/physics-collision.md#path-0x1bf110-0x1c2920,
 *   docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
int func_001C1700(struct S*p,struct C*c,qword a,qword b,int max,void**out){
 struct C local;
 union U mask,copy;
 qword hi,lo;
 int i,n;
 float hx,lx;
 if(!p->active)return 0;
 if(!c){c=&local;c->version=-1;}
 if(c->version!=p->version)func_001C1440(p,c);
 lo=Min(a,b);hi=Max(a,b);
 func_001C15F8(p->xs,&c->xmin,&c->xmax,&c->x,X(lo),X(hi));
 func_001C15F8(p->zs,&c->zmin,&c->zmax,&c->z,Z(lo),Z(hi));
 copy.q=And(c->x,c->z);mask=copy;
 {void**cursor=out;
 for(i=0,n=0;i<p->count;i++){
  if(mask.words[i/32]&(1<<(i%32))){
   if(max<n)break;
   *cursor++=p->objects[i];n++;
  }
 }
 }
 c->version=p->version;
 return n;
}

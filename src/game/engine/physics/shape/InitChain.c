typedef int qword __attribute__((mode(TI)));
struct M4 {qword a,b,c,d;};
struct AL {int cb,align;};
struct VTE {short d,pad;struct M4 *(*pf)(void*);};
struct PATH {char pad[0xC];struct VTE *vt;char pad10[0xE8-0x10];int n;int padEC;float *dist;qword *v;};
struct S {int a,b;float g8;char padC[0x20-0xC];qword start,end;void *fn40,*fn44;struct PATH *path;int n4C;int pad50;qword *v54,*v58;float *dist;float g60;char pad64[8];float g6C,g70;char pad74[0x84-0x74];int n84;float g88;};
extern char D_002C44F0[];
void *func_001AE2A0(struct AL*);
void func_001E9828(struct S*);
static inline qword Point(struct M4 *m,qword a) {qword r;__asm__("vmulax.xyz $ACC,%1,%5x\n\tvmadday.xyz $ACC,%2,%5y\n\tvmaddaz.xyz $ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w":"=j"(r):"j"(m->a),"j"(m->b),"j"(m->c),"j"(m->d),"j"(a));return r;}
/** @sly2-annot addr=0x1E81C0
 * Purpose: Initialise a chain record for a shape (copy points/dists to a private array, set rest lengths)
 * Info: proposed name InitChain (T1); TU shape
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r6b_rest+verdicts_followups
 * See: docs/engine/physics-collision.md#7-key-functions,
 *   docs/reference/functions/physics-collision.md#shape-0x1e5d80-0x1ea568
 * @end-sly2-annot */
struct S *InitChain(struct S *p,struct PATH *path) {
 struct AL size;struct M4 *m;int i;
 p->path=path;p->n4C=-1;
 size.cb=path->n*16;size.align=16;p->v54=func_001AE2A0(&size);
 size.cb=path->n*16;size.align=16;p->v58=func_001AE2A0(&size);
 size.cb=path->n*4;size.align=16;p->dist=func_001AE2A0(&size);
 p->g8=-1750.0f;p->g60=3.0f;p->g6C=500.0f;p->g70=5.0f;
 p->a=0;p->b=0;
 p->n84=p->path->n;p->g88=300.0f;
 m=p->path->vt[31].pf((char*)p->path+p->path->vt[31].d);
 for(i=0;i<p->path->n;i++)p->v54[i]=Point(m,p->path->v[i]);
 p->fn40=p->fn44=D_002C44F0;
 if(p->path->n>0) {
  p->start=p->v54[0];p->end=p->v54[p->path->n-1];
 }
 func_001E9828(p);
 return p;
}

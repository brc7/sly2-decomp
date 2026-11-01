typedef int qword __attribute__((mode(TI)));
struct M3 { qword a,b,c; };
struct V { float x,y,z,w; };
union U { qword q; float f[4]; };
struct E { int n; char pad4[8]; V *out; char pad10[0x1C-0x10]; U *samples; };
struct NODE { int pad; int n; E *entries; NODE *next; };
struct S { char pad[0xDC]; NODE *head; char padE0[0x150-0xE0]; void *path; };
extern "C" {
void func_001F3C70(S*);
float func_001E6630(void*);
M3 *func_001A6318(S*);
void func_001E67B8(void*,float,qword*,qword*);
M3 BuildOrthonormalMatrixZ(qword,qword);
void func_001E9AC0(S*);
}
static inline qword Transform(M3 *m,qword a) { __asm__("vmulax.xyz $ACC,%1,%0x\n\tvmadday.xyz $ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z":"+j"(a):"j"(m->a),"j"(m->b),"j"(m->c));return a; }
static inline qword Add(qword a,qword b) { qword r;__asm__("vadd.xyzw %0,%1,%2":"=j"(r):"j"(a),"j"(b));return r; }
// Store the low packed word directly; higher words remain available for Y/Z extraction.
static inline void StoreX(float *p,qword q) {__asm__("sw %1,%0":"=m"(*p):"r"(q));}
static inline float GetZ(qword q) {float f;__asm__("pextuw %0,$zero,%1":"=r"(f):"r"(q));return f;}
static inline float GetY(qword q) {float f;__asm__("prot3w %0,%1":"=r"(f):"r"(q));return f;}
static inline float Min(float a,float b) {return b<a?b:a;}
void func_001E9AC0(S *p)
{
 func_001F3C70(p);
 float end=func_001E6630(p->path);
 qword axis=func_001A6318(p)->c;
 NODE *node;
 for(node=p->head;node;node=node->next) {
  int j;
  for(j=0;j<node->n;j++) {
   E *e=&node->entries[j];
   U *src=e->samples; V *out=e->out;
   int k;
   for(k=0;k<e->n;k++) {
    U pos,dir;
    func_001E67B8(p->path,Min(end,src->f[3]),&pos.q,&dir.q);
    M3 m;
    M3 tmp=BuildOrthonormalMatrixZ(dir.q,axis);
    m=tmp;
    qword result=Add(Transform(&m,src->q),pos.q);
    src++;
    StoreX(&out->x,result);
    out->y=GetY(result);
    out->z=GetZ(result);
    out++;
   }
  }
 }
 func_001F3C70(p);
}

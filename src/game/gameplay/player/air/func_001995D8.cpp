/* Build paired arc samples and transform them into world space. */
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];int i[4];};
struct M3 {U r[3]; M3(){} M3(const M3&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];} M3&operator=(const M3&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];return *this;}};
struct M4 {U r[4];};
struct P {char pad[0x108];float ax,ay,az,bx,by,bz;};
extern qword D_002A2DC0;
extern "C" M3 func_0011B500(qword,float);
static inline qword Pack(float x,float y,float z){U u;qword q,t;__asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0":"=&r"(q),"=&r"(t):"f"(x),"f"(y),"f"(z));u.q=q;return q;}
static inline qword Rot(const M3&m,qword v){qword r;__asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r):"j"(m.r[0].q),"j"(m.r[1].q),"j"(m.r[2].q),"j"(v));return r;}
static inline qword Transform(const M4*m,qword v){qword r;__asm__("vmulax.xyz ACC,%1,%5x\n\tvmadday.xyz ACC,%2,%5y\n\tvmaddaz.xyz ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w":"=j"(r):"j"(m->r[0].q),"j"(m->r[1].q),"j"(m->r[2].q),"j"(m->r[3].q),"j"(v));return r;}
static inline void Set(U*v,float x,float z){v->f[0]=x;v->f[2]=z;v->i[1]=0;}
/** @sly2-annot addr=0x1995D8
 * Purpose: Generate paired arc sample points (9 rotations of 15 deg) and transform them to world
 * Info: proposed name BuildJtEdgeProbeArc (T2); TU air
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
extern "C" void func_001995D8(P*p,const M4*world,int count,U*out){
 M3 m;U *cursor=out,*v;
 Set(cursor++,p->bx,p->bz);Set(cursor++,p->bx,p->by);Set(cursor++,p->ax,p->az);v=cursor++;Set(v,p->bx,p->bz);
 float x=p->ax,y=v->f[1];qword a=Pack(x,y,p->az),b=Pack(x,y,p->ay);
 int i;for(i=8;i>=0;i--){m=func_0011B500(D_002A2DC0,(i-4)*0.261799395084381103515625f);U*v0=cursor++;U*v1=cursor++;v0->q=Rot(m,a);v1->q=Rot(m,b);}
 cursor=out;i=count;while(--i>=0){cursor->q=Transform(world,cursor->q);cursor++;}
}

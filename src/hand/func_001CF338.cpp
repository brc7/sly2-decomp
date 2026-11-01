typedef int qword __attribute__((mode(TI)));
struct M3 { qword a,b,c; }; struct M4 { qword a,b,c,d; };
union U { qword q; float f; };
struct POLY { float a,b,c; };
struct LO { char pad[0x38]; float radius; };
struct S { char pad[0x14]; float start,duration; LO *lo; POLY size; int pad2; POLY alpha; };
struct G { char pad[0x90]; qword pos; };
extern G D_002DDF40;
extern struct CLOCK { float t,dt; } D_002DDED0;
extern struct LIMIT { float lo,hi; } D_002BBB60;
extern "C" {
int func_0014F850(G*,qword,qword);
int func_0014F898(G*,qword,float,float*);
M4 func_0011C288(qword,const M3&,float);
void func_001A5EE8(LO*,const M4&,float,float);
int func_001CF338(S*,qword,const M3&);
}
static inline float Poly(POLY *p,float t){return p->a+t*(p->b+t*p->c);}
static inline float Clamp(float f,float lo,float hi){if(f<lo)return lo;if(hi<f)return hi;return f;}
static inline qword BcG(float f){U u;qword r;__asm__("mfc1 %0,%1":"=r"(r):"f"(f));u.q=r;return r;}
/** @sly2-annot addr=0x1CF338
 * Purpose: Compute rip render position/matrix relative to camera; false if culled
 * Info: proposed name FRenderRipPosMat (T1); TU rip
 * Notes: names_v2 v1 proposed ProjectBubletTransform__FP6BUBLETf (0.002)
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
int func_001CF338(S*p,qword pos,const M3&mat)
{
    if(!p->lo)return 1;
    if(p->duration<=0.0f)return 0;

    float proj;
    qword delta;
    float t=(D_002DDED0.t-p->start)/p->duration;
    float alpha=Clamp(Poly(&p->alpha,t),D_002BBB60.lo,D_002BBB60.hi);
    float size=Poly(&p->size,t);
    G *pg=&D_002DDF40;
    __asm__("vsub.xyzw %0,%1,%2":"=j"(delta):"j"(pos),"j"(pg->pos));
    if(!func_0014F850(pg,delta,BcG(size*100.0f)))return 1;
    if(!func_0014F898(pg,delta,p->lo->radius,&proj))return 0;
    if(alpha<0.0001f && size<0.0001f)return 1;
    M4 m;
    M4 tmp=func_0011C288(pos,mat,size);
    m=tmp;
    func_001A5EE8(p->lo,m,alpha*proj,1.0f);
    return 1;
}

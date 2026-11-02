typedef int qword __attribute__((mode(TI)));
struct M3 { qword r[3]; };
union U { qword q; float f; };
struct G { char pad[0x60]; M3 mat; };
struct SPIN { char pad[0x40]; qword axis; };
struct S {char pad[0x14];float start,duration;char pad1C[0x40-0x1C];M3 mat;char pad70[0xF0-0x70];SPIN *spin;char padF4[0x100-0xF4];qword pos;};
extern G D_002DDF40;
extern struct CLOCK {float t,dt;} D_002DDED0;
extern "C" {
qword func_0014F268(G*,qword);
M3 MulMatrix3Matrix3(const M3&,const M3&);
void func_0011BCF8(M3*,float*,qword*);
M3 func_0011B500(qword,float);
int func_001CF338(S*,qword,const M3&);
void func_001D0630(S*);
}
static inline M3 Mul(const M3 &a,const M3 &b) return r {
 __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[0]):"j"(a.r[0]),"j"(a.r[1]),"j"(a.r[2]),"j"(b.r[0]));
 __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[1]):"j"(a.r[0]),"j"(a.r[1]),"j"(a.r[2]),"j"(b.r[1]));
 __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"=j"(r.r[2]):"j"(a.r[0]),"j"(a.r[1]),"j"(a.r[2]),"j"(b.r[2]));
}
/** @sly2-annot addr=0x1D0630
 * Purpose: SMACK Render
 * Info: proposed name RenderSmack (T1); TU rip
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void func_001D0630(S *p) {
 M3 camera,fade,spin,render;
 U axis;float angle;
 G *pg=&D_002DDF40;
 qword view=func_0014F268(pg,p->pos);
 camera=MulMatrix3Matrix3(p->mat,pg->mat);
 func_0011BCF8(&camera,&angle,&axis.q);
 float t=(D_002DDED0.t-p->start)/p->duration;
 fade=func_0011B500(axis.q,angle*t);
 spin=func_0011B500(p->spin->axis,(t+t)*3.14159274f);
 render=Mul(Mul(fade,spin),p->mat);
 func_001CF338(p,view,render);
}

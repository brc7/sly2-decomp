typedef int qword __attribute__((mode(TI)));
union U{qword q;float f[4];};
struct M4{qword r[4];};
struct CM{char pad[0x40];float sx,sy;char pad48[0x18];M4 mat;};
struct S{char pad[0x80];float planar;int edge,range,occlusion;};
struct LSG;
extern "C" CM D_002DDF40;
extern "C" float D_002BE5B8,D_002BE5BC,D_002BE5C0,D_002BE5C4,D_002BE5C8;
extern "C" qword ConvertCmWorldToScreen(CM*,qword);
extern "C" int func_00173588(qword,float);
extern "C" void*PsoHitTestLineObjects__FiP6VECTORT1iPP2SOP3LSG(int,qword,qword,int,void**,LSG*);
static inline qword Sub(qword a,qword b){__asm__("vsub.xyzw %0,%1,%2":"=j"(a):"0"(a),"j"(b));return a;}
static inline qword LoadCpu(const U&u){register qword q asm("$5");__asm__("lq %0,%1":"=r"(q):"m"(u.q));return q;}
static inline qword Load(const U&u){register qword q asm("vf3");__asm__("lqc2 %0,%1":"=j"(q):"m"(u.q));return q;}
static inline float Len3(qword a,const U&u){qword b=Load(u);register qword d asm("vf1"),one asm("vf2");float f;__asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(d),"=&j"(one):"j"(a),"j"(b));__asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"=j"(d):"0"(d));__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(d));return f;}
static inline float Len2(qword input){register qword a asm("vf2")=input,d asm("vf1");float f;__asm__("vmul.xy %0,%1,%1\n\tvaddy.x %0,%0,%0y":"=j"(d):"j"(a));__asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"=j"(d):"0"(d));__asm__("qmfc2.ni %0,%1":"=r"(f):"j"(d));return f;}
static inline void ReadInputs(CM*source,const U&position,qword&cpu,qword&point,qword&vector){register CM*p asm("$2")=source;register qword r asm("$5"),v asm("vf1"),o asm("vf2");__asm__("lq %0,%3\n\tlqc2 %1,%4\n\tqmtc2.ni %0,%2":"=r"(r),"=j"(v),"=j"(o):"m"(p->mat.r[3]),"m"(position.q));cpu=r;point=v;vector=o;}
static inline float Z(qword a){register int bits asm("$5");union{int n;float f;}value;__asm__("pextuw %0,$0,%1":"=r"(bits):"r"(a));value.n=bits;return value.f;}
static inline int TooFar(qword a,float limit){return limit<Len2(a);}
/** @sly2-annot addr=0x173768
 * Purpose: Camera check for a spawn point: 0 if too far, behind or off screen; else 1 at screen edge, 2 out
 *   of planar range, 3 occluded (0 if plainly visible)
 * Info: proposed name KCid119PointConcealed (T2); class CID119; TU tu_173158
 * Notes: names_v2 C proposed PostFlameEmit__FP5FLAMEP5EMITB (0.200) | Cid77->Cid119 (CID 0x77=119,
 *   agent-default(hex))
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_173158-0x173158-0x174898
 * @end-sly2-annot */
extern "C" int func_00173768(S*p,qword pos,int count,void**objects,float distance){
 register int cso asm("$19")=count;register void**apso asm("$18")=objects;
 U raised,origin,position,difference;
 register CM*camera asm("$2")=&D_002DDF40;
 position.q=pos;
 qword cpu,point,vector;ReadInputs(camera,position,cpu,point,vector);qword delta=Sub(point,vector);origin.q=cpu;difference.q=delta;
 distance*=D_002BE5B8;
 if(distance<Len3(difference.q,difference))return 0;
 qword screen=ConvertCmWorldToScreen(camera,LoadCpu(position));
 register float zLimit asm("$f1")=D_002BE5BC;
 if(Z(screen)<zLimit)return 0;
 if(!func_00173588(screen,D_002BE5C0))return 0;
 if(p->edge&&!func_00173588(screen,D_002BE5C4))return 1;
 if(p->range&&TooFar(difference.q,p->planar))return 2;
 if(p->occlusion){
  {register qword copied asm("$5")=position.q;raised.q=copied;}raised.f[2]+=D_002BE5C8;
  if(PsoHitTestLineObjects__FiP6VECTORT1iPP2SOP3LSG(0,origin.q,raised.q,cso,apso,0))return 3;
 }
 return 0;
}

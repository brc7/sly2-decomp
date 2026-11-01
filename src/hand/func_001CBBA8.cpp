typedef int qword __attribute__((mode(TI)));
union U {qword q;float f;};
extern float D_002C1ECC;
struct BOUND_PAGE {unsigned char unknown_0000[0x1ECC];float bound;};
struct START {qword q;START(const U&source,float zero,float*lowOut,float*highOut){
 register BOUND_PAGE*page __asm__("$2");
 __asm__("lui %0,%%hi(%1)":"=r"(page):"i"(&D_002C1ECC));
 register qword point __asm__("$2");register float low __asm__("$f12"),high __asm__("$f13");
 __asm__("mov.s %1,%3\n\tlwc1 %2,%%lo(%4)(%6)\n\tlq %0,%5":"=r"(point),"=f"(low),"=f"(high):"f"(zero),"i"(&D_002C1ECC),"m"(source.q),"r"(page),"m"(page->bound));
 q=point;*lowOut=low;*highOut=high;
}};struct M3 {qword a,b,c;};struct S;
extern float D_002C1EC4,D_002C1EC8;
extern qword D_002A2DC0,D_002A2DA0;
extern "C" {
float func_001E8008(S*,float,float);void func_001E6F10(S*,qword*,qword*,float);
float func_0011D448(float,float);qword func_0011E1A0(float,float,float);
void func_001CBBA8(S*,qword*,qword*,float);
}
static inline qword CrossKeepSecond(qword a,const qword *pb){qword b;__asm__("lqc2 %0,%1":"=j"(b):"m"(*pb));qword r;__asm__("vopmula.xyz $ACC,%1,%2\n\tvopmsub.xyz %0,%2,%1":"=j"(r):"j"(a),"0"(b));return r;}
static inline qword Dir(const M3 &m,qword a){__asm__("vmulax.xyz $ACC,%1,%0x\n\tvmadday.xyz $ACC,%2,%0y\n\tvmaddz.xyz %0,%3,%0z":"+j"(a):"j"(m.a),"j"(m.b),"j"(m.c));return a;}
static inline qword AddScaled(qword a,qword b,qword s){register qword r __asm__("vf2");__asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=j"(r):"j"(a),"j"(b),"j"(s));return r;}
static inline qword NormalizeOr(qword value,const qword*fallbackAddress,float epsilon){
 register qword length __asm__("vf1");register qword one __asm__("vf2");register qword result __asm__("vf2");float scalar;register qword fallback __asm__("$3");
 __asm__("vmul.xyz %0,%3,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z\n\tlq %2,%4":"=j"(length),"=&j"(one),"=r"(fallback):"j"(value),"m"(*fallbackAddress));
 __asm__("vsqrt $Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"=j"(length):"j"(length));
 __asm__("mtc1 %1,%0":"=f"(scalar):"r"(length));
 if(scalar<epsilon)return fallback;__asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q":"=j"(result):"j"(length),"j"(value));return result;
}
static inline qword LocalDirection(qword v){register qword q __asm__("vf4");__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(v));return q;}
static inline void SamplerArguments(S*p,float time,S**receiverOut,float*timeOut){
 register S*receiver __asm__("$4");register float parameter __asm__("$f12");
 __asm__("mov.s %1,%3\n\tdaddu %0,%2,$zero":"=r"(receiver),"=f"(parameter):"r"(p),"f"(time));
 *receiverOut=receiver;*timeOut=parameter;
}
static inline qword CrossAndFallbackAddress(qword a,const qword&b,const qword**addressOut,float*epsilonOut){
 register qword result __asm__("vf3"),axis __asm__("vf1");register const qword*address __asm__("$3");register float epsilon __asm__("$f1");
 /* VOPMSUB preserves the actual VF3.W left by the sampler. */
 __asm__ volatile("li.s %2,0.0001\n\tlqc2 %3,%5\n\tlui %1,%%hi(%6)\n\tvopmula.xyz $ACC,%4,%3\n\tvopmsub.xyz %0,%3,%4\n\taddiu %1,%1,%%lo(%6)":"=&j"(result),"=r"(address),"=f"(epsilon),"=&j"(axis):"j"(a),"m"(b),"i"(&D_002A2DA0):"$1");
 *addressOut=address;*epsilonOut=epsilon;return result;
}
static inline qword DisplacementValue(const qword&start,const qword&direction,float distance,U&scalar){
 register qword point __asm__("vf3"),vector __asm__("vf4"),scale __asm__("vf1");register int word __asm__("$2");
 __asm__("lqc2 %0,%3\n\tlqc2 %1,%4\n\tmfc1 %2,%5":"=j"(point),"=j"(vector),"=r"(word):"m"(start),"m"(direction),"f"(distance));
 __asm__("qmtc2.ni %1,%0":"=j"(scale):"r"(word));scalar.q=scale;
 return AddScaled(point,vector,scalar.q);
}
/** @sly2-annot addr=0x1CBBA8
 * Purpose: GRABSPOT iface: orientation basis along rail at distance
 * Info: proposed name GetRailGrabMat (T2); class RAIL; TU rail
 * Notes: names_v2 C proposed SetHbskHbsks__FP4HBSK5HBSKS (0.438)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r5_1AE130
 * See: docs/reference/functions/player.md#rail-0x1cb908-0x1cbeb0
 * @end-sly2-annot */
void func_001CBBA8(S*p,qword*posOut,qword*dirOut,float g){
 U pos;
 M3 basis;
 U scalar,moved,out;
 float x=func_001E8008(p,g,D_002C1EC4);
 S*sampleReceiver;float sampleTime;SamplerArguments(p,x,&sampleReceiver,&sampleTime);
 func_001E6F10(sampleReceiver,&pos.q,&basis.c,sampleTime);
 const qword*fallbackAddress;float epsilon;
 qword cross=CrossAndFallbackAddress(basis.c,D_002A2DC0,&fallbackAddress,&epsilon);
 qword right=NormalizeOr(cross,fallbackAddress,epsilon);
 basis.a=CrossKeepSecond(right,&basis.c);basis.b=right;
 float r=func_0011D448(-1.0f,1.0f);register float zero __asm__("$f22")=0.0f;float ar=__builtin_fabsf(r);
 qword local=func_0011E1A0(r*ar*D_002C1EC8,1.0f,zero);
 qword dir=Dir(basis,LocalDirection(local));
 qword cpu;__asm__("qmfc2.ni %0,%1":"=r"(cpu):"j"(dir));
 out.q=dir;moved.q=cpu;
 if(posOut){
  float low,high;START start(pos,zero,&low,&high);
  float distance=ar*func_0011D448(low,high);
  *posOut=DisplacementValue(start.q,moved.q,distance,scalar);
 }
 if(dirOut)*dirOut=out.q;
}

#define OFF(t,m) ((int)&((t*)0)->m)
typedef char layout_qword_size[(sizeof(qword)==16)?1:-1];
typedef char layout_qword_alignment[(__alignof__(qword)==16)?1:-1];
typedef char layout_float_size[(sizeof(float)==4)?1:-1];
typedef char layout_float_alignment[(__alignof__(float)==4)?1:-1];
typedef char layout_integer_size[(sizeof(int)==4)?1:-1];
typedef char layout_pointer_size[(sizeof(void*)==4)?1:-1];
typedef char layout_scalar_home_size[(sizeof(U)==16)?1:-1];
typedef char layout_scalar_home_alignment[(__alignof__(U)==16)?1:-1];
typedef char layout_scalar_qword_offset[(OFF(U,q)==0)?1:-1];
typedef char layout_scalar_float_offset[(OFF(U,f)==0)?1:-1];
typedef char layout_basis_size[(sizeof(M3)==0x30)?1:-1];
typedef char layout_basis_alignment[(__alignof__(M3)==16)?1:-1];
typedef char layout_basis_first[(OFF(M3,a)==0)?1:-1];
typedef char layout_basis_second[(OFF(M3,b)==0x10)?1:-1];
typedef char layout_basis_third[(OFF(M3,c)==0x20)?1:-1];
typedef char layout_start_size[(sizeof(START)==16)?1:-1];
typedef char layout_start_alignment[(__alignof__(START)==16)?1:-1];
typedef char layout_start_qword[(OFF(START,q)==0)?1:-1];
typedef char layout_bound_page_field[(OFF(BOUND_PAGE,bound)==0x1ECC)?1:-1];
typedef char layout_bound_page_size[(sizeof(BOUND_PAGE)==0x1ED0)?1:-1];
typedef char layout_bound_page_alignment[(__alignof__(BOUND_PAGE)==4)?1:-1];

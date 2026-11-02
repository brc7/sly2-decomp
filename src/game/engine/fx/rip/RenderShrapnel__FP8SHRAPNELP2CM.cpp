// Render the particle with its scalar depth offset along the global axis.
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
union F {float value;int bits;};
struct M3 {qword rows[3];};
struct SHRAPNEL {char pad40[0x40];M3 matrix;qword position;char pad80[0x70];F depth;};
struct CM;
extern qword D_002A2DC0;
extern "C" int FRenderRipPosMat(SHRAPNEL *,qword,const M3 &);
static inline qword AddScaled(qword p,qword a,qword k){__asm__("vadda.xyzw ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x":"+j"(p):"j"(a),"j"(k));return p;}
static inline SHRAPNEL* Receiver(SHRAPNEL*input){register SHRAPNEL*p __asm__("$3");__asm__("daddu %0,%1,$zero":"=r"(p):"r"(input));return p;}
static inline const qword* AxisAddress(void){register const qword*p __asm__("$2");__asm__("lui %0,%%hi(%1)\n\taddiu %0,%0,%%lo(%1)":"=r"(p):"i"(&D_002A2DC0));return p;}
static inline qword Axis(const qword*p){register qword v __asm__("vf3");__asm__("lqc2 %0,0(%1)":"=j"(v):"r"(p),"m"(*p));return v;}
static inline const M3* MatrixAddress(SHRAPNEL*p){register SHRAPNEL*receiver __asm__("$4")=p;register const M3*matrix __asm__("$6");__asm__("addiu %0,%1,64":"=r"(matrix):"r"(receiver));return matrix;}
static inline qword Depth(SHRAPNEL*p){
 register qword k __asm__("vf2");register int bits __asm__("$4");
 __asm__("lw %1,0xF0(%2)\n\tqmtc2.ni %1,%0":"=j"(k),"=&r"(bits):"r"(p),"m"(p->depth.bits));
 return k;
}
/** @sly2-annot addr=0x1CFC88
 * Purpose: SHRAPNEL Render
 * Info: proposed name RenderShrapnel__FP8SHRAPNELP2CM (EXISTING); TU rip
 * Confidence: high [EXISTING 0.85] - evidence: pre-existing symbol in symbol_addrs.txt; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void RenderShrapnel(SHRAPNEL *input,CM *){
    SHRAPNEL *p=Receiver(input);
    U point,scalar;
    qword k=Depth(p);
    point.q=AddScaled(p->position,Axis(AxisAddress()),k);
    scalar.q=k;
    FRenderRipPosMat(p,point.q,*MatrixAddress(p));
}

typedef char layout_render_0[(sizeof(qword)==16)?1:-1];
typedef char layout_render_1[(sizeof(U)==16)?1:-1];
typedef char layout_render_2[(sizeof(F)==4)?1:-1];
typedef char layout_render_3[(sizeof(M3)==0x30)?1:-1];
typedef char layout_render_4[(sizeof(void*)==4)?1:-1];
typedef char layout_render_5[((unsigned)&((SHRAPNEL*)0)->matrix==0x40)?1:-1];
typedef char layout_render_6[((unsigned)&((SHRAPNEL*)0)->position==0x70)?1:-1];
typedef char layout_render_7[((unsigned)&((SHRAPNEL*)0)->depth==0xF0)?1:-1];

typedef char layout_render_8[(sizeof(float)==4)?1:-1];
typedef char layout_render_9[(sizeof(int)==4)?1:-1];
typedef char layout_render_10[(__alignof__(qword)==16)?1:-1];
typedef char layout_render_11[((unsigned)&((F*)0)->bits==0)?1:-1];

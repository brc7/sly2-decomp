typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4]; };
struct B { qword x,y; };
struct MIX_STATE {union U direction,scalar;};
struct S { char pad[0x70]; qword pos; char pad80[0x10]; qword vel; };
extern qword D_002A2DD0,D_002A2DA0,D_002A2DC0;
union FLOATWORD {float value;int bits;};
struct CLOCK {float time;union FLOATWORD dt;};
extern union FLOATWORD D_002DDED4;
void ProjectRipTransform(struct S*);
float GRandInRange__Fff(float,float);
int func_0011D4D0(float,float,float);
void GetNormalVectors(qword,qword,qword,struct B*);
void CalculateSinCos__FfPfT1(float,float*,float*);
static inline const void*ClockHigh(void){register const void*page __asm__("$2");__asm__("lui %0,%%hi(D_002DDED4)":"=r"(page));return page;}
static inline qword Factors(const float*cs,float speed,float*first,float*second){
 register float a __asm__("$f1")=cs[0],b __asm__("$f0");
 const void*page=ClockHigh();const union FLOATWORD*wordView=(const union FLOATWORD*)((const char*)page-0x212C);
 register int word __asm__("$2");register qword dt __asm__("vf5");
 __asm__("lwc1 %1,%5\n\tmul.s %0,%0,%4\n\tlw %2,%%lo(D_002DDED4)(%6)\n\tqmtc2.ni %2,%3\n\tmul.s %1,%1,%4":"+&f"(a),"=&f"(b),"=r"(word),"=j"(dt):"f"(speed),"m"(cs[1]),"r"(page),"m"(wordView->bits));
 *first=a;*second=b;return dt;
}
static inline float X(qword q) {float f; __asm__("mtc1 %1,%0" : "=f"(f) : "r"(q)); return f;}
static inline float Y(qword q) {float f; __asm__("prot3w %0,%1" : "=r"(f) : "r"(q)); return f;}
static inline float Z(qword q) {float f; __asm__("pextuw %0,$zero,%1" : "=r"(f) : "r"(q)); return f;}
static inline void FNear(int *pf,qword a,qword b) { if(func_0011D4D0(X(b),X(a),0.0001f)) if(func_0011D4D0(Y(b),Y(a),0.0001f)) *pf=func_0011D4D0(Z(b),Z(a),0.0001f)!=0; }
static inline qword BcX(float f){register qword value __asm__("vf3");__asm__("qmtc2.ni %1,%0":"=j"(value):"r"(f));return value;}
static inline qword BcYAndPublishX(float f,qword x,union U*home){
 register qword value __asm__("vf4");
 __asm__("sqc2 %2,%1\n\tmfc1 $2,%3\n\tqmtc2.ni $2,%0"
     :"=j"(value),"=m"(home->q):"j"(x),"f"(f):"$2");
 return value;
}
static inline void MixAndAdvance(struct S*owner,const struct B*basis,struct MIX_STATE*state,qword sx,qword sy,qword dt){
 register qword y __asm__("vf1")=basis->y,x __asm__("vf2")=basis->x;
 register qword coefficientX __asm__("vf3")=sx,coefficientY __asm__("vf4")=sy,time __asm__("vf5")=dt;
 __asm__("vmulax.xyzw $ACC,%0,%4x\n\tvmaddx.xyzw %0,%1,%5x\n\t"
         "lqc2 %1,%2\n\tsqc2 %5,16+%3\n\tvadda.xyzw $ACC,%1,$vf0\n\t"
         "vmaddx.xyzw %1,%0,%6x\n\tsqc2 %1,%2\n\tsqc2 %0,%3\n\tsqc2 %6,16+%3"
   :"+&j"(x),"+&j"(y),"+m"(owner->pos),"=m"(*state)
   :"j"(coefficientX),"j"(coefficientY),"j"(time));
}
static inline qword NormalizeAndAxes(const struct S*owner,qword*rawPage,qword*xAxis,qword*upAxis){
 register qword velocity __asm__("vf1"),length __asm__("vf2");
 register qword*up __asm__("$2");register qword*x __asm__("$3")=rawPage;
 register qword upValue __asm__("$6"),xValue __asm__("$5");
 __asm__("lqc2 %0,%4\n\tvmul.xyz %1,%0,%0\n\tvaddw.x $vf3,$vf0,$vf0w\n\t"
         "vadday.x $ACC,%1,%1y\n\tvmaddz.x %1,$vf3,%1z\n\t"
         "lui %2,%%hi(%5)\n\taddiu %2,%2,%%lo(%5)\n\taddiu %3,%3,%%lo(%6)"
   :"=j"(velocity),"=j"(length),"=&r"(up),"+r"(x)
   :"m"(owner->vel),"i"(&D_002A2DC0),"i"(&D_002A2DA0):"vf3");
 __asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q\n\tlq %1,%4\n\t"
         "vdiv $Q,$vf0w,%0x\n\tvwaitq\n\tvmulq.xyzw %0,%3,$Q\n\tlq %2,%5"
   :"+&j"(length),"=&r"(upValue),"=r"(xValue)
   :"j"(velocity),"m"(*up),"m"(*x));
 *xAxis=xValue;*upAxis=upValue;return length;
}
static inline void BasisArguments(qword normal,struct B*destination,qword*argument,struct B**receiver){
 register qword value __asm__("$4");register struct B*output __asm__("$7");
 __asm__("qmfc2.ni %0,%2\n\tla %1,%a3"
     :"=&r"(value),"=r"(output):"j"(normal),"p"(destination));
 *argument=value;*receiver=output;
}
/** @sly2-annot addr=0x1D0EB8
 * Purpose: RIPT 0 Project: wobble + base ProjectRipTransform
 * Info: proposed name ProjectBubletTransform (T2); TU rip
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/particles-effects.md#rip-0x1cdcd0-0x1d1370
 * @end-sly2-annot */
void func_001D0EB8(struct S*p) {
    int near=0,choose=0;
    float roll;
    ProjectRipTransform(p);
    roll=GRandInRange__Fff(0.0f,1.0f);
    FNear(&near,p->vel,D_002A2DD0);
    if(!near&&roll<0.3f) choose=1;
    if(choose) {
        union U norm;
        struct B basis;
        struct MIX_STATE state;
        float speed,angle,cs[2],cx,cy;
        qword kx,ky,kt;
        qword xAxis,upAxis,normalArgument;struct B*basisOutput;
        norm.q=NormalizeAndAxes(p,(qword*)0x002A0000,&xAxis,&upAxis);
        BasisArguments(norm.q,&basis,&normalArgument,&basisOutput);
        GetNormalVectors(normalArgument,xAxis,upAxis,basisOutput);
        speed=GRandInRange__Fff(100.0f,100.0f+(1.0f-roll*3.33333325386f)*300.0f);
        angle=GRandInRange__Fff(0.0f,6.28318548203f);
        CalculateSinCos__FfPfT1(angle,&cs[0],&cs[1]);
        kt=Factors(cs,speed,&cx,&cy);
        kx=BcX(cx);ky=BcYAndPublishX(cy,kx,&state.scalar);
        MixAndAdvance(p,&basis,&state,kx,ky,kt);
    }
}

#define MIX_OFF(t,m) ((unsigned int)&((t*)0)->m)
typedef char layout_qword_size[(sizeof(qword)==16)?1:-1];
typedef char layout_union_size[(sizeof(union U)==16)?1:-1];
typedef char layout_basis_size[(sizeof(struct B)==0x20)?1:-1];
typedef char layout_basis_x[(MIX_OFF(struct B,x)==0)?1:-1];
typedef char layout_basis_y[(MIX_OFF(struct B,y)==0x10)?1:-1];
typedef char layout_owner_position[(MIX_OFF(struct S,pos)==0x70)?1:-1];
typedef char layout_owner_velocity[(MIX_OFF(struct S,vel)==0x90)?1:-1];
typedef char layout_word_size[(sizeof(union FLOATWORD)==4)?1:-1];
typedef char layout_clock_size[(sizeof(struct CLOCK)==8)?1:-1];
typedef char layout_clock_dt[(MIX_OFF(struct CLOCK,dt)==4)?1:-1];
typedef char layout_int_size[(sizeof(int)==4)?1:-1];
typedef char layout_float_size[(sizeof(float)==4)?1:-1];
typedef char layout_state_size[(sizeof(struct MIX_STATE)==32)?1:-1];
typedef char layout_state_direction[(MIX_OFF(struct MIX_STATE,direction)==0)?1:-1];
typedef char layout_state_scalar[(MIX_OFF(struct MIX_STATE,scalar)==16)?1:-1];
typedef char layout_pointer_size[(sizeof(void*)==4)?1:-1];
typedef char layout_unsigned_size[(sizeof(unsigned int)==4)?1:-1];
typedef char layout_short_size[(sizeof(short)==2)?1:-1];
typedef char layout_qword_alignment[(__alignof__(qword)==16)?1:-1];
typedef char layout_union_alignment[(__alignof__(union U)==16)?1:-1];
typedef char layout_basis_alignment[(__alignof__(struct B)==16)?1:-1];
typedef char layout_owner_alignment[(__alignof__(struct S)==16)?1:-1];
typedef char layout_state_alignment[(__alignof__(struct MIX_STATE)==16)?1:-1];
typedef char layout_owner_size[(sizeof(struct S)==0xA0)?1:-1];
typedef char layout_union_qword[(MIX_OFF(union U,q)==0)?1:-1];
typedef char layout_union_float[(MIX_OFF(union U,f)==0)?1:-1];
typedef char layout_word_float[(MIX_OFF(union FLOATWORD,value)==0)?1:-1];
typedef char layout_word_integer[(MIX_OFF(union FLOATWORD,bits)==0)?1:-1];
typedef char layout_clock_time[(MIX_OFF(struct CLOCK,time)==0)?1:-1];

// Update directional acceleration and side state.
typedef int qword __attribute__((mode(TI)));
typedef union{qword q;float f[4];int i[4];}U;
struct M4{qword r[4];};
struct VT{short delta,pad;void*fn;};
struct O{char pad[0x494];struct O*anchor;};
struct ANIM{char pad[0x40];int ratebits;};
struct P{char pad[0xC];struct VT*vt;char pad10[0x180];U acceleration;char pad1A0[8];float heading;char pad1AC[0xE84];struct ANIM*anim;int pad1034;int side;char pad103C[0xA0];struct O*target;int pad10E0;int targetMode;};
struct AXES{char pad[0x10];qword a;char pad20[0x10];qword b;char pad40[0x10];qword c;char pad60[0x10];qword d;char pad80[0x10];qword e;char padA0[0x10];qword f;};
extern float D_002BF9F0;
void func_002046F0(struct P*,struct O*);
float func_0011CF60(float);
qword func_0011E1A0(float,float,float);
struct M4*func_001A6318(struct P*);
static inline qword GetVel(struct P*p){return ((qword(*)(void*))p->vt[33].fn)((char*)p+p->vt[33].delta);}
static inline void SetVel(struct P*p,qword v){((void(*)(void*,qword))p->vt[35].fn)((char*)p+p->vt[35].delta,v);}
static inline qword ScaleAndSetSide(struct P*p,qword v,qword k,int side){qword r;__asm__("vmulx.xyzw %0,%2,%3x\n\tsw %4,%1":"=j"(r),"=m"(p->side):"j"(v),"j"(k),"r"(side));return r;}
static inline qword ToVu(qword bits){register qword r __asm__("vf3");__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(bits));return r;}
static inline qword ProjectOut(qword ni,qword vi){
 register qword n __asm__("vf1")=ni,v __asm__("vf3")=vi;
 register qword dot __asm__("vf2"),one __asm__("vf4");
 __asm__("vmul.xyz %0,%2,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(dot),"=&j"(one):"j"(v),"j"(n));
 __asm__("vmulx.xyzw %0,%1,%0x":"+j"(dot):"j"(n));
 __asm__("vsub.xyzw %0,%0,%1":"+j"(v):"j"(dot));
 return v;
}
static inline void RemoveSide(struct P*p,U*result,U*old){
 struct M4*basis;
 qword value;
 old->q=ToVu(GetVel(p));
 basis=func_001A6318(p);
 value=ProjectOut(basis->r[1],old->q);
 result->q=value;
 SetVel(p,value);
}
static inline qword PureBits(int bits){qword q;__asm__("qmtc2.ni %1,%0":"=j"(q):"r"(bits));return q;}
static inline void BlendPositive(struct P*p,struct AXES*axes,float xin,const U*input,U*out){
 float x=xin;
 register float y __asm__("$f3"),zero __asm__("$f0"),left __asm__("$f2"),right __asm__("$f0");
 register qword a __asm__("vf2"),b __asm__("vf1"),c __asm__("vf4");
 register qword ka __asm__("vf3"),kb __asm__("vf5"),kc __asm__("vf6");
 register int ybits __asm__("$2"),leftbits __asm__("$2");
 __asm__("lwc1 %1,%3\n\tmtc1 $%2,%0":"=f"(zero),"=f"(y):"J"(0),"m"(input->f[1]));
 __asm__("mfc1 %0,%3\n\tmin.s %1,%4,%5\n\tlqc2 %2,%6":"=&r"(ybits),"=f"(left),"=j"(c):"f"(y),"f"(zero),"f"(x),"m"(axes->f));
 __asm__("qmtc2.ni %4,%0\n\tneg.s %1,%1\n\tmax.s %2,%5,%6\n\tsqc2 %0,%3":"=j"(ka),"+&f"(left),"=f"(right),"=m"(out->q):"r"(ybits),"f"(zero),"f"(x));
 __asm__("qmtc2.ni %1,%0":"=j"(kb):"r"(right));
 __asm__("mfc1 %0,%4\n\tlqc2 %1,%5\n\tqmtc2.ni %0,%2\n\tlqc2 %3,%6":"=&r"(leftbits),"=j"(a),"=j"(kc),"=j"(b):"f"(left),"m"(axes->b),"m"(axes->d));
 out->q=kb;
 __asm__("vmulax.xyzw ACC,%1,%4x\n\tvmaddax.xyzw ACC,%2,%5x\n\tvmaddx.xyzw %0,%3,%6x":"=j"(ka):"j"(a),"j"(b),"j"(c),"0"(ka),"j"(kb),"j"(kc));
 p->acceleration.q=ka;out->q=kc;
}
static inline void BlendNegative(struct P*p,struct AXES*axes,float xin,const U*input,U*out){
 float x=xin;
 register float y __asm__("$f2"),zero __asm__("$f0"),left __asm__("$f3"),right __asm__("$f0"),negativeY __asm__("$f2");
 register qword a __asm__("vf3"),b __asm__("vf1"),c __asm__("vf4");
 register qword ka __asm__("vf2"),kb __asm__("vf6"),kc __asm__("vf5");
 __asm__("mtc1 $%2,%0\n\tlwc1 %1,%3":"=f"(zero),"=f"(y):"J"(0),"m"(input->f[1]));
 __asm__("min.s %0,%2,%3\n\tlqc2 %1,%4":"=f"(left),"=j"(c):"f"(zero),"f"(x),"m"(axes->e));
 __asm__("max.s %0,%2,%3\n\tneg.s %1,%4":"=&f"(right),"=f"(negativeY):"0"(zero),"f"(x),"f"(y));
 __asm__("qmtc2.ni %2,%0\n\tneg.s %1,%1":"=j"(kb),"+f"(left):"r"(right));
 __asm__("qmtc2.ni %2,%0\n\tsqc2 %0,%1":"=j"(ka),"=m"(out->q):"r"(negativeY));
 __asm__("qmtc2.ni %3,%0\n\tlqc2 %1,%4\n\tlqc2 %2,%5":"=j"(kc),"=j"(a),"=j"(b):"r"(left),"m"(axes->a),"m"(axes->c));
 out->q=kb;
 __asm__("vmulax.xyzw ACC,%1,%4x\n\tvmaddax.xyzw ACC,%2,%5x\n\tvmaddx.xyzw %0,%3,%6x":"=j"(ka):"j"(a),"j"(b),"j"(c),"0"(ka),"j"(kb),"j"(kc));
 __asm__("sqc2 %2,%0\n\tsqc2 %3,%1":"=m"(p->acceleration.q),"=m"(out->q):"j"(ka),"j"(kc));
}
/** @sly2-annot addr=0x1A0720
 * Purpose: Update directional acceleration and side state from heading/magnitude
 * Info: proposed name UpdateJtDirAccel (T2); TU dirblend
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/animation.md#dirblend-0x1a0300-0x1a0b18
 * @end-sly2-annot */
void func_001A0720(struct P*p,struct AXES*axes,int enabled,float heading,float magnitude){
 U input,scalar,old;
 register int side __asm__("$3");
 float zero;
 struct O*target;
 if(!enabled)return;
 target=p->target;
 if(target){if(p->targetMode==3)target=target->anchor;
 func_002046F0(p,target);}
 zero=0.0f;
 input.q=func_0011E1A0(func_0011CF60(heading-p->heading),magnitude,zero);
 if(D_002BF9F0<input.f[1]){
  if(p->side==0){input.f[1]=zero;RemoveSide(p,&scalar,&old);}
  side=1;
 }else if(input.f[1]<-D_002BF9F0){
  if(p->side==1){input.f[1]=zero;RemoveSide(p,&scalar,&old);}
  side=0;
 }else{side=p->side;if(side==-1)side=0;}
 {register float x __asm__("$f1")=input.f[0];
 if(side==1){
  BlendPositive(p,axes,x,&input,&scalar);
 }else{
  BlendNegative(p,axes,x,&input,&scalar);
 }
 }
 p->acceleration.i[2]=0;
 {unsigned int animWord=(unsigned int)p->anim;register qword rate __asm__("vf2"),acceleration __asm__("vf1"),scaled __asm__("vf1");
 __asm__("lqc2 %0,%2\n\tlw %1,%3":"=j"(acceleration),"+r"(animWord):"m"(p->acceleration.q),"m"(((struct ANIM*)animWord)->ratebits));
 rate=PureBits(animWord);scaled=ScaleAndSetSide(p,acceleration,rate,side);
  __asm__("sqc2 %2,%0\n\tsqc2 %3,%1":"=m"(p->acceleration.q),"=m"(scalar.q):"j"(scaled),"j"(rate));
 }
}

#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef char layout_qword_size[(sizeof(qword)==16)?1:-1];
typedef char layout_qword_alignment[(__alignof__(qword)==16)?1:-1];
typedef char layout_U_size[(sizeof(U)==16)?1:-1];
typedef char layout_U_alignment[(__alignof__(U)==16)?1:-1];
typedef char layout_U_q[(FIELD_OFFSET(U,q)==0)?1:-1];
typedef char layout_U_f[(FIELD_OFFSET(U,f)==0)?1:-1];
typedef char layout_U_i[(FIELD_OFFSET(U,i)==0)?1:-1];
typedef char layout_M4_size[(sizeof(struct M4)==64)?1:-1];
typedef char layout_M4_alignment[(__alignof__(struct M4)==16)?1:-1];
typedef char layout_M4_rows[(FIELD_OFFSET(struct M4,r)==0)?1:-1];
typedef char layout_M4_row1[(FIELD_OFFSET(struct M4,r[1])==16)?1:-1];
typedef char layout_VT_size[(sizeof(struct VT)==8)?1:-1];
typedef char layout_VT_delta[(FIELD_OFFSET(struct VT,delta)==0)?1:-1];
typedef char layout_VT_fn[(FIELD_OFFSET(struct VT,fn)==4)?1:-1];
typedef char layout_O_prefix_size[(sizeof(struct O)==0x498)?1:-1];
typedef char layout_O_anchor[(FIELD_OFFSET(struct O,anchor)==0x494)?1:-1];
typedef char layout_ANIM_prefix_size[(sizeof(struct ANIM)==0x44)?1:-1];
typedef char layout_ANIM_ratebits[(FIELD_OFFSET(struct ANIM,ratebits)==0x40)?1:-1];
typedef char layout_P_prefix_size[(sizeof(struct P)==0x10F0)?1:-1];
typedef char layout_P_alignment[(__alignof__(struct P)==16)?1:-1];
typedef char layout_P_vt[(FIELD_OFFSET(struct P,vt)==0xC)?1:-1];
typedef char layout_P_acceleration[(FIELD_OFFSET(struct P,acceleration)==0x190)?1:-1];
typedef char layout_P_acceleration_z[(FIELD_OFFSET(struct P,acceleration.i[2])==0x198)?1:-1];
typedef char layout_P_heading[(FIELD_OFFSET(struct P,heading)==0x1A8)?1:-1];
typedef char layout_P_anim[(FIELD_OFFSET(struct P,anim)==0x1030)?1:-1];
typedef char layout_P_side[(FIELD_OFFSET(struct P,side)==0x1038)?1:-1];
typedef char layout_P_target[(FIELD_OFFSET(struct P,target)==0x10DC)?1:-1];
typedef char layout_P_targetMode[(FIELD_OFFSET(struct P,targetMode)==0x10E4)?1:-1];
typedef char layout_AXES_size[(sizeof(struct AXES)==0xC0)?1:-1];
typedef char layout_AXES_alignment[(__alignof__(struct AXES)==16)?1:-1];
typedef char layout_AXES_a[(FIELD_OFFSET(struct AXES,a)==0x10)?1:-1];
typedef char layout_AXES_b[(FIELD_OFFSET(struct AXES,b)==0x30)?1:-1];
typedef char layout_AXES_c[(FIELD_OFFSET(struct AXES,c)==0x50)?1:-1];
typedef char layout_AXES_d[(FIELD_OFFSET(struct AXES,d)==0x70)?1:-1];
typedef char layout_AXES_e[(FIELD_OFFSET(struct AXES,e)==0x90)?1:-1];
typedef char layout_AXES_f[(FIELD_OFFSET(struct AXES,f)==0xB0)?1:-1];
typedef char layout_int_size[(sizeof(int)==4)?1:-1];
typedef char layout_float_size[(sizeof(float)==4)?1:-1];
typedef char layout_pointer_size[(sizeof(void*)==4)?1:-1];

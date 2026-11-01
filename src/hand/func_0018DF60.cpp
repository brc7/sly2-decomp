// Smooth movement and update directional animation weights.
typedef int qword __attribute__((mode(TI)));
union U {qword q;float f[4];};
struct M3{qword r[3];};
struct M4{qword r[4];};
struct VTE{short delta,pad;void*fn;};
struct O{char pad[0xC];VTE*vt;};
struct PARAM{char pad[0x10];float y,z;int pad18;float weight;};
struct ANIM;
struct P{char pad[0xC];VTE*vt;char pad10[0x140];PARAM*params;char pad154[0x5C];M3 matrix;char pad1E0[0x16E4];O*object;char pad18C8[0x698];float base;char pad1F64[8];float left;char pad1F70[8];float middle;char pad1F7C[8];float right;char pad1F88[8];ANIM*anim;float amount,weight;char pad1F9C[4];qword offset;};
struct CLOCK{float time,dt;};
struct SMP{float a,b,c;};
extern "C" CLOCK D_002DDED0;
extern "C" M3 D_002DDFA0;
extern "C" qword D_002A2DC0;
extern "C" SMP D_002BF3F8,D_002BF408;
extern "C" M4*func_001A6318(P*);
extern "C" qword func_001A62E8(P*);
extern "C" void func_0011C950(M3*,M3*,SMP*,M3*,qword*,float);
extern "C" qword func_0011F088(qword,qword,SMP*,qword*,float);
extern "C" qword func_001A6FC0(O*,O*,qword);
extern "C" void func_001384C0(ANIM*);
static inline qword Make(float x,float y,float z){qword q,t;__asm__("mfc1 %0,%2\n\tmfc1 %1,%3\n\tpextlw %0,%1,%0\n\tmfc1 %1,%4\n\tpcpyld %0,%1,%0":"=&r"(q),"=&r"(t):"f"(x),"f"(y),"f"(z));return q;}
static inline float Max(float a,float b){float r;__asm__("max.s %0,%1,%2":"=f"(r):"f"(a),"f"(b));return r;}
static inline qword Normalize(qword vin,const U*negative,U*u){
 register qword v __asm__("vf3")=vin;
 register qword fallback __asm__("vf4");
 qword d;register qword one __asm__("vf2");
 float len;
 __asm__("vmul.xyz %0,%3,%3\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z\n\tlqc2 %2,%4":"=j"(d),"=&j"(one),"=j"(fallback):"j"(v),"m"(negative->q));
 __asm__("vsqrt Q,%1x\n\tvwaitq\n\tvaddq.x %0,$vf0,Q":"+j"(one):"j"(d));
 register int lenBits __asm__("$3");register unsigned page __asm__("$2");register qword up __asm__("vf1");
 __asm__("lui %0,%%hi(%5)\n\tqmfc2.ni %1,%4\n\taddiu %0,%0,%%lo(%5)\n\tmtc1 %1,%2\n\tlqc2 %3,0(%0)":"=&r"(page),"=&r"(lenBits),"=f"(len),"=j"(up):"j"(one),"i"(&D_002A2DC0):"memory");
 qword r;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(up),"j"(fallback));register qword bits __asm__("$2");__asm__("qmfc2.ni %0,%1":"=r"(bits):"j"(fallback));u->q=bits;
 if(len<0.0001f)return r;
 __asm__("vdiv Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,Q":"=j"(r):"j"(one),"j"(v));
 return r;
}

static inline P*NegativeTarget(const M4&m,const qword&offset,U*negative,U*scratch,U*target,P*p){
 register float minus __asm__("$f0")=-1.0f;register qword raw __asm__("$2");
 register qword axis1 __asm__("vf4"),axis2 __asm__("vf3"),scaled __asm__("vf2");register P*owner __asm__("$4");
 {
  register qword scalar __asm__("vf1");
  __asm__("daddu %0,%3,$zero\n\tmfc1 %1,%4\n\tlqc2 %2,%5":"=&r"(owner),"=&r"(raw),"=j"(axis1):"r"(p),"f"(minus),"m"(m.r[1]));
  __asm__("sq %4,%0\n\tqmtc2.ni %4,%1\n\tlqc2 %2,%5\n\tlqc2 %3,%6":"=m"(negative->q),"=j"(scalar),"=j"(axis2),"=j"(scaled):"r"(raw),"m"(m.r[2]),"m"(offset));
  __asm__("vmulx.xyzw %0,%0,%1x":"+j"(scaled):"j"(scalar));
 }
 register qword axis0 __asm__("vf5"),origin __asm__("vf1"),result __asm__("vf6");/* Preserve the inherited VF6.W after the direction callback. */
 __asm__("lqc2 %0,%2\n\tlqc2 %1,%3":"=j"(axis0),"=j"(origin):"m"(m.r[0]),"m"(m.r[3]));
 __asm__ volatile("vmulax.xyz ACC,%2,%6x\n\tvmadday.xyz ACC,%3,%6y\n\tvmaddaz.xyz ACC,%4,%6z\n\tvmaddw.xyz %0,%5,$vf0w\n\tsq %7,%1"
 :"=&j"(result),"=m"(scratch->q):"j"(axis0),"j"(axis1),"j"(axis2),"j"(origin),"j"(scaled),"r"(raw));
 target->q=result;return owner;
}


static inline float WeightAmount(qword normalized,float&zero){
 register qword raw __asm__("$2");register int bits __asm__("$3");register float z __asm__("$f2"),amount __asm__("$f0");
 __asm__("qmfc2.ni %0,%4\n\tprot3w %1,%0\n\tmtc1 $zero,%2\n\tmtc1 %1,%3":"=&r"(raw),"=&r"(bits),"=f"(z),"=f"(amount):"j"(normalized));
 zero=z;return amount;
}





static inline unsigned SmoothSamplePage(){register unsigned page __asm__("$6");__asm__("lui %0,%%hi(%1)":"=r"(page):"i"(&D_002BF3F8));return page;}
static inline float SmoothInputs(M4*source,const CLOCK*clock,float factor,unsigned page,M3*&current,M3*&optional,SMP*&sample){
 register M3*receiver __asm__("$4"),*out __asm__("$7");register float dt __asm__("$f12");register unsigned address __asm__("$6")=page;
 __asm__("lwc1 %0,%4\n\tdaddu %1,%5,$zero\n\taddiu %3,%3,%%lo(%7)\n\tdaddu %2,$zero,$zero\n\tmul.s %0,%0,%6":"=&f"(dt),"=r"(receiver),"=r"(out),"+r"(address):"m"(clock->dt),"r"(source),"f"(factor),"i"(&D_002BF3F8));
 current=receiver;optional=out;sample=(SMP*)address;return dt;
}

/** @sly2-annot addr=0x18DF60
 * Purpose: Smooth Sly's matrix/position (SmoothMatrix, PosSmooth) and update directional anim weights
 * Info: proposed name UpdateJtSmoothing (T2); TU jt
 * Notes: names_v2 C proposed UpdateSuvInfluences__FP3SUVP2RO (0.438)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
extern "C" void func_0018DF60(P*p){
 M4 m=*((M4*(*)(void*))p->object->vt[31].fn)((char*)p->object+p->object->vt[31].delta);
 U direction,position,scratch,negative;
 *(M3*)&p->matrix=*(M3*)&m;
 M4*current=func_001A6318(p);
 CLOCK*clock=&D_002DDED0;
 float factor=0.5f;
 unsigned samplePage=SmoothSamplePage();M3*receiver,*optional;SMP*sample;float smoothDt=SmoothInputs(current,clock,factor,samplePage,receiver,optional,sample);
 func_0011C950(receiver,&p->matrix,sample,optional,&direction.q,smoothDt);
 ((void(*)(void*,qword))p->vt[36].fn)((char*)p+p->vt[36].delta,direction.q);
 U target;P*owner=NegativeTarget(m,p->offset,&negative,&scratch,&target,p);
 (void)func_0011F088(func_001A62E8(owner),target.q,&D_002BF408,&position.q,clock->dt*factor);
 ((void(*)(void*,qword))p->vt[35].fn)((char*)p+p->vt[35].delta,position.q);
 qword relative;
 {
  register PARAM*params __asm__("$3");register M3*camera __asm__("$2");register float zero __asm__("$f2");
  __asm__("lw %0,%3\n\tlui %1,%%hi(%4)\n\tmtc1 $zero,%2\n\taddiu %1,%1,%%lo(%4)":"=&r"(params),"=r"(camera),"=f"(zero):"m"(p->params),"i"(&D_002DDFA0));
  register float y __asm__("$f0"),z __asm__("$f1");register O*root __asm__("$4");register qword axis2 __asm__("vf3");
  __asm__("lwc1 %0,%4\n\tdaddu %1,$zero,$zero\n\tlwc1 %2,%5\n\tneg.s %0,%0\n\tlqc2 %3,%6":"=f"(y),"=&r"(root),"=f"(z),"=j"(axis2):"m"(params->y),"m"(params->z),"m"(camera->r[2]));
  qword local=Make(zero,y,z);
  register qword axis0 __asm__("vf2"),axis1 __asm__("vf1"),rotated __asm__("vf4");
  __asm__("lqc2 %0,%3\n\tlqc2 %1,%4\n\tqmtc2.ni %5,%2":"=j"(axis0),"=j"(axis1),"=j"(rotated):"m"(camera->r[0]),"m"(camera->r[1]),"r"(local));
  register O*object __asm__("$5");
  __asm__("lw %1,%6\n\tvmulax.xyz ACC,%3,%0x\n\tvmadday.xyz ACC,%4,%0y\n\tvmaddz.xyz %0,%5,%0z\n\tsq %7,%2":"+j"(rotated),"=&r"(object),"=m"(scratch.q):"j"(axis0),"j"(axis1),"j"(axis2),"m"(p->object),"r"(local));
  relative=func_001A6FC0(root,object,rotated);
 }
 qword normalized=Normalize(relative,&negative,&scratch);
 register float zero __asm__("$f2");
 register float amount __asm__("$f0")=WeightAmount(normalized,zero);
 register float one __asm__("$f5")=1.0f;
 register float right __asm__("$f4")=Max(amount,zero);
 register float left __asm__("$f1")=-amount;
 register float middle __asm__("$f3");register PARAM*params __asm__("$2");
 __asm__("sub.s %3,%7,%8\n\tlw %0,%4\n\tswc1 %5,%1\n\tmax.s %2,%2,%6":"=&r"(params),"=m"(p->amount),"+f"(left),"=&f"(middle):"m"(p->params),"f"(amount),"f"(zero),"f"(one),"f"(right));
 register ANIM*animation __asm__("$4");register float weight __asm__("$f0");
 __asm__("lw %0,%5\n\tsub.s %1,%1,%3\n\tlwc1 %2,%6\n\tmul.s %3,%3,%2\n\tswc1 %2,%4":"=&r"(animation),"+&f"(middle),"=f"(weight),"+f"(left),"=m"(p->weight):"m"(p->anim),"m"(params->weight));
 __asm__("sub.s %0,%0,%4\n\tmul.s %1,%1,%4\n\tmul.s %2,%2,%4\n\tswc1 %5,%3":"+&f"(one),"+&f"(right),"+&f"(middle),"=m"(p->left):"f"(weight),"f"(left));
 p->right=right;p->middle=middle;p->base=one;
 func_001384C0(animation);
}

typedef char Qword_Size[(sizeof(qword)==16)?1:-1];
typedef char Float_Size[(sizeof(float)==4)?1:-1];
typedef char Int_Size[(sizeof(int)==4)?1:-1];
typedef char UInt_Size[(sizeof(unsigned)==4)?1:-1];
typedef char Short_Size[(sizeof(short)==2)?1:-1];
typedef char Pointer_Size[(sizeof(void*)==4)?1:-1];
typedef char U_Size[(sizeof(U)==16)?1:-1];
typedef char M3_Size[(sizeof(M3)==48)?1:-1];
typedef char M4_Size[(sizeof(M4)==64)?1:-1];
typedef char VTE_Size[(sizeof(VTE)==8)?1:-1];
typedef char O_Size[(sizeof(O)==16)?1:-1];
typedef char PARAM_Size[(sizeof(PARAM)==32)?1:-1];
typedef char SMP_Size[(sizeof(SMP)==12)?1:-1];
typedef char CLOCK_Size[(sizeof(CLOCK)==8)?1:-1];
typedef char P_Size[(sizeof(P)==8112)?1:-1];
typedef char qword_Alignment[(__alignof__(qword)==16)?1:-1];
typedef char U_Alignment[(__alignof__(U)==16)?1:-1];
typedef char M3_Alignment[(__alignof__(M3)==16)?1:-1];
typedef char M4_Alignment[(__alignof__(M4)==16)?1:-1];
typedef char P_Alignment[(__alignof__(P)==16)?1:-1];
typedef char U_q_Offset[((unsigned)&((U*)0)->q==0)?1:-1];
typedef char U_f_Offset[((unsigned)&((U*)0)->f==0)?1:-1];
typedef char M3_r_Offset[((unsigned)&((M3*)0)->r==0)?1:-1];
typedef char M4_r_Offset[((unsigned)&((M4*)0)->r==0)?1:-1];
typedef char VTE_delta_Offset[((unsigned)&((VTE*)0)->delta==0)?1:-1];
typedef char VTE_fn_Offset[((unsigned)&((VTE*)0)->fn==4)?1:-1];
typedef char O_vt_Offset[((unsigned)&((O*)0)->vt==12)?1:-1];
typedef char PARAM_y_Offset[((unsigned)&((PARAM*)0)->y==16)?1:-1];
typedef char PARAM_z_Offset[((unsigned)&((PARAM*)0)->z==20)?1:-1];
typedef char PARAM_weight_Offset[((unsigned)&((PARAM*)0)->weight==28)?1:-1];
typedef char SMP_a_Offset[((unsigned)&((SMP*)0)->a==0)?1:-1];
typedef char SMP_b_Offset[((unsigned)&((SMP*)0)->b==4)?1:-1];
typedef char SMP_c_Offset[((unsigned)&((SMP*)0)->c==8)?1:-1];
typedef char CLOCK_time_Offset[((unsigned)&((CLOCK*)0)->time==0)?1:-1];
typedef char CLOCK_dt_Offset[((unsigned)&((CLOCK*)0)->dt==4)?1:-1];
typedef char P_vt_Offset[((unsigned)&((P*)0)->vt==12)?1:-1];
typedef char P_params_Offset[((unsigned)&((P*)0)->params==336)?1:-1];
typedef char P_matrix_Offset[((unsigned)&((P*)0)->matrix==432)?1:-1];
typedef char P_object_Offset[((unsigned)&((P*)0)->object==6340)?1:-1];
typedef char P_base_Offset[((unsigned)&((P*)0)->base==8032)?1:-1];
typedef char P_left_Offset[((unsigned)&((P*)0)->left==8044)?1:-1];
typedef char P_middle_Offset[((unsigned)&((P*)0)->middle==8056)?1:-1];
typedef char P_right_Offset[((unsigned)&((P*)0)->right==8068)?1:-1];
typedef char P_anim_Offset[((unsigned)&((P*)0)->anim==8080)?1:-1];
typedef char P_amount_Offset[((unsigned)&((P*)0)->amount==8084)?1:-1];
typedef char P_weight_Offset[((unsigned)&((P*)0)->weight==8088)?1:-1];
typedef char P_offset_Offset[((unsigned)&((P*)0)->offset==8096)?1:-1];
typedef char M3_Row_Count[(sizeof(((M3*)0)->r)/sizeof(qword)==3)?1:-1];
typedef char M4_Row_Count[(sizeof(((M4*)0)->r)/sizeof(qword)==4)?1:-1];
typedef char M4_Basis_Prefix[(sizeof(M3)==(unsigned)&((M4*)0)->r[3])?1:-1];

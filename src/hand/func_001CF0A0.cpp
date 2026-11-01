typedef int qword __attribute__((mode(TI)));
union U {qword q;float f;};
struct M3 {U r[3];M3(){} M3(const M3&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];} M3 &operator=(const M3&m){r[0]=m.r[0];r[1]=m.r[1];r[2]=m.r[2];return *this;}};struct M4 {qword a,b,c,d;};
struct PHYS {qword accel,axis;float rate,scale;};
struct S {char pad[0x40];M3 mat;qword pos,prev,vel;PHYS phys;qword omega;float gE0,gE4;};
extern struct CLOCK {float time;union{float dt;int dtbits;};} D_002DDED0;
extern "C" {
int func_001CF010(qword);int func_001CEFC8(qword);int func_001CF058(qword);
M3*func_0011B500(M3*,qword,float);M4 *CalculateEmitdvMatrix__FP6EMITDVfP7MATRIX4(M4*,PHYS*);
void func_001CF0A0(S*);
}
static inline qword Scale(qword a,qword s){qword r;__asm__("vmulx.xyzw %0,%1,%2x":"=j"(r):"j"(a),"j"(s));return r;}
static inline qword Sq(qword a){qword r,one;__asm__("vmul.xyz %0,%2,%2\n\tvaddw.x %1,$vf0,$vf0w\n\tvadday.x $ACC,%0,%0y\n\tvmaddz.x %0,%1,%0z":"=j"(r),"=&j"(one):"j"(a));return r;}
static inline qword Sqrt(qword a){__asm__("vsqrt $Q,%0x\n\tvwaitq\n\tvaddq.x %0,$vf0,$Q":"+j"(a));return a;}
static inline float Float(qword a){float f;__asm__("mtc1 %1,%0":"=f"(f):"r"(a));return f;}
struct R3 {U r[3];};
struct MATH {U first,second;R3 scratch;};
union SHARED {MATH math;M4 matrix;};
struct WORK {SHARED shared;R3 rotation,product;};
static inline qword Broadcast(U*storage,float f){qword r;__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(f));storage->q=r;return storage->q;}
static inline qword BroadcastBits(U*storage,int f){qword r;__asm__("qmtc2.ni %1,%0":"=j"(r):"r"(f));storage->q=r;return storage->q;}
static inline qword Normalize(qword a,float f,U*storage){qword r,l=Broadcast(storage,f);__asm__("vdiv $Q,$vf0w,%1x\n\tvwaitq\n\tvmulq.xyzw %0,%2,$Q":"=j"(r):"j"(l),"j"(a));return r;}
static inline void Multiply(R3*scratch,R3*out,const R3*a,const M3&b){
 register qword row0 __asm__("vf4")=b.r[0].q;
 __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"+j"(row0):"j"(a->r[0].q),"j"(a->r[1].q),"j"(a->r[2].q),"j"(b.r[0].q));
 scratch->r[0].q=row0;out->r[0]=scratch->r[0];
 register qword row1 __asm__("vf2")=b.r[1].q;
 __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"+j"(row1):"j"(a->r[0].q),"j"(a->r[1].q),"j"(a->r[2].q),"j"(b.r[1].q));
 scratch->r[1].q=row1;out->r[1]=scratch->r[1];
 register qword input2 __asm__("vf1")=b.r[2].q;
 qword row2=a->r[0].q;
 __asm__("vmulax.xyz $ACC,%1,%4x\n\tvmadday.xyz $ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z":"+j"(row2):"j"(a->r[0].q),"j"(a->r[1].q),"j"(a->r[2].q),"j"(input2));
 out->r[2].q=row2;scratch->r[2]=out->r[2];
}
static inline qword Point(const M4 &m,qword a){register qword r __asm__("vf3")=m.a;__asm__("vmulax.xyz $ACC,%1,%5x\n\tvmadday.xyz $ACC,%2,%5y\n\tvmaddaz.xyz $ACC,%3,%5z\n\tvmaddw.xyz %0,%4,$vf0w":"+j"(r):"j"(m.a),"j"(m.b),"j"(m.c),"j"(m.d),"j"(a));return r;}
static inline qword AddScaled(qword a,qword b,qword s){qword r;__asm__("vadda.xyzw $ACC,%1,$vf0\n\tvmaddx.xyzw %0,%2,%3x":"=j"(r):"j"(a),"j"(b),"j"(s));return r;}
static inline int ClockWord(char*page){int word;__asm__("lw %0,%%lo(%2)(%1)":"=r"(word):"r"(page),"i"(&D_002DDED0.dtbits),"m"(*(const int*)(page-0x212C)));return word;}
static inline char*ClockPage(){register char*page __asm__("$2");__asm__("lui %0,%%hi(%1)":"=r"(page):"i"(&D_002DDED0.dtbits));return page;}
static inline void PublishBasisStart(S*p,qword first,qword second){__asm__("sqc2 %3,0x40(%2)\n\tsqc2 %4,0x50(%2)":"=m"(p->mat.r[0].q),"=m"(p->mat.r[1].q):"r"(p),"j"(first),"j"(second));}
void func_001CF0A0(S*p){
 WORK w;
 if(!func_001CF010(p->omega)) {
  qword omega=Scale(p->omega,Broadcast(&w.shared.math.first,p->gE4*D_002DDED0.dt+1.0f));
  p->omega=omega;
  float length=Float(Sqrt(Sq(omega)));
  if(0.0001f<length){
   w.shared.math.first.q=Normalize(omega,length,&w.shared.math.second);
   func_0011B500((M3*)&w.rotation,w.shared.math.first.q,length*D_002DDED0.dt);
   Multiply(&w.shared.math.scratch,&w.product,&w.rotation,p->mat);
   PublishBasisStart(p,w.product.r[0].q,w.product.r[1].q);p->mat.r[2]=w.shared.math.scratch.r[2];
  }
 }
 p->prev=p->pos;
 int moving=0;
 if(!func_001CEFC8(p->vel) || !func_001CF058(p->phys.accel))moving=1;
 if(moving){
  CalculateEmitdvMatrix__FP6EMITDVfP7MATRIX4(&w.shared.matrix,&p->phys);
  char*page=ClockPage();
  qword v=Point(w.shared.matrix,p->vel);
  p->vel=v;
  p->pos=AddScaled(p->pos,v,BroadcastBits(&w.shared.math.first,ClockWord(page)));
 }
}

typedef char layout_cf0a0_0[(sizeof(qword)==16)?1:-1];
typedef char layout_cf0a0_1[(sizeof(U)==16)?1:-1];
typedef char layout_cf0a0_2[(sizeof(void*)==4)?1:-1];
typedef char layout_cf0a0_3[(sizeof(M3)==0x30)?1:-1];
typedef char layout_cf0a0_4[(sizeof(R3)==0x30)?1:-1];
typedef char layout_cf0a0_5[(sizeof(M4)==0x40)?1:-1];
typedef char layout_cf0a0_6[(sizeof(PHYS)==0x30)?1:-1];
typedef char layout_cf0a0_7[(sizeof(MATH)==0x50)?1:-1];
typedef char layout_cf0a0_8[(sizeof(SHARED)==0x50)?1:-1];
typedef char layout_cf0a0_9[(sizeof(WORK)==0xB0)?1:-1];
typedef char layout_cf0a0_10[((unsigned)&((PHYS*)0)->accel==0x0)?1:-1];
typedef char layout_cf0a0_11[((unsigned)&((PHYS*)0)->axis==0x10)?1:-1];
typedef char layout_cf0a0_12[((unsigned)&((PHYS*)0)->rate==0x20)?1:-1];
typedef char layout_cf0a0_13[((unsigned)&((PHYS*)0)->scale==0x24)?1:-1];
typedef char layout_cf0a0_14[((unsigned)&((S*)0)->mat==0x40)?1:-1];
typedef char layout_cf0a0_15[((unsigned)&((S*)0)->pos==0x70)?1:-1];
typedef char layout_cf0a0_16[((unsigned)&((S*)0)->prev==0x80)?1:-1];
typedef char layout_cf0a0_17[((unsigned)&((S*)0)->vel==0x90)?1:-1];
typedef char layout_cf0a0_18[((unsigned)&((S*)0)->phys==0xA0)?1:-1];
typedef char layout_cf0a0_19[((unsigned)&((S*)0)->omega==0xD0)?1:-1];
typedef char layout_cf0a0_20[((unsigned)&((S*)0)->gE0==0xE0)?1:-1];
typedef char layout_cf0a0_21[((unsigned)&((S*)0)->gE4==0xE4)?1:-1];
typedef char layout_cf0a0_22[((unsigned)&((CLOCK*)0)->time==0x0)?1:-1];
typedef char layout_cf0a0_23[((unsigned)&((CLOCK*)0)->dt==0x4)?1:-1];
typedef char layout_cf0a0_24[((unsigned)&((CLOCK*)0)->dtbits==0x4)?1:-1];
typedef char layout_cf0a0_25[((unsigned)&((MATH*)0)->first==0x0)?1:-1];
typedef char layout_cf0a0_26[((unsigned)&((MATH*)0)->second==0x10)?1:-1];
typedef char layout_cf0a0_27[((unsigned)&((MATH*)0)->scratch==0x20)?1:-1];
typedef char layout_cf0a0_28[((unsigned)&((WORK*)0)->shared==0x0)?1:-1];
typedef char layout_cf0a0_29[((unsigned)&((WORK*)0)->rotation==0x50)?1:-1];
typedef char layout_cf0a0_30[((unsigned)&((WORK*)0)->product==0x80)?1:-1];
typedef char layout_cf0a0_31[((unsigned)&((M4*)0)->a==0x0)?1:-1];
typedef char layout_cf0a0_32[((unsigned)&((M4*)0)->b==0x10)?1:-1];
typedef char layout_cf0a0_33[((unsigned)&((M4*)0)->c==0x20)?1:-1];
typedef char layout_cf0a0_34[((unsigned)&((M4*)0)->d==0x30)?1:-1];

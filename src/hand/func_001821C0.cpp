// func_001821C0 (Claude agent, lane G4): laneG3v1 with the outer loop as `do { if (count<=0) goto done; top: ... if (i<count) goto top; } while (0);`.
// The do/while(0) gives the real loop notes (alignment nop, fall-in entry) while the goto back edge keeps loop.c from hoisting the 0x30 stride.
// One pin (epsilon in $f1).

typedef int qword __attribute__((mode(TI)));
typedef union {qword q;float f[4];int i[4];} U;
struct M3 {qword r[3];};
struct M4 {U r[4];};
struct N {qword pos,dir;char pad20[0x50-0x20];float lateral,roll;char pad58[8];N*next;};
struct E {char pad[0x10];N*first;char pad14[0x1C];};
struct P {char pad[0x1C4];void*model;int count;E entries[1];};
extern qword D_002A2DC0;
extern "C" void func_001F3C70(P*);
extern "C" M3 BuildOrthonormalMatrixZ(qword,qword);
extern "C" M3 func_0011B500(qword,float);
extern "C" qword*func_00256D18(qword*,qword);
extern "C" void func_001A5EE8(void*,M4*,float,float);
extern "C" float fabsf(float);
static inline qword Load(const qword*p){return *p;}
static inline qword Cross(qword a,qword b){qword up=a,dir=b;__asm__("vopmula.xyz ACC,%0,%1\n\tvopmsub.xyz %0,%1,%0":"+j"(up):"j"(dir));return up;}
static inline qword AddLateral(qword a,N*n,float f){qword k;U u;__asm__("qmtc2.ni %1,%0":"=j"(k):"r"(f));u.q=k;qword b=Cross(D_002A2DC0,n->dir);__asm__("vadda.xyzw ACC,%0,$vf0\n\tvmaddx.xyzw %0,%1,%2x":"+j"(a):"j"(b),"j"(k));return a;}
static inline M3 Mul(const M3 &a, const M3 &b) return r
{
    qword a0 = a.r[0], a1 = a.r[1], a2 = a.r[2];
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
        : "=j"(r.r[0]) : "j"(a0), "j"(a1), "j"(a2), "j"(b.r[0]));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
        : "=j"(r.r[1]) : "j"(a0), "j"(a1), "j"(a2), "j"(b.r[1]));
    __asm__("vmulax.xyz ACC,%1,%4x\n\tvmadday.xyz ACC,%2,%4y\n\tvmaddz.xyz %0,%3,%4z"
        : "=j"(r.r[2]) : "j"(a0), "j"(a1), "j"(a2), "j"(b.r[2]));
}

extern "C" void func_001821C0(P*p){
 int i,count;M3 m;M4 mm;qword pos;
 func_001F3C70(p);
 if(!p->model)return;
 count=p->count;i=0;
 do {
  if(count<=0) goto done;
 top:
  {
  N*n=((N**)((int)p+i*0x30))[0x1DC/4];
  while(n){M4*pm=&mm;
   pos=Load(&n->pos);
   m=BuildOrthonormalMatrixZ(n->dir,D_002A2DC0);
   if(1.0f<fabsf(n->lateral))pos=AddLateral(pos,n,n->lateral);
   {float roll=n->roll;register float epsilon __asm__("$f1")=0.01f; /* the one pin */if(epsilon<fabsf(roll))m=Mul(m,func_0011B500(D_002A2DC0,roll));}
   func_00256D18(&mm.r[3].q,pos);
   pm->r[0].q=m.r[0];pm->r[1].q=m.r[1];pm->r[2].q=m.r[2];
   mm.r[0].i[3]=mm.r[1].i[3]=mm.r[2].i[3]=0;
   pm->r[3].f[3]=1.0f;
   func_001A5EE8(p->model,pm,1.0f,1.0f);
   n=n->next;
  }
 }

  count=p->count;
  ++i;
  if(i<count) goto top;
 } while(0);
done:;
}
typedef char qword_Size[(sizeof(qword)==16)?1:-1];
typedef char Pointer_Size[(sizeof(void*)==4)?1:-1];
typedef char Int_Size[(sizeof(int)==4)?1:-1];
typedef char Float_Size[(sizeof(float)==4)?1:-1];
typedef char U_Size[(sizeof(U)==16)?1:-1];
typedef char U_Align[(__alignof__(U)==16)?1:-1];
typedef char M3_Size[(sizeof(M3)==0x30)?1:-1];
typedef char M3_Align[(__alignof__(M3)==16)?1:-1];
typedef char M4_Size[(sizeof(M4)==0x40)?1:-1];
typedef char N_Prefix_Size[(sizeof(N)==0x70)?1:-1];
typedef char N_Align[(__alignof__(N)==16)?1:-1];
typedef char E_Size[(sizeof(E)==0x30)?1:-1];
typedef char P_Prefix_Size[(sizeof(P)==0x1fc)?1:-1];
typedef char N_position_Offset[((unsigned)&((N*)0)->pos==0)?1:-1];
typedef char N_direction_Offset[((unsigned)&((N*)0)->dir==0x10)?1:-1];
typedef char N_lateral_Offset[((unsigned)&((N*)0)->lateral==0x50)?1:-1];
typedef char N_roll_Offset[((unsigned)&((N*)0)->roll==0x54)?1:-1];
typedef char N_next_Offset[((unsigned)&((N*)0)->next==0x60)?1:-1];
typedef char E_first_Offset[((unsigned)&((E*)0)->first==0x10)?1:-1];
typedef char P_model_Offset[((unsigned)&((P*)0)->model==0x1c4)?1:-1];
typedef char P_count_Offset[((unsigned)&((P*)0)->count==0x1c8)?1:-1];
typedef char P_entries_Offset[((unsigned)&((P*)0)->entries==0x1cc)?1:-1];
typedef char P_first_Offset[((unsigned)&((P*)0)->entries[0].first==0x1dc)?1:-1];
typedef char M3_row0_Offset[((unsigned)&((M3*)0)->r[0]==0)?1:-1];
typedef char M3_row1_Offset[((unsigned)&((M3*)0)->r[1]==0x10)?1:-1];
typedef char M3_row2_Offset[((unsigned)&((M3*)0)->r[2]==0x20)?1:-1];
typedef char M4_row3_Offset[((unsigned)&((M4*)0)->r[3]==0x30)?1:-1];
typedef char U_float_View[((unsigned)&((U*)0)->f[0]==0)?1:-1];
typedef char U_int_View[((unsigned)&((U*)0)->i[0]==0)?1:-1];

extern "C" {void*func_001291A8(void*,const void*);void func_00129EC8(void*);}
struct CRefData{int tag;union{int n;float g;};};
struct CRef:CRefData{
 CRef(float x){tag=0x16;g=x;}
 CRef(const CRef&r){func_001291A8(this,&r);}
 ~CRef(){func_00129EC8(this);}
};
struct OPT;
struct H{char pad[0x9C];float start,end,elapsed;};
struct G{char pad[0x3B0C];H*object;char pad10[0xC];OPT*primary;char pad20[0x38];OPT*options[9];};
struct CLOCK{float time,dt;};
extern G D_003DD490;
extern CLOCK D_002DDED0;
extern int D_002E0C20;
extern char D_003D4A60[],D_002DE578[];
extern "C" CRef func_002581C0(OPT*,void*);
extern "C" CRef func_00258200(OPT*,const CRefData&,void*);
extern "C" int NWorldCur();
extern "C" void func_00154D88(void*,float);
static inline float GetFloat(OPT*p,void*context){const CRef&r=func_002581C0(p,context);return r.g;}
static inline void SetPv(OPT*p,const CRef&r,void*context){CRef arg(r);const CRef&ret=func_00258200(p,arg,context);}
static inline void SetFloat(OPT*p,float x,void*context){CRef r(x);SetPv(p,r,context);}
struct CRefCopy:CRefData{
 CRefCopy(const CRefData&r){func_001291A8(this,&r);}
 ~CRefCopy(){func_00129EC8(this);}
};
static inline void SetPrimaryFloat(OPT*p,float value,void*context){
 CRef r(value);CRefCopy arg(r);const CRef&ret=func_00258200(p,arg,context);
}
extern "C" void UpdateGameTimers(){
 if(!D_003DD490.primary)return;
 float x=GetFloat(D_003DD490.primary,D_003D4A60);
 SetPrimaryFloat(D_003DD490.primary,x+D_002DDED0.dt,D_003D4A60);
 unsigned int i=NWorldCur();
 if(i<9){
  OPT*p=D_003DD490.options[i];
  if(p){float g=GetFloat(p,D_003D4A60);SetFloat(p,g+D_002DDED0.dt,D_003D4A60);}
 }
 H*p=D_003DD490.object;
 if(p&&(float)D_002E0C20<10.0f){
  p->elapsed+=D_002DDED0.dt;
  if(p->start<p->elapsed)func_00154D88(D_002DE578,D_002DDED0.dt/(p->end-p->start));
 }
}

#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef char Pointer_Size[sizeof(void*)==4?1:-1];
typedef char Integer_Size[sizeof(int)==4?1:-1];
typedef char Float_Size[sizeof(float)==4?1:-1];
typedef char CRefData_Size[sizeof(CRefData)==8?1:-1];
typedef char CRef_Size[sizeof(CRef)==8?1:-1];
typedef char CRefCopy_Size[sizeof(CRefCopy)==8?1:-1];
typedef char CRefData_Alignment[__alignof__(CRefData)==4?1:-1];
typedef char CRef_Alignment[__alignof__(CRef)==4?1:-1];
typedef char CRefCopy_Alignment[__alignof__(CRefCopy)==4?1:-1];
typedef char CRefData_Tag[FIELD_OFFSET(CRefData,tag)==0?1:-1];
typedef char CRefData_Float[FIELD_OFFSET(CRefData,g)==4?1:-1];
typedef char CRef_Tag[FIELD_OFFSET(CRef,tag)==0?1:-1];
typedef char CRef_Float[FIELD_OFFSET(CRef,g)==4?1:-1];
typedef char CRefCopy_Tag[FIELD_OFFSET(CRefCopy,tag)==0?1:-1];
typedef char CRefCopy_Float[FIELD_OFFSET(CRefCopy,g)==4?1:-1];
typedef char H_Size[sizeof(H)==0xA8?1:-1];
typedef char H_Start[FIELD_OFFSET(H,start)==0x9C?1:-1];
typedef char H_End[FIELD_OFFSET(H,end)==0xA0?1:-1];
typedef char H_Elapsed[FIELD_OFFSET(H,elapsed)==0xA4?1:-1];
typedef char G_Size[sizeof(G)==0x3B7C?1:-1];
typedef char G_Object[FIELD_OFFSET(G,object)==0x3B0C?1:-1];
typedef char G_Primary[FIELD_OFFSET(G,primary)==0x3B1C?1:-1];
typedef char G_Options[FIELD_OFFSET(G,options)==0x3B58?1:-1];
typedef char G_OptionCount[sizeof(((G*)0)->options)/sizeof(OPT*)==9?1:-1];
typedef char Clock_Size[sizeof(CLOCK)==8?1:-1];
typedef char Clock_Time[FIELD_OFFSET(CLOCK,time)==0?1:-1];
typedef char Clock_Delta[FIELD_OFFSET(CLOCK,dt)==4?1:-1];

extern "C" {void *func_001291A8(void*,const void*);void func_00129EC8(void*);}
struct CRef {
 int tag;
 union {int n;float g;};
 CRef(int x){tag=0x15;n=x;}
 CRef(bool x){tag=0x17;n=x;}
 CRef(const CRef&r){func_001291A8(this,&r);}
 ~CRef(){func_00129EC8(this);}
};
struct OPT{int kind,value;};
struct S{char pad[0x54];int state;char pad58[0xC];int a,b;char pad6C[0x10];void*next;char pad80[0x48];OPT c,d,e,f,h;int fF0,fF4;};
extern char D_003D4A60[];
extern "C" CRef func_00258200(OPT*,const CRef&,void*);
extern "C" int FTaskComplete(S*);
extern "C" void UpdateJobSaveVars(void*);
static inline void SetPv(OPT*p,const CRef&r,void*context=0){CRef arg(r);const CRef&ret=func_00258200(p,arg,D_003D4A60);}
static inline void SetInt(OPT*p,int n){CRef r(n);SetPv(p,r);}
extern "C" void UpdateTaskSaveVars(S*p){
 if(p->fF4==1||p->state==-1)return;
 {OPT*opt=&p->c;if(opt->kind){SetInt(opt,p->a);}}
 {OPT*opt=&p->d;if(opt->kind){SetInt(opt,p->b);}}
 {OPT*opt=&p->e;if(opt->kind){void*key=D_003D4A60;int f=FTaskComplete(p);{CRef r(f!=0);SetPv(opt,r,key);}}}
 {OPT*opt=&p->f;if(opt->kind){SetInt(opt,p->state);}}
 {OPT*opt=&p->h;if(opt->kind){CRef r(false);SetPv(opt,r,D_003D4A60);}}
 if(p->next)UpdateJobSaveVars(p->next);
}

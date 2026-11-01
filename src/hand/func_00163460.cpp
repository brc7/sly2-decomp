extern "C" {void*func_001291A8(void*,const void*);void func_00129EC8(void*);}
struct CRef{
 int m_tagk;
 union{int m_tag;float m_g;};
 CRef(int n){m_tagk=0x15;m_tag=n;}
 CRef(bool f){m_tagk=0x17;m_tag=f;}
 CRef(float g){m_tagk=0x16;m_g=g;}
 CRef(const CRef&r){func_001291A8(this,&r);}
 ~CRef(){func_00129EC8(this);}
};
struct OPT{int kind,value;};
struct S{char pad[0x7C];int n;char pad80[0x24];float g;char padA8[0x28];OPT a,b,c;};
extern char D_003D4A60[];
extern "C" CRef func_00258200(OPT*,const CRef&,void*);
extern "C" int func_00163400(S*);
static inline void SetPv(OPT*p,const CRef&r,void*context=0){CRef arg(r);const CRef&ret=func_00258200(p,arg,D_003D4A60);}
static inline void SetInt(OPT*p,int n){CRef r(n);SetPv(p,r);}
static inline void SetBool(OPT*p,bool b,void*context=0){CRef r(b);SetPv(p,r,context);}
/** @sly2-annot addr=0x163460
 * Purpose: Writes job count (+0x7C), complete flag and elapsed time (+0xA4) into its three save-var refs
 *   (+0xD0/+0xD8/+0xE0)
 * Info: proposed name UpdateJobSaveVars (T1); TU game
 * Notes: runtime6 G13: JOB save-var refs +0xD0..+0xE0
 * Confidence: mid (draft) [T1 0.55] - evidence: runtime (save-state RAM); sources: r3b_rest+runtime
 * See: docs/engine/game-progression.md#1-job-and-task-gamecpp-0x1613a8-0x166270,
 *   docs/engine/game-progression.md#8-key-functions,
 *   docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
extern "C" void func_00163460(S*p){
 {OPT*opt=&p->a;if(opt->kind){SetInt(opt,p->n);}}
 {OPT*opt=&p->b;if(opt->kind){void*key=D_003D4A60;int f=func_00163400(p);{SetBool(opt,f!=0,key);}}}
 {OPT*opt=&p->c;if(opt->kind){CRef r(p->g);SetPv(opt,r);}}
}

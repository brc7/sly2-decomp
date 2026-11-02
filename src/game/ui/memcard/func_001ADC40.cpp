/* (Claude agent, 2026-10-17) frame pointer copy pf used only for the push call reproduces the a0->s5 copy. */
struct REF{int tag;union{int n;float g;};};
struct G{char pad[0x3A18];char object[0x104];void*primary;int pad20[2];void*fallback;};
struct B{char data[0x5464];};
extern "C" {
 extern G D_003DD490;
 extern B D_002E6604[];
 extern char D_003D9ED0[],D_002A6EE0[],D_002A6EC8[];
 void*RefGetGlobalSaveVar(REF*,void*,void*);
 void Decref__4CRef(REF*);
 void*STACKSCOPE_ctor(void*);
 void STACKSCOPE_ctor_1AE4A0(void*,int);
 void*PkeyAllocSaveKey(void*,...);
 int IdsFromWorld(int);
 void*PchzFindSwLangstr__FP2SWi(void*,int);
 int NCompletionPercentGs(void*,void*);
 int sprintf(char*,const char*,...);
 char*strcpy(char*,const char*);
 int IdPauseTopPage(void*);
}
struct FRAME{FRAME()__asm__("STACKSCOPE_ctor");~FRAME(){STACKSCOPE_ctor_1AE4A0(this,2);}};
static inline float GetG(REF&r,void*a,B*b){RefGetGlobalSaveVar(&r,a,b);float g=r.g;Decref__4CRef(&r);return g;}
static inline int GetN(REF&r,void*a,B*b){RefGetGlobalSaveVar(&r,a,b);int n=r.n;Decref__4CRef(&r);return n;}
/** @sly2-annot addr=0x1ADC40
 * Purpose: Per-slot text (0x5CF..0x5D1, arg=slot): episode best key + completion %
 * Info: proposed name TextSaveSlotSummary (T2); TU memcard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/save-memcard.md#memcard-0x1aa978-0x1ae150
 * @end-sly2-annot */
extern "C" void func_001ADC40(char*buf,int idx){
 REF ref;
 G*pg=&D_003DD490;
 B*pb=&D_002E6604[idx];
 if(GetG(ref,pg->primary,pb)!=0.0f){
  int frame[4];int*pf=frame;STACKSCOPE_ctor(pf);
  void*value=PkeyAllocSaveKey(D_002A6EE0,0);
  int tag;
  if(value)tag=GetN(ref,value,pb);else tag=GetN(ref,pg->fallback,pb);
  sprintf(buf,D_002A6EC8,(char*)PchzFindSwLangstr__FP2SWi(&D_003DD490,IdsFromWorld(tag)),NCompletionPercentGs(D_003DD490.object,pb));
  STACKSCOPE_ctor_1AE4A0(frame,2);
 }else{
  switch(IdPauseTopPage(D_003D9ED0)){
   case 0x1E:case 0x1F:strcpy(buf,(char*)PchzFindSwLangstr__FP2SWi(pg,0x44));break;
   case 0x19:strcpy(buf,(char*)PchzFindSwLangstr__FP2SWi(pg,0x45));break;
  }
 }
}

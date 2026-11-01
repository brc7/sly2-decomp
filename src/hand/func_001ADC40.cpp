/* (Claude agent, 2026-10-17) frame pointer copy pf used only for the push call reproduces the a0->s5 copy. */
struct REF{int tag;union{int n;float g;};};
struct G{char pad[0x3A18];char object[0x104];void*primary;int pad20[2];void*fallback;};
struct B{char data[0x5464];};
extern "C" {
 extern G D_003DD490;
 extern B D_002E6604[];
 extern char D_003D9ED0[],D_002A6EE0[],D_002A6EC8[];
 void*func_002581C0(REF*,void*,void*);
 void func_00129EC8(REF*);
 void*func_001AE478(void*);
 void func_001AE4A0(void*,int);
 void*func_001D89A8(void*,...);
 int func_001DDD40(int);
 void*PchzFindSwLangstr__FP2SWi(void*,int);
 int func_001D82B8(void*,void*);
 int func_0021ECD8(char*,const char*,...);
 char*func_0021ED58(char*,const char*);
 int func_001B0078(void*);
}
struct FRAME{FRAME()__asm__("func_001AE478");~FRAME(){func_001AE4A0(this,2);}};
static inline float GetG(REF&r,void*a,B*b){func_002581C0(&r,a,b);float g=r.g;func_00129EC8(&r);return g;}
static inline int GetN(REF&r,void*a,B*b){func_002581C0(&r,a,b);int n=r.n;func_00129EC8(&r);return n;}
extern "C" void func_001ADC40(char*buf,int idx){
 REF ref;
 G*pg=&D_003DD490;
 B*pb=&D_002E6604[idx];
 if(GetG(ref,pg->primary,pb)!=0.0f){
  int frame[4];int*pf=frame;func_001AE478(pf);
  void*value=func_001D89A8(D_002A6EE0,0);
  int tag;
  if(value)tag=GetN(ref,value,pb);else tag=GetN(ref,pg->fallback,pb);
  func_0021ECD8(buf,D_002A6EC8,(char*)PchzFindSwLangstr__FP2SWi(&D_003DD490,func_001DDD40(tag)),func_001D82B8(D_003DD490.object,pb));
  func_001AE4A0(frame,2);
 }else{
  switch(func_001B0078(D_003D9ED0)){
   case 0x1E:case 0x1F:func_0021ED58(buf,(char*)PchzFindSwLangstr__FP2SWi(pg,0x44));break;
   case 0x19:func_0021ED58(buf,(char*)PchzFindSwLangstr__FP2SWi(pg,0x45));break;
  }
 }
}

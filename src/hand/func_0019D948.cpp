struct FRAME;
extern "C" FRAME*func_001AE478(FRAME*);
extern "C" void func_001AE4A0(FRAME*,int);
struct FRAME {FRAME(){(void)func_001AE478(this);}~FRAME(){func_001AE4A0(this,2);}};
struct X {char pad0[0x5C];int x5C;char pad1[0x6C-0x60];int x6C;char pad2[0x7C-0x70];X*x7C;};
struct T {char pad[0xCC];int tags[2];};
struct P {int a,b;T*t;};
struct G {char pad[0x3B0C];X*x;};
extern "C" G D_003DD490;
extern "C" void SortTasksTopologically(int*,X***);
extern "C" void func_002338C0(void*,unsigned,unsigned,int(*)(const void*,const void*));
extern "C" int func_00165C28(const void*,const void*);
extern "C" int FTaskComplete(X*);
struct LOCAL_LAYOUT{FRAME frame;int count;X**array;};
static inline G*CopyGame(G*source){register G*input __asm__("$16")=source;register G*result __asm__("$20");__asm__("daddu %0,%1,$zero":"=r"(result):"r"(input));return result;}
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char Offset_0[FIELD_OFFSET(X,x5C)==92?1:-1];
typedef char Offset_1[FIELD_OFFSET(X,x6C)==108?1:-1];
typedef char Offset_2[FIELD_OFFSET(X,x7C)==124?1:-1];
typedef char Offset_3[FIELD_OFFSET(T,tags)==204?1:-1];
typedef char Offset_4[FIELD_OFFSET(P,t)==8?1:-1];
typedef char Offset_5[FIELD_OFFSET(G,x)==15116?1:-1];
typedef char Offset_6[FIELD_OFFSET(LOCAL_LAYOUT,frame)==0?1:-1];
typedef char Offset_7[FIELD_OFFSET(LOCAL_LAYOUT,count)==4?1:-1];
typedef char Offset_8[FIELD_OFFSET(LOCAL_LAYOUT,array)==8?1:-1];
typedef char Size_0[sizeof(FRAME)==1?1:-1];
typedef char Size_1[sizeof(LOCAL_LAYOUT)==12?1:-1];
typedef char Size_2[sizeof(X)==128?1:-1];
typedef char Size_3[sizeof(T)==212?1:-1];
typedef char Size_4[sizeof(P)==12?1:-1];
typedef char Size_5[sizeof(G)==15120?1:-1];
typedef char Size_6[sizeof(int)==4?1:-1];
typedef char Size_7[sizeof(unsigned)==4?1:-1];
typedef char Size_8[sizeof(void*)==4?1:-1];
typedef char Alignment_0[__alignof__(FRAME)==1?1:-1];
typedef char Alignment_1[__alignof__(LOCAL_LAYOUT)==4?1:-1];
typedef char Alignment_2[__alignof__(X)==4?1:-1];
typedef char Alignment_3[__alignof__(T)==4?1:-1];
typedef char Alignment_4[__alignof__(P)==4?1:-1];
typedef char Alignment_5[__alignof__(G)==4?1:-1];
/** @sly2-annot addr=0x19D948
 * Purpose: Collect and qsort a list of world tag ids (g_sw game record x6C...) for the HUD
 * Info: proposed name GatherJtModeHudTags (T2); TU mode
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#mode-0x19cbf8-0x19de10
 * @end-sly2-annot */
extern "C" void func_0019D948(P*p){
 T*pt=p->t;
 if(!pt)return;
 pt->tags[0]=pt->tags[1]=-1;
 G*pg=&D_003DD490;
 if(!pg->x || pg->x->x6C==-1)return;
 p->t->tags[0]=pg->x->x6C;
 int n=1,i=0;
 G*pgLoop=CopyGame(pg);
 FRAME fr;
 int c;
 X**ap;
 SortTasksTopologically(&c,&ap);
 func_002338C0(ap,c,4,func_00165C28);
 for(;i<c;i++){
  X*x=ap[i];
  if(pgLoop->x==x->x7C && x->x5C!=-1 && !FTaskComplete(x)){
   p->t->tags[n++]=x->x5C;
   break;
  }
 }
}

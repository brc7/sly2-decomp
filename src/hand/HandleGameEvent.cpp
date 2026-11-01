#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
struct P{
 char pad0[0x10];int selections[3];char pad1[0x64-0x1C];P*child;
 char pad2[0xF4-0x68];int x,y;char pad3[0x11A8-0xFC];void*widget;
};
struct CONTEXT{char pad[0xF0];int x,y;};
struct CHECK{int words[16];};
struct G{
 char pad0[0x3B0C];P*first;P*second;P*third;int pad18;void*primary;
 char pad1[0x3BF4-0x3B20];int flags;
 char pad2[0x3C88-0x3BF8];int a,b;
 char pad3[0x3E38-0x3C90];int ready;
};
struct BANK{
 char pad0[8];int mode;char pad1[0xFD50-12];int selected;void*current;
};
struct LEASE;
struct CRef;
extern "C"{
 extern G D_003DD490;
 extern BANK D_002E65E0;
 extern CHECK D_003E1088,D_002A37E8;
 extern char D_003D4A60[];
 int CploFindSwObjectsByClass(G*,int,int,P*,int,P**);
 int func_0013C478(void*);
 void*func_00143AF0(P*,int);
 void InitGame(P*);
 void func_00166008();
 void func_001AB630(BANK*);
 int func_00210AE0();
 void func_001D8FF8(void*);
 void RestoreTasksAfterLoad(P*);
 LEASE*func_001AE478(LEASE*);
 void func_001AE4A0(LEASE*,int);
 void SortTasksTopologically(int*,P***);
 void RestoreTaskState(P*);
 void UpdateTaskSaveVars(P*);
 P*PloFindSwObject__FP2SWi3OIDP2LO(G*,int,int,P*);
 void SetSwJobCur(G*,P*);
 void SetSwTaskCur(G*,P*);
 void SetSwTaskPending(G*,P*);
 void BeginJob(P*);
 void BeginTask(P*);
 void AnnounceTaskJob(P*);
 void func_0022B490(G*,CHECK*,int,int,int,int);
 CRef*func_001291A8(CRef*,const CRef*);
 void func_00129EC8(CRef*);
}
struct LEASE{LEASE(){func_001AE478(this);}void Release(){func_001AE4A0(this,2);}};
struct OVERRIDE{CONTEXT*context;P*object;int oldX,oldY;
 OVERRIDE(P*p,int x,int y):context(0),object(p){if(object){oldX=object->x;object->x=x;oldY=object->y;object->y=y;}}
 ~OVERRIDE(){if(context){context->x=oldX;context->y=oldY;}if(object){object->x=oldX;object->y=oldY;}}
};
struct CRef{int kind;union{int n;float g;};CRef(const CRef&r){func_001291A8(this,&r);}~CRef(){func_00129EC8(this);}};
extern "C" CRef func_002581C0(void*,void*);
static inline float GetFloat(void*p,void*context){const CRef&r=func_002581C0(p,context);return r.g;}
static inline int Same(CHECK*a,CHECK*b){
 if(a==b)return 1;
 if(!a||!b)return 0;
 return (a->words[2]^a->words[14])==(b->words[2]^b->words[14]);
}
typedef char CHECK_Size[sizeof(CHECK)==64?1:-1];
typedef char OVERRIDE_Size[sizeof(OVERRIDE)==16?1:-1];
typedef char CRef_Size[sizeof(CRef)==8?1:-1];
typedef char LEASE_Size[sizeof(LEASE)==1?1:-1];
typedef char P_selections_Offset[FIELD_OFFSET(P,selections)==16?1:-1];
typedef char P_child_Offset[FIELD_OFFSET(P,child)==100?1:-1];
typedef char P_x_Offset[FIELD_OFFSET(P,x)==244?1:-1];
typedef char P_y_Offset[FIELD_OFFSET(P,y)==248?1:-1];
typedef char P_widget_Offset[FIELD_OFFSET(P,widget)==4520?1:-1];
typedef char CONTEXT_x_Offset[FIELD_OFFSET(CONTEXT,x)==240?1:-1];
typedef char CONTEXT_y_Offset[FIELD_OFFSET(CONTEXT,y)==244?1:-1];
typedef char CHECK_words_Offset[FIELD_OFFSET(CHECK,words)==0?1:-1];
typedef char G_first_Offset[FIELD_OFFSET(G,first)==15116?1:-1];
typedef char G_second_Offset[FIELD_OFFSET(G,second)==15120?1:-1];
typedef char G_third_Offset[FIELD_OFFSET(G,third)==15124?1:-1];
typedef char G_primary_Offset[FIELD_OFFSET(G,primary)==15132?1:-1];
typedef char G_flags_Offset[FIELD_OFFSET(G,flags)==15348?1:-1];
typedef char G_a_Offset[FIELD_OFFSET(G,a)==15496?1:-1];
typedef char G_b_Offset[FIELD_OFFSET(G,b)==15500?1:-1];
typedef char G_ready_Offset[FIELD_OFFSET(G,ready)==15928?1:-1];
typedef char BANK_mode_Offset[FIELD_OFFSET(BANK,mode)==8?1:-1];
typedef char BANK_selected_Offset[FIELD_OFFSET(BANK,selected)==64848?1:-1];
typedef char BANK_current_Offset[FIELD_OFFSET(BANK,current)==64852?1:-1];
typedef char OVERRIDE_context_Offset[FIELD_OFFSET(OVERRIDE,context)==0?1:-1];
typedef char OVERRIDE_object_Offset[FIELD_OFFSET(OVERRIDE,object)==4?1:-1];
typedef char OVERRIDE_oldX_Offset[FIELD_OFFSET(OVERRIDE,oldX)==8?1:-1];
typedef char OVERRIDE_oldY_Offset[FIELD_OFFSET(OVERRIDE,oldY)==12?1:-1];
typedef char CRef_kind_Offset[FIELD_OFFSET(CRef,kind)==0?1:-1];
typedef char CRef_g_Offset[FIELD_OFFSET(CRef,g)==4?1:-1];
extern "C" void HandleGameEvent(P*p,int action){
 switch(action){
 case 1:{
  if(Same(&D_003E1088,&D_002A37E8)){
   P*results[3];
   int found=CploFindSwObjectsByClass(&D_003DD490,5,0x34,0,3,results);
   if(found>0){P**cursor=results;int count=found;do{P*object=*cursor;if(object->widget)(void)func_00143AF0(object,func_0013C478(object->widget));--count;++cursor;}while(count);}
  }
  break;
 }
 case 3:{
  if(D_003DD490.flags&1){InitGame(p);D_002E65E0.selected=0;}
  else if(D_003DD490.flags&0x40)func_00166008();
  func_001AB630(&D_002E65E0);
  if(func_00210AE0())break;
  if(D_002E65E0.mode!=6||D_002E65E0.selected||!D_002E65E0.current)break;
  if(GetFloat(D_003DD490.primary,D_003D4A60)!=0.0f)break;
  if(GetFloat(D_003DD490.primary,D_002E65E0.current)!=0.0f)func_001D8FF8(D_002E65E0.current);
  break;
 }
 case 4:RestoreTasksAfterLoad(p);break;
 case 5:{
  int i=0;
  LEASE mark;
  int count;
  P**objects;
  SortTasksTopologically(&count,&objects);
  for(;i<count;i++){P*object=objects[i];OVERRIDE temp(object,1,0);RestoreTaskState(object);}
  G*g=&D_003DD490;
  SetSwJobCur(g,PloFindSwObject__FP2SWi3OIDP2LO(g,0x105,p->selections[0],0));
  SetSwTaskCur(g,PloFindSwObject__FP2SWi3OIDP2LO(g,0x105,p->selections[1],0));
  SetSwTaskPending(g,PloFindSwObject__FP2SWi3OIDP2LO(g,0x105,p->selections[2],0));
  if(g->first)BeginJob(g->first);
  if(g->second)BeginTask(g->second);
  if(g->flags&4){if(g->third)AnnounceTaskJob(g->third);else if(g->first)AnnounceTaskJob(g->first->child);}
  for(int j=0;j<count;j++){P*object=objects[j];OVERRIDE temp(object,0,0);UpdateTaskSaveVars(object);}
  mark.Release();
  break;
 }
 case 16:{G*g=&D_003DD490;if(!g->first)func_0022B490(g,0,g->a,g->b,g->ready?0x20:0,0x30A);break;}
 case 2:case 6:case 7:case 8:case 9:case 10:case 11:case 12:case 13:case 14:case 15:break;
 }
}

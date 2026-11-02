#define FIELD_OFFSET(t,m) ((uint)&(((t*)0)->m))
typedef unsigned int uint;
struct P;struct ITEM;struct INPUT;struct POOL;struct OPT;
struct VTE{short delta,pad;void(*fn)(POOL*);};
struct POOL{char pad[0xC];VTE*vt;};
struct ITEM{int kind;void(*select)(ITEM*);char pad8[8];void(*update)(ITEM*);char pad14[8];uint flags;};
struct META{char pad[0xC];int ready,pad10,back,forward,reverse;};
struct GROUP{int count;ITEM*items;POOL*pool;int back;int pad10,index;META*meta;};
struct P{char pad[0x10];INPUT*input;char pad14[0x10];int depth;char pad28[0x1C0];int state,pending;};
struct CONTEXT{char bytes[0x5464];};
struct BANK{char pad[0x24];CONTEXT contexts[3];char padFD50[8];CONTEXT*current;};
struct MENU;
extern "C" void*func_001291A8(void*,const void*);
extern "C" void func_00129EC8(void*);
struct CRef{int tag;union{int n;float f;};CRef(const CRef&r){func_001291A8(this,&r);}~CRef(){func_00129EC8(this);}};
extern "C"{
void func_001D9870();
int FWidgetAnimPlaying(POOL*,int);
void SetPauseMenuState(P*,int);
int DySelectionJoy(INPUT*);
uint GrfHandleJoyBtn(INPUT*,uint);
void func_001AF490(P*);
void func_001AF608(P*);
void func_001AF660(P*);
void func_001AFF88(P*,ITEM*);
void func_001AFB88(P*,ITEM*);
void func_001AF6D0(P*,GROUP*,ITEM*);
int IdPauseTopPage(P*);
CRef RefGetGlobalSaveVar(OPT*,CONTEXT*);
void SetMcDialogState(MENU*,int);
extern OPT*D_003E0FAC;
extern CONTEXT D_002E6604[];
extern MENU D_002E6550;
}
static inline float Value(OPT*key,CONTEXT*context){CRef result=RefGetGlobalSaveVar(key,context);return result.f;}
static inline void Select(CONTEXT*contexts,CONTEXT*context){BANK*bank=(BANK*)((char*)contexts-0x24);bank->current=context;}
typedef char VTE_Size[sizeof(VTE)==8?1:-1];
typedef char ITEM_Size[sizeof(ITEM)==32?1:-1];
typedef char META_Size[sizeof(META)==32?1:-1];
typedef char GROUP_Size[sizeof(GROUP)==28?1:-1];
typedef char CONTEXT_Size[sizeof(CONTEXT)==21604?1:-1];
typedef char CRef_Size[sizeof(CRef)==8?1:-1];
typedef char POOL_vt_Offset[FIELD_OFFSET(POOL,vt)==12?1:-1];
typedef char ITEM_kind_Offset[FIELD_OFFSET(ITEM,kind)==0?1:-1];
typedef char ITEM_select_Offset[FIELD_OFFSET(ITEM,select)==4?1:-1];
typedef char ITEM_update_Offset[FIELD_OFFSET(ITEM,update)==16?1:-1];
typedef char ITEM_flags_Offset[FIELD_OFFSET(ITEM,flags)==28?1:-1];
typedef char META_ready_Offset[FIELD_OFFSET(META,ready)==12?1:-1];
typedef char META_back_Offset[FIELD_OFFSET(META,back)==20?1:-1];
typedef char META_forward_Offset[FIELD_OFFSET(META,forward)==24?1:-1];
typedef char META_reverse_Offset[FIELD_OFFSET(META,reverse)==28?1:-1];
typedef char GROUP_count_Offset[FIELD_OFFSET(GROUP,count)==0?1:-1];
typedef char GROUP_items_Offset[FIELD_OFFSET(GROUP,items)==4?1:-1];
typedef char GROUP_pool_Offset[FIELD_OFFSET(GROUP,pool)==8?1:-1];
typedef char GROUP_back_Offset[FIELD_OFFSET(GROUP,back)==12?1:-1];
typedef char GROUP_index_Offset[FIELD_OFFSET(GROUP,index)==20?1:-1];
typedef char GROUP_meta_Offset[FIELD_OFFSET(GROUP,meta)==24?1:-1];
typedef char P_input_Offset[FIELD_OFFSET(P,input)==16?1:-1];
typedef char P_depth_Offset[FIELD_OFFSET(P,depth)==36?1:-1];
typedef char P_state_Offset[FIELD_OFFSET(P,state)==488?1:-1];
typedef char P_pending_Offset[FIELD_OFFSET(P,pending)==492?1:-1];
typedef char BANK_contexts_Offset[FIELD_OFFSET(BANK,contexts)==36?1:-1];
typedef char BANK_current_Offset[FIELD_OFFSET(BANK,current)==64856?1:-1];
typedef char CRef_tag_Offset[FIELD_OFFSET(CRef,tag)==0?1:-1];
typedef char CRef_f_Offset[FIELD_OFFSET(CRef,f)==4?1:-1];
/** @sly2-annot addr=0x1AEEE0
 * Purpose: Screen vt7: per-frame menu input (DySelectionJoy, GrfHandleJoyBtn), item activation callbacks,
 *   volume/video item handling, save-slot pages
 * Info: proposed name UpdatePauseMenu (T1); TU menu
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
extern "C" void UpdatePauseMenu(P*p){
 int direction;
 func_001D9870();
 if(p->depth<=0)return;
 GROUP*group=(GROUP*)((char*)p+0xC+p->depth*sizeof(GROUP));
 int i=0;if(group->count>0){
 next:{ITEM*item=&group->items[i];if(item->update)item->update(item);}
  i++;if(i<group->count)goto next;}
 switch(p->state){
 case 0:break;
 case 1:
  if(FWidgetAnimPlaying(group->pool,0x314))break;
  if(p->pending!=-1){p->depth=p->pending;p->pending=-1;}
  goto resumed;
 case 2:
  if(FWidgetAnimPlaying(group->pool,0x314))break;
  if(p->pending!=-1)p->pending=-1;else p->depth--;
  if(p->depth==0)goto close;
  goto resumed;
 case 3:
  if(FWidgetAnimPlaying(group->pool,group->meta->back))break;
  p->depth=p->pending;p->pending=-1;
  SetPauseMenuState(p,4);
  break;
 case 4:
  if(FWidgetAnimPlaying(group->pool,group->meta->ready))break;
  goto resumed;
 case 8:{
  if(FWidgetAnimPlaying(group->pool,0x314))break;
 close:
  POOL*pool=group->pool;VTE*method=&pool->vt[50];method->fn((POOL*)((char*)pool+method->delta));
  SetPauseMenuState(p,0);
  break;
 }
 case 5:
  if(FWidgetAnimPlaying(group->pool,group->meta->forward))break;
 activate:
  SetPauseMenuState(p,7);
  if(!p->input)break;
  {INPUT*input=p->input;direction=DySelectionJoy(input);if(direction<0)goto up;goto positive;}
 case 6:
  if(FWidgetAnimPlaying(group->pool,group->meta->reverse))break;goto activate;
 case 7:{
  if(!p->input)break;
  uint flags;flags=GrfHandleJoyBtn(p->input,0x870);
  if(flags&0x800){func_001AF660(p);break;}
  if((flags&0x10)||(group->count==0&&flags)){if(group->back)func_001AF490(p);else func_001AF608(p);break;}
  if(p->depth>0&&(flags&0x40)){
   ITEM*item;item=&group->items[group->index];
   if(item->select)item->select(item);
   if(item->flags&2)break;
   if(item->flags&1)goto back;func_001AF660(p);break;
 back:
   if(group->back)func_001AF490(p);else func_001AF608(p);
   break;
  }
  if(flags&0x20){
   int kind=IdPauseTopPage(p);
   if(kind!=0x19&&kind!=0x1E)break;
   CONTEXT*contexts=D_002E6604;CONTEXT*context=contexts+group->index;
   if(Value(D_003E0FAC,context)==0.0f)break;
   Select(contexts,context);
   SetMcDialogState(&D_002E6550,0x20);
   break;
  }
  if(!group)break;
 {
   direction=DySelectionJoy(p->input);
   if(direction>=0)goto positive;
 up:SetPauseMenuState(p,6);break;
 positive:if(direction<=0)break;SetPauseMenuState(p,5);break;
  }
 }
 case 11:
  if(FWidgetAnimPlaying(group->pool,0x331))break;
  SetPauseMenuState(p,10);break;
 case 9:
  if(FWidgetAnimPlaying(group->pool,0x332))break;
 resumed:
  SetPauseMenuState(p,7);break;
 case 10:{
  if(!p->input)break;
  ITEM*item;item=&group->items[group->index];
  uint flags;flags=GrfHandleJoyBtn(p->input,0x850);
  if(flags&0x800){func_001AFF88(p,item);
func_001AF660(p);break;}
  if(flags&0x10)func_001AFF88(p,item);
  else if(flags&0x40)func_001AFB88(p,item);
  else func_001AF6D0(p,group,item);
  break;
 }
 default:break;
 }
}

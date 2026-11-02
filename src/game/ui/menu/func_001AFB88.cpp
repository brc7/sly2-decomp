#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
struct CRef;
extern "C" CRef*func_001291A8(CRef*,const CRef*);
extern "C" void func_00129EC8(CRef*);
struct CRef{
 int tag;
 union{int n;float f;};
 CRef(int x){tag=0x15;n=x;}
 CRef(bool x){tag=0x17;n=x;}
 CRef(float x){tag=0x16;f=x;}
 CRef(const CRef&r){func_001291A8(this,&r);}
 ~CRef(){func_00129EC8(this);}
};
struct OPT;
struct MSG{int kind;};
struct GAME{char pad[0x3B40];OPT*first,*second;};
extern GAME D_003DD490;
extern OPT*D_003E0FC0,*D_003E0FC4,*D_003E0FC8,*D_003E0FCC,*D_003E0FD8,*D_003E0FDC,*D_003E0FE0;
extern float D_003DD190,D_003DD18C,D_002F6468,D_002F646C;
extern int D_002C2D8C;
extern char D_003D4A60[];
typedef char CRefSize[sizeof(CRef)==8?1:-1];
typedef char CRefTag[FIELD_OFFSET(CRef,tag)==0?1:-1];
typedef char CRefInt[FIELD_OFFSET(CRef,n)==4?1:-1];
typedef char CRefFloat[FIELD_OFFSET(CRef,f)==4?1:-1];
typedef char MessageKind[FIELD_OFFSET(MSG,kind)==0?1:-1];
typedef char GameFirst[FIELD_OFFSET(GAME,first)==0x3B40?1:-1];
typedef char GameSecond[FIELD_OFFSET(GAME,second)==0x3B44?1:-1];
extern "C" CRef RefSetGlobalSaveVar(OPT*,const CRef&,void*);
extern "C" void SetPauseMenuState(void*,int);
extern "C" void ApplyGsSettings(void*);
static inline void SetPv(OPT*key,CRef r,void*context){RefSetGlobalSaveVar(key,r,context);}
static inline void SetInt(OPT*key,int x){CRef r(x);SetPv(key,r,D_003D4A60);}
static inline void SetBool(OPT*key,bool x){CRef r(x);SetPv(key,r,D_003D4A60);}
static inline void SetFloat(OPT*key,float x){CRef r(x);SetPv(key,r,D_003D4A60);}
typedef char Final_Size_1[sizeof(MSG)==4?1:-1];
typedef char Final_Size_2[sizeof(GAME)==15176?1:-1];
typedef char Final_Size_3[sizeof(int)==4?1:-1];
typedef char Final_Size_4[sizeof(float)==4?1:-1];
typedef char Final_Size_5[sizeof(bool)==4?1:-1];
typedef char Final_Size_6[sizeof(void*)==4?1:-1];
/** @sly2-annot addr=0x1AFB88
 * Purpose: Apply option item (msg kinds 103..): set option values (CRef setters) and ApplyGsSettings
 * Info: proposed name ActivatePauseVideoItem (T2); TU menu
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/hud-ui.md#menu-0x1ae928-0x1b0b78
 * @end-sly2-annot */
extern "C" void func_001AFB88(void*owner,MSG*msg){
 switch(msg->kind){
 case 103:
  SetFloat(D_003E0FC4,1.0f-D_003DD190);
  SetPauseMenuState(owner,9);
  break;
 case 104:
  SetFloat(D_003E0FC8,1.0f-D_003DD18C);
  SetPauseMenuState(owner,9);
  break;
 case 63:{
  GAME*game=&D_003DD490;
  SetFloat(game->first,D_002F6468-636.0f);
  SetFloat(game->second,D_002F646C-50.0f);
  SetPauseMenuState(owner,9);
  break;
 }
 case 78:SetInt(D_003E0FC0,0);break;
 case 77:SetInt(D_003E0FC0,2);break;
 case 76:SetInt(D_003E0FC0,1);break;
 case 79:case 80:SetBool(D_003E0FCC,msg->kind==79);break;
 case 72:case 73:{
  bool enabled=msg->kind==73;
  if(enabled)D_002C2D8C=3;else D_002C2D8C=2;
  SetBool(D_003E0FD8,enabled);
  break;
 }
 case 81:case 82:SetBool(D_003E0FDC,msg->kind==82);break;
 case 83:case 84:SetBool(D_003E0FE0,msg->kind==84);break;
 default:break;
 }
 ApplyGsSettings(D_003D4A60);
}

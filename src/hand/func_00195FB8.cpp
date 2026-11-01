typedef int qword __attribute__((mode(TI)));
union U{qword q;float f[4];int i[4];};struct M3{U row[3];};struct VTE{short delta,pad;void*fn;};
struct SPACE;struct OBJECT;struct PROVIDER;struct REFERENCE;struct EVENT;struct EFFECT;struct SCENE;struct SECONDARY;
struct DATA{char pad0[4];float amount;};struct SOURCE{char pad0[0x114];DATA*data;char pad118[0x64];int state;};
struct CLIP{char pad0[0x74];float duration;};struct ANIMATION{char pad0[0x14];CLIP*clip;};struct COMPONENT{void*object;};
struct BODY{char pad0[0x70];U position;char pad80[0x20];SPACE*frame;};struct MOTION{char pad0[0x80];U acceleration;};
struct ACTOR{char pad0[12];VTE*vt;char pad10[0x48];BODY*body;char pad5C[0x8C];MOTION*motion;char padEC[0x28];DATA*data;char pad118[0x78];U velocity;char pad1A0[8];float heading;char pad1AC[0x31C];int category;char pad4CC[0x434];char secondary[0x30];CLIP*secondaryClip;char pad934[0x50];SOURCE*secondaryFocus;char pad988[0x688];U offset;char pad1020[0x10];ANIMATION*animation;COMPONENT component;char pad1038[0x4DC];CLIP*clip1514;CLIP*clip1518;char pad151C[0x338];SOURCE*primaryFocus;char pad1858[0x4D0];EFFECT*effect;};
struct LINK{SPACE*space;void*target;};union WORD{int bits;float amount;};struct TRACK{int opaque;WORD control;float weight;};struct QUERY{OBJECT*object;PROVIDER*provider;REFERENCE*reference;int tag;U point;};
struct CONTEXT{ACTOR*actor;int state,variant;SOURCE*source;char pad10[8];float stamp;int mode;float time;int flags24,flags28,flags2C;int actionFlag;float gain,turnGain,height;int combo;float time44,time48,time4C;char pad50[0x6C];int fieldBC;char padC0[8];LINK*link;char padCC[4];U relative,direction,offset;char pad100[0x20];CLIP*base[5];CLIP*alternate[5];CLIP*special148,*special14C,*special150,*special154,*special158,*special15C;char pad160[8];float oldScale;void*focus;char pad170[4];int focused;CLIP*scripted;float scriptedGain;char pad180[4];TRACK tracks[4];char pad1B4[4];CLIP*multi;SOURCE*scriptFocus;char pad1C0[8];CLIP*special1C8;char pad1CC[4];QUERY query;EVENT*event;};
struct CLOCK{float time,dt;};struct RANGE{float low,high;};struct TWO_ROWS{U row[2];};
struct WORK{U temp;M3 basis;U scalar,bodyPosition,goal,delta,acceleration,velocity,endVelocity,duration;};
extern "C"{extern SCENE D_003DD490;extern CLOCK D_002DDED0;extern ACTOR*D_002E1E40;extern U D_002A2DB0,D_002A2DC0,D_002A2DD0;extern float D_002BF780[],D_002BF7A0;extern RANGE D_002BF798;
void func_00204F78(ACTOR*,void*,int);void RecalcSwOxfFilterForObject(SCENE*,void*);void func_00183B80(SOURCE*,int);void func_002303A0(ACTOR*,BODY*,int);void func_001CCAC0(SECONDARY*,int);void SetJtJtcs__FP2JTi(ACTOR*,int);void SetSmaGoal(EFFECT*,int);void SetStepplayerTnOverride(ACTOR*,void*);void ReleaseAct(EVENT**);void func_001F6860(ACTOR*,int);
qword func_0022FC48(SPACE*,SPACE*,qword);qword ConvertXfVec(SPACE*,SPACE*,qword);void GetNormalVectors(qword,qword,qword,TWO_ROWS*);qword func_001A62E8(ACTOR*);void func_00204DA8(ACTOR*,float);qword func_0019A750(CONTEXT*,int);void func_00204660(ACTOR*,float);void func_001384C0(CLIP*);int FIsBasicDerivedFrom__FP5BASIC3CID(OBJECT*,int);qword func_0019F3F8(ACTOR*,QUERY*,float);float func_00196928(qword,qword,float,float,float,float);EVENT*func_0012DE58(ACTOR*,BODY*,int,float,qword,qword);void SetStepRadTargetVec(ACTOR*,qword);void ReleaseSma(COMPONENT*);void func_001305B8(CLIP*,ACTOR*,float,float,int,ANIMATION**);}
static inline void Notify(ACTOR*p,int slot){VTE&v=p->vt[slot];((void(*)(void*))v.fn)((char*)p+v.delta);}
static inline void ClearXYZ(U*v){v->i[2]=0;v->i[1]=0;v->i[0]=0;}
static inline void ClearVelocity(ACTOR*p){ClearXYZ(&p->velocity);}
static inline bool Less(int x,int limit){return x<limit;}
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char Pointer_Width[sizeof(void*)==4?1:-1];
typedef char Float_Width[sizeof(float)==4?1:-1];
typedef char Int_Width[sizeof(int)==4?1:-1];
typedef char Short_Width[sizeof(short)==2?1:-1];
typedef char U_Alignment[__alignof__(U)==16?1:-1];
typedef char WORK_Alignment[__alignof__(WORK)==16?1:-1];
typedef char qword_Size[sizeof(qword)==16?1:-1];
typedef char U_Size[sizeof(U)==16?1:-1];
typedef char M3_Size[sizeof(M3)==48?1:-1];
typedef char VTE_Size[sizeof(VTE)==8?1:-1];
typedef char DATA_Size[sizeof(DATA)==8?1:-1];
typedef char SOURCE_Size[sizeof(SOURCE)==384?1:-1];
typedef char CLIP_Size[sizeof(CLIP)==120?1:-1];
typedef char ANIMATION_Size[sizeof(ANIMATION)==24?1:-1];
typedef char COMPONENT_Size[sizeof(COMPONENT)==4?1:-1];
typedef char BODY_Size[sizeof(BODY)==176?1:-1];
typedef char MOTION_Size[sizeof(MOTION)==144?1:-1];
typedef char ACTOR_Size[sizeof(ACTOR)==7472?1:-1];
typedef char LINK_Size[sizeof(LINK)==8?1:-1];
typedef char WORD_Size[sizeof(WORD)==4?1:-1];
typedef char WORD_bits_Offset[FIELD_OFFSET(WORD,bits)==0?1:-1];
typedef char WORD_amount_Offset[FIELD_OFFSET(WORD,amount)==0?1:-1];
typedef char TRACK_Size[sizeof(TRACK)==12?1:-1];
typedef char QUERY_Size[sizeof(QUERY)==32?1:-1];
typedef char CONTEXT_Size[sizeof(CONTEXT)==512?1:-1];
typedef char CLOCK_Size[sizeof(CLOCK)==8?1:-1];
typedef char RANGE_Size[sizeof(RANGE)==8?1:-1];
typedef char TWO_ROWS_Size[sizeof(TWO_ROWS)==32?1:-1];
typedef char WORK_Size[sizeof(WORK)==192?1:-1];
typedef char VTE_delta_Offset[FIELD_OFFSET(VTE,delta)==0?1:-1];
typedef char VTE_fn_Offset[FIELD_OFFSET(VTE,fn)==4?1:-1];
typedef char DATA_amount_Offset[FIELD_OFFSET(DATA,amount)==4?1:-1];
typedef char SOURCE_data_Offset[FIELD_OFFSET(SOURCE,data)==276?1:-1];
typedef char SOURCE_state_Offset[FIELD_OFFSET(SOURCE,state)==380?1:-1];
typedef char CLIP_duration_Offset[FIELD_OFFSET(CLIP,duration)==116?1:-1];
typedef char ANIMATION_clip_Offset[FIELD_OFFSET(ANIMATION,clip)==20?1:-1];
typedef char COMPONENT_object_Offset[FIELD_OFFSET(COMPONENT,object)==0?1:-1];
typedef char BODY_position_Offset[FIELD_OFFSET(BODY,position)==112?1:-1];
typedef char BODY_frame_Offset[FIELD_OFFSET(BODY,frame)==160?1:-1];
typedef char MOTION_acceleration_Offset[FIELD_OFFSET(MOTION,acceleration)==128?1:-1];
typedef char ACTOR_vt_Offset[FIELD_OFFSET(ACTOR,vt)==12?1:-1];
typedef char ACTOR_body_Offset[FIELD_OFFSET(ACTOR,body)==88?1:-1];
typedef char ACTOR_motion_Offset[FIELD_OFFSET(ACTOR,motion)==232?1:-1];
typedef char ACTOR_data_Offset[FIELD_OFFSET(ACTOR,data)==276?1:-1];
typedef char ACTOR_velocity_Offset[FIELD_OFFSET(ACTOR,velocity)==400?1:-1];
typedef char ACTOR_heading_Offset[FIELD_OFFSET(ACTOR,heading)==424?1:-1];
typedef char ACTOR_category_Offset[FIELD_OFFSET(ACTOR,category)==1224?1:-1];
typedef char ACTOR_secondary_Offset[FIELD_OFFSET(ACTOR,secondary)==2304?1:-1];
typedef char ACTOR_secondaryClip_Offset[FIELD_OFFSET(ACTOR,secondaryClip)==2352?1:-1];
typedef char ACTOR_secondaryFocus_Offset[FIELD_OFFSET(ACTOR,secondaryFocus)==2436?1:-1];
typedef char ACTOR_offset_Offset[FIELD_OFFSET(ACTOR,offset)==4112?1:-1];
typedef char ACTOR_animation_Offset[FIELD_OFFSET(ACTOR,animation)==4144?1:-1];
typedef char ACTOR_component_Offset[FIELD_OFFSET(ACTOR,component)==4148?1:-1];
typedef char ACTOR_clip1514_Offset[FIELD_OFFSET(ACTOR,clip1514)==5396?1:-1];
typedef char ACTOR_clip1518_Offset[FIELD_OFFSET(ACTOR,clip1518)==5400?1:-1];
typedef char ACTOR_primaryFocus_Offset[FIELD_OFFSET(ACTOR,primaryFocus)==6228?1:-1];
typedef char ACTOR_effect_Offset[FIELD_OFFSET(ACTOR,effect)==7464?1:-1];
typedef char LINK_space_Offset[FIELD_OFFSET(LINK,space)==0?1:-1];
typedef char LINK_target_Offset[FIELD_OFFSET(LINK,target)==4?1:-1];
typedef char TRACK_opaque_Offset[FIELD_OFFSET(TRACK,opaque)==0?1:-1];
typedef char TRACK_control_Offset[FIELD_OFFSET(TRACK,control)==4?1:-1];
typedef char TRACK_weight_Offset[FIELD_OFFSET(TRACK,weight)==8?1:-1];
typedef char QUERY_object_Offset[FIELD_OFFSET(QUERY,object)==0?1:-1];
typedef char QUERY_provider_Offset[FIELD_OFFSET(QUERY,provider)==4?1:-1];
typedef char QUERY_reference_Offset[FIELD_OFFSET(QUERY,reference)==8?1:-1];
typedef char QUERY_tag_Offset[FIELD_OFFSET(QUERY,tag)==12?1:-1];
typedef char QUERY_point_Offset[FIELD_OFFSET(QUERY,point)==16?1:-1];
typedef char CONTEXT_actor_Offset[FIELD_OFFSET(CONTEXT,actor)==0?1:-1];
typedef char CONTEXT_state_Offset[FIELD_OFFSET(CONTEXT,state)==4?1:-1];
typedef char CONTEXT_variant_Offset[FIELD_OFFSET(CONTEXT,variant)==8?1:-1];
typedef char CONTEXT_source_Offset[FIELD_OFFSET(CONTEXT,source)==12?1:-1];
typedef char CONTEXT_stamp_Offset[FIELD_OFFSET(CONTEXT,stamp)==24?1:-1];
typedef char CONTEXT_mode_Offset[FIELD_OFFSET(CONTEXT,mode)==28?1:-1];
typedef char CONTEXT_time_Offset[FIELD_OFFSET(CONTEXT,time)==32?1:-1];
typedef char CONTEXT_flags24_Offset[FIELD_OFFSET(CONTEXT,flags24)==36?1:-1];
typedef char CONTEXT_flags28_Offset[FIELD_OFFSET(CONTEXT,flags28)==40?1:-1];
typedef char CONTEXT_flags2C_Offset[FIELD_OFFSET(CONTEXT,flags2C)==44?1:-1];
typedef char CONTEXT_actionFlag_Offset[FIELD_OFFSET(CONTEXT,actionFlag)==48?1:-1];
typedef char CONTEXT_gain_Offset[FIELD_OFFSET(CONTEXT,gain)==52?1:-1];
typedef char CONTEXT_turnGain_Offset[FIELD_OFFSET(CONTEXT,turnGain)==56?1:-1];
typedef char CONTEXT_height_Offset[FIELD_OFFSET(CONTEXT,height)==60?1:-1];
typedef char CONTEXT_combo_Offset[FIELD_OFFSET(CONTEXT,combo)==64?1:-1];
typedef char CONTEXT_time44_Offset[FIELD_OFFSET(CONTEXT,time44)==68?1:-1];
typedef char CONTEXT_time48_Offset[FIELD_OFFSET(CONTEXT,time48)==72?1:-1];
typedef char CONTEXT_time4C_Offset[FIELD_OFFSET(CONTEXT,time4C)==76?1:-1];
typedef char CONTEXT_fieldBC_Offset[FIELD_OFFSET(CONTEXT,fieldBC)==188?1:-1];
typedef char CONTEXT_link_Offset[FIELD_OFFSET(CONTEXT,link)==200?1:-1];
typedef char CONTEXT_relative_Offset[FIELD_OFFSET(CONTEXT,relative)==208?1:-1];
typedef char CONTEXT_direction_Offset[FIELD_OFFSET(CONTEXT,direction)==224?1:-1];
typedef char CONTEXT_offset_Offset[FIELD_OFFSET(CONTEXT,offset)==240?1:-1];
typedef char CONTEXT_base_Offset[FIELD_OFFSET(CONTEXT,base)==288?1:-1];
typedef char CONTEXT_alternate_Offset[FIELD_OFFSET(CONTEXT,alternate)==308?1:-1];
typedef char CONTEXT_special148_Offset[FIELD_OFFSET(CONTEXT,special148)==328?1:-1];
typedef char CONTEXT_special14C_Offset[FIELD_OFFSET(CONTEXT,special14C)==332?1:-1];
typedef char CONTEXT_special150_Offset[FIELD_OFFSET(CONTEXT,special150)==336?1:-1];
typedef char CONTEXT_special154_Offset[FIELD_OFFSET(CONTEXT,special154)==340?1:-1];
typedef char CONTEXT_special158_Offset[FIELD_OFFSET(CONTEXT,special158)==344?1:-1];
typedef char CONTEXT_special15C_Offset[FIELD_OFFSET(CONTEXT,special15C)==348?1:-1];
typedef char CONTEXT_oldScale_Offset[FIELD_OFFSET(CONTEXT,oldScale)==360?1:-1];
typedef char CONTEXT_focus_Offset[FIELD_OFFSET(CONTEXT,focus)==364?1:-1];
typedef char CONTEXT_focused_Offset[FIELD_OFFSET(CONTEXT,focused)==372?1:-1];
typedef char CONTEXT_scripted_Offset[FIELD_OFFSET(CONTEXT,scripted)==376?1:-1];
typedef char CONTEXT_scriptedGain_Offset[FIELD_OFFSET(CONTEXT,scriptedGain)==380?1:-1];
typedef char CONTEXT_tracks_Offset[FIELD_OFFSET(CONTEXT,tracks)==388?1:-1];
typedef char CONTEXT_multi_Offset[FIELD_OFFSET(CONTEXT,multi)==440?1:-1];
typedef char CONTEXT_scriptFocus_Offset[FIELD_OFFSET(CONTEXT,scriptFocus)==444?1:-1];
typedef char CONTEXT_special1C8_Offset[FIELD_OFFSET(CONTEXT,special1C8)==456?1:-1];
typedef char CONTEXT_query_Offset[FIELD_OFFSET(CONTEXT,query)==464?1:-1];
typedef char CONTEXT_event_Offset[FIELD_OFFSET(CONTEXT,event)==496?1:-1];
typedef char CLOCK_time_Offset[FIELD_OFFSET(CLOCK,time)==0?1:-1];
typedef char CLOCK_dt_Offset[FIELD_OFFSET(CLOCK,dt)==4?1:-1];
typedef char RANGE_low_Offset[FIELD_OFFSET(RANGE,low)==0?1:-1];
typedef char RANGE_high_Offset[FIELD_OFFSET(RANGE,high)==4?1:-1];
typedef char TWO_ROWS_row_Offset[FIELD_OFFSET(TWO_ROWS,row)==0?1:-1];
typedef char WORK_temp_Offset[FIELD_OFFSET(WORK,temp)==0?1:-1];
typedef char WORK_basis_Offset[FIELD_OFFSET(WORK,basis)==16?1:-1];
typedef char WORK_scalar_Offset[FIELD_OFFSET(WORK,scalar)==64?1:-1];
typedef char WORK_bodyPosition_Offset[FIELD_OFFSET(WORK,bodyPosition)==80?1:-1];
typedef char WORK_goal_Offset[FIELD_OFFSET(WORK,goal)==96?1:-1];
typedef char WORK_delta_Offset[FIELD_OFFSET(WORK,delta)==112?1:-1];
typedef char WORK_acceleration_Offset[FIELD_OFFSET(WORK,acceleration)==128?1:-1];
typedef char WORK_velocity_Offset[FIELD_OFFSET(WORK,velocity)==144?1:-1];
typedef char WORK_endVelocity_Offset[FIELD_OFFSET(WORK,endVelocity)==160?1:-1];
typedef char WORK_duration_Offset[FIELD_OFFSET(WORK,duration)==176?1:-1];
typedef void(*VECTOR_SETTER)(void*,qword);
static inline void PublishReal(U*home,qword value){__asm__ volatile("sqc2 %1,%0":"=m"(home->q):"j"(value));}
/** @sly2-annot addr=0x195FB8
 * Purpose: Set the air context sub-state: leave old / enter new (clips, cane state via SetJtJtcs, landing
 *   target, velocities, SM flags)
 * Info: proposed name SetJtAirState (T1); TU air
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
extern "C" void func_00195FB8(CONTEXT*p,int newMode,SOURCE*source){WORK w;
 if(newMode==p->mode)return;
 switch(p->mode){
 case 17:func_00204F78(p->actor,p->link->target,0);p->link=0;break;
 case 7:{SOURCE*old=p->source;p->source=0;RecalcSwOxfFilterForObject(&D_003DD490,old);break;}
 case 6:{SOURCE*old=p->source;p->source=0;if(p->actor->category!=1){RecalcSwOxfFilterForObject(&D_003DD490,old);if(old->state==1)func_00183B80(old,0);}func_002303A0(p->actor,p->actor->body,2);p->actor->velocity.q=D_002A2DD0.q;break;}
 case 8:case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:case 18:
  func_002303A0(p->actor,p->actor->body,2);p->actor->velocity.q=D_002A2DD0.q;
  if(Less(p->mode,16)&&!Less(p->mode,14)){if(p->actor!=D_002E1E40)func_001CCAC0((SECONDARY*)p->actor->secondary,-1);else SetJtJtcs__FP2JTi(p->actor,-1);}break;
 case 19:case 20:if(Less(newMode,21)&&!Less(newMode,19))break;if(p->actor->effect)SetSmaGoal(p->actor->effect,0x5F8);SetStepplayerTnOverride(p->actor,0);break;
 case 21:ReleaseAct(&p->event);func_002303A0(p->actor,p->actor->body,2);func_001F6860(p->actor,0);break;
 }
 register unsigned clockHigh __asm__("$3"),savedClockHigh __asm__("$21");
 __asm__("lui %0,%%hi(%1)":"=r"(clockHigh):"i"(&D_002DDED0));
 __asm__("daddu %0,%1,$zero":"=r"(savedClockHigh):"r"(clockHigh));
 ANIMATION*active=p->actor->animation;CLIP*clip=0;p->mode=newMode;p->source=source;
 float entryTime,blend;__asm__("mtc1 $zero,%1\n\tlwc1 %0,%%lo(%3)(%2)":"=f"(entryTime),"=f"(blend):"r"(clockHigh),"i"(&D_002DDED0):"memory");p->time=entryTime;int flags=1;float weight=1.0f;
 switch(newMode){
 case 17:{flags=9;Notify(p->actor,91);func_00204F78(p->actor,p->link->target,1);qword point=func_0022FC48(p->link->space,0,p->relative.q);
  register qword direction __asm__("vf1"),negative __asm__("vf2"),upArg __asm__("$5");register U*axisAddress __asm__("$4");
  float negativeScalar=-1.0f;
  __asm__("lui $3,%%hi(%6)\n\tlui %3,%%hi(%7)\n\tlqc2 %0,%4\n\tmfc1 %2,%5\n\taddiu $3,$3,%%lo(%6)\n\tqmtc2.ni %2,%1\n\taddiu %3,%3,%%lo(%7)\n\tlq %2,0($3)\n\tvmulx.xyzw %0,%0,%1x"
   :"=&j"(direction),"=j"(negative),"=&r"(upArg),"=r"(axisAddress):"m"(p->direction.q),"f"(negativeScalar),"i"(&D_002A2DC0),"i"(&D_002A2DB0):"$3","memory");
  register qword axisArg __asm__("$6"),directionArg __asm__("$4");register TWO_ROWS*basisOutput __asm__("$7");
  __asm__("lq %0,0(%2)\n\taddiu %1,%4,0x20\n\tqmfc2.ni %2,%3"
   :"=&r"(axisArg),"=r"(basisOutput),"=r"(directionArg):"j"(direction),"p"(&w.temp),"2"(axisAddress):"memory");
  w.temp.q=point;PublishReal(&w.scalar,negative);w.basis.row[0].q=direction;
  GetNormalVectors(directionArg,upArg,axisArg,basisOutput);
  ACTOR*actor;qword delta;
  {register ACTOR*owner __asm__("$16");qword value;
   __asm__("lqc2 $vf3,%2\n\tlqc2 $vf1,%3\n\tlqc2 $vf5,%4\n\tlqc2 $vf4,%5\n\tlw %0,%6\n\tvmulax.xyz $ACC,$vf5,$vf3x\n\tvmadday.xyz $ACC,$vf4,$vf3y\n\tvmaddz.xyz $vf3,$vf1,$vf3z\n\tlqc2 %1,%7\n\tvsub.xyzw %1,%1,$vf3"
    :"=&r"(owner),"=j"(value)
    :"m"(p->offset.q),"m"(w.basis.row[2].q),"m"(w.basis.row[0].q),"m"(w.basis.row[1].q),"m"(p->actor),"m"(w.temp.q)
    :"vf1","vf3","vf4","vf5");
   actor=owner;delta=value;}
  register ACTOR*pointOwner __asm__("$4");
  __asm__ volatile("daddu %0,%2,$zero\n\tsqc2 %3,%1":"=r"(pointOwner),"=m"(w.temp.q):"r"(actor),"j"(delta));
  {qword point=func_001A62E8(pointOwner);register qword position __asm__("vf1");
   __asm__("qmtc2.ni %1,%0":"=j"(position):"r"(point));
   __asm__("vsub.xyzw %0,%0,%1":"+j"(delta):"j"(position));actor->offset.q=delta;}
  ClearVelocity(p->actor);p->flags28=0;clip=p->special154;
  if(p->focused){func_00204DA8(p->actor,p->oldScale);SetStepplayerTnOverride(p->actor,0);p->focused=0;}break;}
 case 0:case 1:case 4:case 5:case 7:
  switch(p->state){case 13:clip=p->special148;break;case 14:clip=p->special158;break;case 16:clip=p->actor->clip1514;break;case 15:clip=p->actor->clip1518;break;}
  if(!clip)clip=p->base[p->variant];if(!clip)clip=p->base[0];break;
 case 2:case 3:if(p->state==14)clip=p->special15C;if(!clip)clip=p->alternate[p->variant];if(!clip)clip=p->alternate[0];if(!clip)clip=p->base[0];p->flags28=0;break;
 case 6:func_002303A0(p->actor,p->actor->body,17);RecalcSwOxfFilterForObject(&D_003DD490,source);func_00183B80(source,1);ClearVelocity(p->actor);clip=p->actor->animation->clip;break;
 case 8:func_002303A0(p->actor,p->actor->body,17);ClearVelocity(p->actor);clip=active->clip;break;
 case 11:case 12:case 13:case 14:case 15:
  func_002303A0(p->actor,p->actor->body,17);ClearVelocity(p->actor);p->flags28=0;p->variant=0;clip=p->base[0];
  if(Less(p->mode,16)&&!Less(p->mode,14)){ACTOR*actor=p->actor;if(actor==D_002E1E40){actor->primaryFocus=p->source;SetJtJtcs__FP2JTi(actor,0);}else{actor->secondaryFocus=p->source;weight=0.0f;func_001CCAC0((SECONDARY*)p->actor->secondary,0);clip=p->actor->secondaryClip;blend=clip->duration*0.5f;}}break;
 case 9:case 10:case 16:case 18:{func_002303A0(p->actor,p->actor->body,17);ClearVelocity(p->actor);p->flags28=0;ACTOR*actor=p->actor;VTE&v=actor->vt[35];void*receiver=(char*)actor+v.delta;qword velocity=func_0019A750(p,4);((void(*)(void*,qword))v.fn)(receiver,velocity);p->variant=0;if(p->mode==16)clip=p->special14C;else clip=p->special148;active=0;break;}
 case -1:if(p->focused){func_00204DA8(p->actor,p->oldScale);SetStepplayerTnOverride(p->actor,0);p->focused=0;}p->fieldBC=0;p->state=-1;break;
 case 19:func_00204660(p->actor,p->actor->heading-D_002BF780[p->variant]);p->flags28=0;clip=p->scripted;SetStepplayerTnOverride(p->actor,p->scriptFocus);p->scriptFocus->data->amount=D_002BF798.low*0.5f+D_002BF798.high*0.5f;if(p->actor->effect)SetSmaGoal(p->actor->effect,0x5F7);break;
 case 20:p->tracks[0].control.bits=0;p->tracks[1].control.bits=0;p->tracks[2].control.bits=0;p->tracks[3].control.bits=0;p->scriptedGain=1.0f;func_001384C0(p->multi);clip=p->multi;flags=9;break;
 case 21:{register float minimum __asm__("$f21"),maximum __asm__("$f20");register qword bodyPosition __asm__("$3");register OBJECT*queryObject __asm__("$4");
  __asm__("lw $2,0(%4)\n\tlw %1,0x1D0(%4)\n\tlw %0,0x58($2)\n\tli.s %2,-1.00000000000000000000e3\n\tlq %0,0x70(%0)\n\tli.s %3,-1.00000000000000000000e0"
   :"=&r"(bodyPosition),"=r"(queryObject),"=f"(minimum),"=f"(maximum):"r"(p),"m"(p->actor),"m"(p->query.object):"$2","at","memory");
  w.bodyPosition.q=bodyPosition;
  if(queryObject&&(FIsBasicDerivedFrom__FP5BASIC3CID(queryObject,19)||FIsBasicDerivedFrom__FP5BASIC3CID(p->query.object,11))){minimum=0.0f;maximum=500.0f;}
  qword queried=func_0019F3F8(p->actor,&p->query,D_002BF7A0);
  register ACTOR*queryOwner __asm__("$3");register qword queriedVector __asm__("vf2");register float maximumArg __asm__("$f15"),minimumArg __asm__("$f13");
  __asm__("lw %0,%5\n\tqmtc2.ni %4,%1\n\tsq %4,%2\n\tmov.s %3,%6"
   :"=&r"(queryOwner),"=j"(queriedVector),"=m"(w.goal.q),"=f"(maximumArg)
   :"r"(queried),"m"(p->actor),"f"(maximum));
  register float durationUnit __asm__("$f14"),rangeLimit __asm__("$f12");
  __asm__("lqc2 $vf3,%3\n\tmov.s %1,%4\n\tvsub.xyzw %0,%0,$vf3\n\tli.s %2,1.00000000000000000000e0"
   :"+j"(queriedVector),"=f"(minimumArg),"=f"(durationUnit):"m"(w.bodyPosition.q),"f"(minimum):"vf3","at");
  __asm__("sqc2 %2,%0\n\tli.s %1,1.00000000000000000000e3":"=m"(w.delta.q),"=f"(rangeLimit):"j"(queriedVector):"at");
  register qword deltaArg __asm__("$4"),accelerationArg __asm__("$2");
  __asm__("lw %1,0xE8(%3)\n\tlq %0,%4\n\tlq %1,0x80(%1)\n\tsq %1,%2"
   :"=r"(deltaArg),"=&r"(accelerationArg),"=m"(w.acceleration.q)
   :"r"(queryOwner),"m"(w.delta.q),"m"(queryOwner->motion):"memory");
  float solved=func_00196928(deltaArg,accelerationArg,rangeLimit,minimumArg,durationUnit,maximumArg);maximum=D_002BF7A0;
  {register float negativeHalf __asm__("$f1")=-0.5f;register ACTOR*velocityOwner __asm__("$5");
   __asm__("max.s %0,%0,%3\n\tlw %1,%4\n\tmul.s %2,%0,%2"
    :"+f"(maximum),"=r"(velocityOwner),"+f"(negativeHalf):"f"(solved),"m"(p->actor));
   register qword full __asm__("vf1"),quotient __asm__("vf3");register VTE*vt __asm__("$2");register unsigned halfBits __asm__("$3");
   __asm__("lqc2 $vf2,%4\n\tmfc1 %3,%5\n\tqmtc2.ni %3,%0\n\tlw %2,%6\n\tmfc1 %3,%7\n\tvdiv $Q,$vf0w,%0x\n\tvwaitq\n\tvmulq.xyzw %1,$vf2,$Q"
    :"=j"(full),"=j"(quotient),"=r"(vt),"=&r"(halfBits)
    :"m"(w.delta.q),"f"(maximum),"m"(velocityOwner->vt),"f"(negativeHalf):"vf2");
   register qword half __asm__("vf2"),velocity __asm__("vf4");
   __asm__("lqc2 %1,%4\n\tqmtc2.ni %2,%0\n\tvadda.xyzw $ACC,%3,$vf0\n\tvmaddx.xyzw %1,%1,%0x"
    :"=&j"(half),"=&j"(velocity):"r"(halfBits),"j"(quotient),"m"(w.acceleration.q));
   register int adjustment __asm__("$4");
   __asm__("lh %0,0x118(%2)\n\tsqc2 %3,%1":"=r"(adjustment),"=m"(w.velocity.q):"r"(vt),"j"(velocity),"m"(vt[35].delta));
   register void*receiver __asm__("$4");register VECTOR_SETTER setter __asm__("$3");
   __asm__("addu %0,%2,%4\n\tlw %1,0x11C(%3)":"=&r"(receiver),"=r"(setter):"r"(velocityOwner),"r"(vt),"0"(adjustment),"m"(vt[35].fn));
   PublishReal(&w.temp,full);
   register qword velocityArg __asm__("$5");
   __asm__ volatile("lq %0,%2\n\tsqc2 %3,%1":"=r"(velocityArg),"=m"(w.temp.q):"m"(w.velocity.q),"j"(half));
   w.duration.q=full;setter(receiver,velocityArg);
  }
  register qword fullAfter __asm__("vf1"),endVelocity __asm__("vf2");register SPACE*coordinateSpace __asm__("$4");
  __asm__("lqc2 %0,%3\n\tdaddu %1,%z6,$zero\n\tlqc2 %2,%4\n\tlqc2 $vf3,%5\n\tvadda.xyzw $ACC,%2,$vf0\n\tvmaddx.xyzw %2,$vf3,%0x"
   :"=j"(fullAfter),"=&r"(coordinateSpace),"=j"(endVelocity):"m"(w.duration.q),"m"(w.velocity.q),"m"(w.acceleration.q),"J"((void*)0):"vf3");
  register float deadline __asm__("$f21");register qword goalArg __asm__("$6");
  __asm__("lwc1 %0,%%lo(%4)(%3)\n\tsqc2 %5,%1\n\tlq %2,%6\n\tadd.s %0,%0,%7"
   :"=&f"(deadline),"=m"(w.endVelocity.q),"=r"(goalArg):"r"(savedClockHigh),"i"(&D_002DDED0),"j"(endVelocity),"m"(w.goal.q),"f"(maximum):"memory");
  register BODY*coordinateBody __asm__("$2");
  __asm__ volatile("lw $3,%2\n\tlw %0,0x58($3)\n\tsqc2 %3,%1":"=&r"(coordinateBody),"=m"(w.temp.q):"m"(p->actor),"j"(fullAfter):"$3","memory");
  qword localGoal=func_0022FC48(coordinateSpace,coordinateBody->frame,goalArg);qword localEnd=ConvertXfVec(0,p->actor->body->frame,w.endVelocity.q);
  p->event=func_0012DE58(p->actor,p->actor->body,5,deadline,localGoal,localEnd);func_001F6860(p->actor,1);
  {register ACTOR*cleared __asm__("$3");register qword directionArg __asm__("$5");
   __asm__("lw %0,%2\n\tlq %1,%3":"=&r"(cleared),"=r"(directionArg):"m"(p->actor),"m"(w.delta.q));
   ClearVelocity(cleared);SetStepRadTargetVec(p->actor,directionArg);}clip=p->base[0];break;}
 case 22:clip=p->special1C8;flags=9;break;
 }
 switch(p->mode){case 0:case 2:p->actionFlag=0;break;case 18:RecalcSwOxfFilterForObject(&D_003DD490,p->actor);break;}
 if(p->mode!=-1){if(!active||active->clip!=clip){ReleaseSma(&p->actor->component);func_001305B8(clip,p->actor,blend,weight,flags,&p->actor->animation);}Notify(p->actor,90);}
}

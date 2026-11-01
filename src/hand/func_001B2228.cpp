typedef int qword __attribute__((mode(TI)));
union U { qword q; float f[4];};
struct V { float x,y,z,w; } __attribute__((aligned(16)));
struct VT { short d,pad;void *fn; };
struct LO;struct CONTROLLER;struct ASEG;struct ASEGA;struct ACTION;
struct P {char pad0[0xC];VT *vt;char pad1[0x5C-0x10];CONTROLLER *controller;
 char pad2[0x210-0x60];LO *source;char opaque214[12];V position;
 ASEG *aseg;ASEGA *asega[12];int enabled;ACTION *action;float parameter;
 float g270,g274,g278;};
struct HIT { qword q[4];float t0,t1;int count;void *owner0,*owner1;};
struct CLOCK {float t,dt;};
extern "C" CLOCK D_002DDED0;
extern "C" qword D_002A2DD0;
struct LO;
extern "C" void func_001B1E88(P*,LO*,V*);
extern "C" V*func_001B2398(V*,P*,float);
extern "C" qword func_0011E9F8(qword,float);
extern "C" qword func_001A62E8(P*);
extern "C" int func_0014C748(qword,qword,qword,int,HIT*,float);
inline qword Scale(qword a,qword k){__asm__("vmulx.xyzw %0,%1,%2x":"=j"(k):"j"(a),"0"(k));return k;}
struct WORK{U pos;V dir;U velocity,tmp;V factor;HIT hit;V scalar;U direction,spill;};
/** @sly2-annot addr=0x1B2228
 * Purpose: CID-82 Fire (vt78): FireMissile then, if no fixed time, predict target position 0.5 s ahead and
 *   set clamped launch velocity
 * Info: proposed name FireSplmiss (T1); class CID82 slot78; TU missile
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/gameplay-objects.md#missile-0x1b1db8-0x1b2820
 * @end-sly2-annot */
extern "C" void func_001B2228(P*p,LO*source,V*point){
 WORK w;
 w.pos.q=*(qword*)point;
 func_001B1E88(p,source,(V*)&w.pos);
 if(p->g270>0.0f)p->g278=D_002DDED0.t+p->g270;
 else{
  func_001B2398((V*)&w.velocity,p,0.5f);
  register qword copied __asm__("$2");register VT*table __asm__("$3");register float negative __asm__("$f0");
  __asm__("lq %0,%4\n\tlw %1,%5\n\tli.s %2,%6\n\tsq %0,%3":"=&r"(copied),"=r"(table),"=f"(negative),"=m"(w.dir):"m"(w.velocity.q),"m"(p->vt),"F"(-0.5f));
  register qword k __asm__("vf1");register qword(*method)(void*) __asm__("$2");register void*receiver __asm__("$4");
  __asm__("mfc1 %0,%4\n\tlh %1,%5\n\tqmtc2.ni %0,%2\n\tlw %0,%6\n\tsqc2 %2,%3\n\taddu %1,%7,%1":"=&r"(method),"=&r"(receiver),"=j"(k),"=m"(w.tmp.q):"f"(negative),"m"(table[33].d),"m"(table[33].fn),"r"(p));
  w.spill.q=k;
  qword raw=method(receiver);
  w.velocity.q=Scale(raw,w.spill.q);
  qword clamped=func_0011E9F8(w.velocity.q,0.75f*p->g274);
  register qword velocity __asm__("vf1");
  __asm__("qmtc2.ni %1,%0":"=j"(velocity):"r"(clamped));
  register qword scalarWord __asm__("$2");
  __asm__("mfc1 %0,%3\n\tsqc2 %4,%1\n\tsq %0,%2":"=&r"(scalarWord),"=m"(w.velocity.q),"=m"(w.scalar):"f"(1.5f),"j"(velocity));
  register P*owner __asm__("$4");
  __asm__("daddu %0,%4,$zero\n\tlq %1,%5\n\tsqc2 %6,%2\n\tsq %1,%3\n\tlq %1,%7":"=r"(owner),"=&r"(scalarWord),"=m"(w.spill.q),"=m"(w.direction.q):"r"(p),"m"(w.dir),"j"(velocity),"m"(w.scalar));
  *(qword*)&w.factor=scalarWord;
  qword fresh=func_001A62E8(owner);
  register qword direction __asm__("vf3"),position __asm__("vf4");register qword*zero __asm__("$3");
  __asm__("lqc2 %0,%3\n\tlui %1,%%hi(%5)\n\tqmtc2.ni %4,%2\n\taddiu %1,%1,%%lo(%5)":"=j"(direction),"=&r"(zero),"=j"(position):"m"(w.direction.q),"r"(fresh),"i"(&D_002A2DD0));
  register qword d __asm__("vf2"),acc __asm__("vf1"),amount __asm__("vf3");register int count __asm__("$7");
  __asm__("vsub.xyzw %0,%4,%5\n\tlqc2 %1,%6\n\tlqc2 %2,%7\n\taddiu %3,$zero,%8\n\tvadda.xyzw ACC,%1,$vf0\n\tvmaddx.xyzw %1,%0,%2x":"=j"(d),"=j"(acc),"=j"(amount),"=r"(count):"j"(direction),"j"(position),"m"(w.spill.q),"m"(w.scalar),"I"(1));
  register qword originArgument __asm__("$4"),endpointArgument __asm__("$6"),velocityArgument __asm__("$5");
  register float radius __asm__("$f12");register HIT*output __asm__("$8");
  __asm__("lq %0,%5\n\tlwc1 %1,%6\n\tqmfc2.ni %2,%7\n\tlq %3,%8\n\tla %4,%a9":"=&r"(originArgument),"=f"(radius),"=&r"(endpointArgument),"=&r"(velocityArgument),"=r"(output):"m"(*zero),"m"(p->g274),"j"(acc),"m"(w.velocity.q),"p"(&w.hit));
  w.tmp.q=acc;
  func_0014C748(originArgument,velocityArgument,endpointArgument,count,output,radius);
  register float max __asm__("$f1");
  register float lo __asm__("$f1")=0.25f;
  register float hi __asm__("$f3")=w.hit.t1;
  __asm__("max.s %0,%1,%2":"=f"(max):"f"(lo),"f"(hi));
  register float duration __asm__("$f2")=1.0f/max;
  p->g278=D_002DDED0.t+duration;
 }
}

#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char U_q_Offset[FIELD_OFFSET(U,q)==0?1:-1];
typedef char U_f_Offset[FIELD_OFFSET(U,f)==0?1:-1];
typedef char V_x_Offset[FIELD_OFFSET(V,x)==0?1:-1];
typedef char V_y_Offset[FIELD_OFFSET(V,y)==4?1:-1];
typedef char V_z_Offset[FIELD_OFFSET(V,z)==8?1:-1];
typedef char V_w_Offset[FIELD_OFFSET(V,w)==12?1:-1];
typedef char VT_d_Offset[FIELD_OFFSET(VT,d)==0?1:-1];
typedef char VT_fn_Offset[FIELD_OFFSET(VT,fn)==4?1:-1];
typedef char P_vt_Offset[FIELD_OFFSET(P,vt)==12?1:-1];
typedef char P_controller_Offset[FIELD_OFFSET(P,controller)==92?1:-1];
typedef char P_source_Offset[FIELD_OFFSET(P,source)==528?1:-1];
typedef char P_position_Offset[FIELD_OFFSET(P,position)==544?1:-1];
typedef char P_aseg_Offset[FIELD_OFFSET(P,aseg)==560?1:-1];
typedef char P_asega_Offset[FIELD_OFFSET(P,asega)==564?1:-1];
typedef char P_enabled_Offset[FIELD_OFFSET(P,enabled)==612?1:-1];
typedef char P_action_Offset[FIELD_OFFSET(P,action)==616?1:-1];
typedef char P_parameter_Offset[FIELD_OFFSET(P,parameter)==620?1:-1];
typedef char P_g270_Offset[FIELD_OFFSET(P,g270)==624?1:-1];
typedef char P_g274_Offset[FIELD_OFFSET(P,g274)==628?1:-1];
typedef char P_g278_Offset[FIELD_OFFSET(P,g278)==632?1:-1];
typedef char HIT_q_Offset[FIELD_OFFSET(HIT,q)==0?1:-1];
typedef char HIT_t0_Offset[FIELD_OFFSET(HIT,t0)==64?1:-1];
typedef char HIT_t1_Offset[FIELD_OFFSET(HIT,t1)==68?1:-1];
typedef char HIT_count_Offset[FIELD_OFFSET(HIT,count)==72?1:-1];
typedef char HIT_owner0_Offset[FIELD_OFFSET(HIT,owner0)==76?1:-1];
typedef char HIT_owner1_Offset[FIELD_OFFSET(HIT,owner1)==80?1:-1];
typedef char CLOCK_t_Offset[FIELD_OFFSET(CLOCK,t)==0?1:-1];
typedef char CLOCK_dt_Offset[FIELD_OFFSET(CLOCK,dt)==4?1:-1];
typedef char WORK_pos_Offset[FIELD_OFFSET(WORK,pos)==0?1:-1];
typedef char WORK_dir_Offset[FIELD_OFFSET(WORK,dir)==16?1:-1];
typedef char WORK_velocity_Offset[FIELD_OFFSET(WORK,velocity)==32?1:-1];
typedef char WORK_tmp_Offset[FIELD_OFFSET(WORK,tmp)==48?1:-1];
typedef char WORK_factor_Offset[FIELD_OFFSET(WORK,factor)==64?1:-1];
typedef char WORK_hit_Offset[FIELD_OFFSET(WORK,hit)==80?1:-1];
typedef char WORK_scalar_Offset[FIELD_OFFSET(WORK,scalar)==176?1:-1];
typedef char WORK_direction_Offset[FIELD_OFFSET(WORK,direction)==192?1:-1];
typedef char WORK_spill_Offset[FIELD_OFFSET(WORK,spill)==208?1:-1];
typedef char U_PrefixSize[sizeof(U)==16?1:-1];
typedef char U_Alignment[__alignof__(U)==16?1:-1];
typedef char V_PrefixSize[sizeof(V)==16?1:-1];
typedef char V_Alignment[__alignof__(V)==16?1:-1];
typedef char VT_PrefixSize[sizeof(VT)==8?1:-1];
typedef char VT_Alignment[__alignof__(VT)==4?1:-1];
typedef char P_PrefixSize[sizeof(P)==640?1:-1];
typedef char P_Alignment[__alignof__(P)==16?1:-1];
typedef char HIT_PrefixSize[sizeof(HIT)==96?1:-1];
typedef char HIT_Alignment[__alignof__(HIT)==16?1:-1];
typedef char CLOCK_PrefixSize[sizeof(CLOCK)==8?1:-1];
typedef char CLOCK_Alignment[__alignof__(CLOCK)==4?1:-1];
typedef char WORK_PrefixSize[sizeof(WORK)==224?1:-1];
typedef char WORK_Alignment[__alignof__(WORK)==16?1:-1];
typedef char qword_Width[sizeof(qword)==16?1:-1];
typedef char int_Width[sizeof(int)==4?1:-1];
typedef char float_Width[sizeof(float)==4?1:-1];
typedef char pointer_Width[sizeof(void*)==4?1:-1];
typedef char short_Width[sizeof(short)==2?1:-1];
typedef char qword_Alignment[__alignof__(qword)==16?1:-1];

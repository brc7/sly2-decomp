
typedef int qword __attribute__((mode(TI)));
union V{qword q;float f[4];};
struct ACTOR;struct MOVE{ACTOR*actor;};
class A0{public:char pad[224];};
class O{public:int pad[8];virtual void f1();virtual void f2();virtual void f3(ACTOR*,V*,void*,float);};
class Y:public A0,public O{};
struct S{MOVE*move;Y*object;int pad8[2];qword q;};
extern qword D_002A2DD0;
extern "C" void func_0019A4A0(MOVE*,qword,Y*,qword,qword,float*,float*,qword*,qword*);
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char qword_Size[sizeof(qword)==16?1:-1];
typedef char V_Size[sizeof(V)==16?1:-1];
typedef char MOVE_Size[sizeof(MOVE)==4?1:-1];
typedef char MOVE_actor_Offset[FIELD_OFFSET(MOVE,actor)==0?1:-1];
typedef char A0_Size[sizeof(A0)==224?1:-1];
typedef char O_Size[sizeof(O)==36?1:-1];
typedef char Y_Size[sizeof(Y)==224+36?1:-1];
typedef char S_Size[sizeof(S)==32?1:-1];
typedef char S_move_Offset[FIELD_OFFSET(S,move)==0?1:-1];
typedef char S_object_Offset[FIELD_OFFSET(S,object)==4?1:-1];
typedef char S_q_Offset[FIELD_OFFSET(S,q)==16?1:-1];
/** @sly2-annot addr=0x19AAB8
 * Purpose: MinimizeRange callback: object vfunc gives target pos at u, then MeasureJtJumpToTarget cost
 * Info: proposed name GMeasureJumpHpnt (T2); TU air
 * Notes: names_v2 B proposed GetJtJumpToTargetVelocity__FP2JTP6VECTOR (0.856)
 * Confidence: low (draft) [T2 0.45] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
extern "C" float func_0019AAB8(S*s,float parameter){
 V v;float r;
 s->object->f3(s->move->actor,&v,0,parameter);
 func_0019A4A0(s->move,s->q,s->object,v.q,D_002A2DD0,0,&r,0,0);
 return r;
}

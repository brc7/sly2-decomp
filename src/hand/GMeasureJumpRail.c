
typedef int qword __attribute__((mode(TI)));
typedef struct CONTROL CONTROL;typedef struct MOVE MOVE;typedef struct OBJECT OBJECT;
typedef struct S{CONTROL*a;MOVE*b;OBJECT*c;int padC;qword q;}S;
void GetJtRailLanding(CONTROL*,OBJECT*,qword*,qword*,float);
void MeasureJtJumpToTarget(MOVE*,qword,OBJECT*,qword,qword,float*,float*,qword*,qword*);
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0x10000)->m)-0x10000)
typedef char qword_Size[sizeof(qword)==16?1:-1];
typedef char S_Size[sizeof(S)==32?1:-1];
typedef char S_a_Offset[FIELD_OFFSET(S,a)==0?1:-1];
typedef char S_b_Offset[FIELD_OFFSET(S,b)==4?1:-1];
typedef char S_c_Offset[FIELD_OFFSET(S,c)==8?1:-1];
typedef char S_q_Offset[FIELD_OFFSET(S,q)==16?1:-1];
/** @sly2-annot addr=0x19AA58
 * Purpose: MinimizeRange callback: GetJtRailLanding at u then MeasureJtJumpToTarget cost
 * Info: proposed name GMeasureJumpRail (T1); TU air
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
float GMeasureJumpRail(S*s,float parameter){
 float r;qword a,b;
 GetJtRailLanding(s->a,s->c,&a,&b,parameter);
 MeasureJtJumpToTarget(s->b,s->q,s->c,a,b,0,&r,0,0);
 return r;
}

typedef int qword __attribute__((mode(TI)));
struct CONTEXT; struct OBJECT;
struct S { struct CONTEXT *a; struct OBJECT *b; int c, d; qword q; };
void MeasureJtJumpToTargetAtTime(struct CONTEXT *, struct OBJECT *, qword, float *, qword *, qword *, float);
#define FIELD_OFFSET(t,m) ((unsigned int)&(((t*)0)->m))
typedef char S_Size[sizeof(struct S)==32?1:-1];
typedef char S_Context_Offset[FIELD_OFFSET(struct S,a)==0?1:-1];
typedef char S_Object_Offset[FIELD_OFFSET(struct S,b)==4?1:-1];
typedef char S_Local_Point_Offset[FIELD_OFFSET(struct S,q)==16?1:-1];
/** @sly2-annot addr=0x19ABB8
 * Purpose: MinimizeRange callback over time: MeasureJtJumpToTargetAtTime(dt) cost
 * Info: proposed name GMeasureJumpTargetTime (T1); TU air
 * Notes: names_v2 v1 proposed OnJmtRemove__FP3JMT (0.001)
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/player.md#air-0x195950-0x19cbf8
 * @end-sly2-annot */
float GMeasureJumpTargetTime(void *record, float duration) { struct S *s = (struct S *)record; float f[4]; MeasureJtJumpToTargetAtTime(s->a, s->b, s->q, f, 0, 0, duration); return f[0]; }

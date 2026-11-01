typedef int qword __attribute__((mode(TI)));
struct O { int a; int b; };
struct S { char pad[0x1900]; struct O *p; char pad2[0x1930-0x1904]; qword a; qword b; };
void func_001FAC30(int, qword);
void func_001FAC98(int, qword);
/** @sly2-annot addr=0x1904E0
 * Purpose: Callback: AddSoVelocity/AddSoAngularVelocity of the stored force pair to the hang target SO
 * Info: proposed name AddJtExternalAccelerations (T1); class JT slot107; TU hang
 * Notes: names_v2 v1 proposed FUN_001cedf8 (0.000)
 * Confidence: mid (draft) [T1 0.65] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/player.md#hang-0x18f908-0x191090
 * @end-sly2-annot */
void func_001904E0(struct S *s) { func_001FAC30(s->p->b, s->a); func_001FAC98(s->p->b, s->b); }

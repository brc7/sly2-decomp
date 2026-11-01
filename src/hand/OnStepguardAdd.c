extern float D_002A8868;
struct DL { void *a, *b; int c; };
extern struct DL D_003E1444;
struct S { char pad[0xFF8]; float t; };
void OnSoAdd(struct S *p);
void AppendDlEntry__FP2DLPv(struct DL *pdl, void *pv);
/** @sly2-annot addr=0x2079E0
 * Purpose: STEPGUARD vt23: base (0x1F3EA8), append to guard list D_003E1444, +0xFF8 = FLT_MAX
 * Info: proposed name OnStepguardAdd (T1); class STEPGUARD slot23; TU stepguard
 * Notes: slot 23 OnAdd resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world, incl.
 *   proximity activation) (was OnStepguardAddToWorld)
 * Confidence: high [T1 0.85] - evidence: arbitration; sources: arbitration_slots22_23
 * See: docs/engine/guards-ai.md#1-stepguard-stepguardcpp-0x205e70-0x20b840-and-its-subclasses,
 *   docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void OnStepguardAdd(struct S *p) { OnSoAdd(p); AppendDlEntry__FP2DLPv(&D_003E1444, p); p->t = D_002A8868; }

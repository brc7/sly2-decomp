extern float D_002A8868;
struct DL { void *a, *b; int c; };
extern struct DL D_003E1444;
struct S { char pad[0xFF8]; float t; };
void func_001F3EA8(struct S *p);
void func_0011A220(struct DL *pdl, void *pv);
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
void func_002079E0(struct S *p) { func_001F3EA8(p); func_0011A220(&D_003E1444, p); p->t = D_002A8868; }

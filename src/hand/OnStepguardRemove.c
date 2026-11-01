struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
struct DL { void *a, *b; int c; };
extern struct DL D_003E1444;
struct S { char pad[0xFF8]; float t; };
void OnSoRemove(struct S *p);
void RemoveDlEntry__FP2DLPv(struct DL *pdl, void *pv);
/** @sly2-annot addr=0x2079A0
 * Purpose: STEPGUARD vt22: base (0x1F3E00), unlink from global guard list D_003E1444, stamp +0xFF8
 * Info: proposed name OnStepguardRemove (T1); class STEPGUARD slot22; TU stepguard
 * Notes: slot 22 OnRemove resolved by arbitration (OnRemove/OnAdd = object leaves/enters the active world,
 *   incl. proximity activation) (was OnStepguardRemoveFromWorld)
 * Confidence: high [T1 0.85] - evidence: arbitration; sources: arbitration_slots22_23
 * See: docs/engine/guards-ai.md#1-stepguard-stepguardcpp-0x205e70-0x20b840-and-its-subclasses,
 *   docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void OnStepguardRemove(struct S *p) { OnSoRemove(p); RemoveDlEntry__FP2DLPv(&D_003E1444, p); p->t = D_002DDED0.t; }

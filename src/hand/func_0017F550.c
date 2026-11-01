struct S { char pad[0x194]; void *p194; void *p198; };
int FAsegaRetracted(void *p);
int func_001F07E8(void *p);
int GetSmaGoal(void *p);
/** @sly2-annot addr=0x17F550
 * Purpose: True when the current ASEGA retracted or the SM reached its end state
 * Info: proposed name FCid47AnimDone (T2); class CID47; TU tu_17F090
 * Notes: names_v2 v1 proposed DoStepguardFreefallLanding__FP9STEPGUARD (0.030)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
int func_0017F550(struct S *p) { if (p->p194) return FAsegaRetracted(p->p194); { int f = 1; if (p->p198) f = func_001F07E8(p->p198) == GetSmaGoal(p->p198); return f; } }

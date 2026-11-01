struct O { char pad[0x58]; int n; };
int func_00209A68(void *p, int n);
struct O *func_00205458(void *p, int oid);
/** @sly2-annot addr=0x20A010
 * Purpose: BHV177 +0x58 while stunned (flag 0x17), else 0
 * Info: proposed name GetStepguardStunField (T2); TU stepguard
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int func_0020A010(void *p) { if (func_00209A68(p, 0x17)) return func_00205458(p, 0xB1)->n; return 0; }

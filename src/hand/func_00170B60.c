struct K { int a, b; };
struct R { int x, y; struct K k; };
struct R *func_00170E00(void *p, int a, int b);
/** @sly2-annot addr=0x170B60
 * Purpose: Splice: append command kind 1 with an 8-byte payload (copied to guard +0x2CC on execute)
 * Info: proposed name AddCid113CmdSetK2CC (T2); class CID113; TU tu_1709A8
 * Notes: Cid71->Cid113 (CID 0x71=113, agent-default(hex))
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
void func_00170B60(void *p, struct K *pk) { func_00170E00(p, 1, -1)->k = *pk; }

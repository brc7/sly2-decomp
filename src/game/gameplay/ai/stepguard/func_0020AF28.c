struct S { char pad[0x7C]; int a, b, c; };
/** @sly2-annot addr=0x20AF28
 * Purpose: +0x7C/+0x80/+0x84
 * Info: proposed name SetCid120AttackOids (T2); class CID120; TU stepguard
 * Notes: Cid78->Cid120 (CID 0x78=120, agent-default(hex))
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020AF28(struct S *p, int a, int b, int c) { p->a = a; p->b = b; p->c = c; }

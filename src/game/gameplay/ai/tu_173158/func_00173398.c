struct S { char pad[0x90]; int c; int *a; };
/** @sly2-annot addr=0x173398
 * Purpose: Appends a guard template object (max 8) at +0x90/+0x94
 * Info: proposed name AddCid119Template (T2); class CID119; TU tu_173158
 * Notes: Cid77->Cid119 (CID 0x77=119, agent-default(hex))
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_173158-0x173158-0x174898
 * @end-sly2-annot */
void func_00173398(struct S *p, int x) { if (p->c < 8) { p->a[p->c++] = x; } }

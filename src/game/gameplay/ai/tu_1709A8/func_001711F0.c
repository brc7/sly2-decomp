struct N { int k; int id; int pad[4]; struct N *next; };
struct S { char pad[0x50]; struct N *first; };
/** @sly2-annot addr=0x1711F0
 * Purpose: Finds the kind-9 (label) command with the given id
 * Info: proposed name FindCid113Label (T2); class CID113; TU tu_1709A8
 * Notes: Cid71->Cid113 (CID 0x71=113, agent-default(hex))
 * Confidence: low (draft) [T2 0.50] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
struct N *func_001711F0(struct S *p, int id) { int unused[4]; struct N *n; for (n = p->first; n; n = n->next) if (n->k == 9 && n->id == id) break; return n; }

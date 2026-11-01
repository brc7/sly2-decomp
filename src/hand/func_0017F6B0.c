struct K { short x; short y; int i; };
extern struct K D_002A5638;
extern char D_003DD490[];
struct S { char pad[0x180]; int n180; int n184; int pad2; int n18C; };
void func_00149B58(void *g, int a, void *p, struct K k, int n);
/** @sly2-annot addr=0x17F6B0
 * Purpose: Health = max; cancel pending dying callback (ClearSwCallbacks D_002A5638)
 * Info: proposed name ResetHack (T2); class HACK slot83; TU tu_17F090
 * Notes: Cid47->Cid71 (CID 0x47=71, vtable) | verifier PLAUSIBLE: Placeholder class. +0x184 = +0x180 (hp =
 *   max: class vtable has FCanPoBeHealed/AddPoHpMp), and if +0x18C ClearSwCallbacks(g_sw,1,p,D_002A5638).
 *   Called by 4 subclass Reset overrides. | v3 class identity: ResetCid71 -> ResetHack | T2 forced: v3:
 *   coined class word (Hack) without descriptor/string backing
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources:
 *   r4b_rest+verdicts_followups+class_review(master_names_v2_1)
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_0017F6B0(struct S *p) { p->n184 = p->n180; if (p->n18C) func_00149B58(D_003DD490, 1, p, D_002A5638, 0); }

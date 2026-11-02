struct S { char pad[0x4A4]; int f; };
void InitStepguard(struct S *p);
/** @sly2-annot addr=0x1CBEB0
 * Purpose: CID 61 vt7: InitStepguard + one field
 * Info: proposed name InitRajan (T1); class RAJAN slot7; TU tu_1CBEB0
 * Notes: class identity from whole_game WE names/object strings: InitCid61Guard -> InitRajan
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r5b_rest+whole_game(class identity)
 * See: docs/engine/bosses.md#32-rajan-m3_boss-l12-temple-exterior, docs/engine/guards-ai.md#8-source-files,
 *   docs/engine/player.md#10-source-files
 * @end-sly2-annot */
void InitRajan(struct S *p) { InitStepguard(p); p->f = 1; }

struct S { char pad[0x4A4]; int f; };
void func_00205EB8(struct S *p);
/** @sly2-annot addr=0x1CBEB0
 * Purpose: CID 61 vt7: InitStepguard + one field
 * Info: proposed name InitRajan (T1); class RAJAN slot7; TU tu_1CBEB0
 * Notes: class identity from whole_game WE names/object strings: InitCid61Guard -> InitRajan
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r5b_rest+whole_game(class identity)
 * See: docs/engine/guards-ai.md#8-source-files, docs/engine/player.md#10-source-files,
 *   docs/reference/functions/guards-ai.md#tu_1cbeb0-0x1cbeb0-0x1cc158
 * @end-sly2-annot */
void func_001CBEB0(struct S *p) { func_00205EB8(p); p->f = 1; }

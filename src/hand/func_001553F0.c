typedef struct MSG { int cb; int nKind; } MSG;
void *func_001AE2A0(MSG *pmsg);
static inline void *PvAllocMsg(MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE2A0(pmsg); }
struct S { char pad[0x1014]; void *pv; int n; };
void func_002039C0(struct S *p, void *pv);
/** @sly2-annot addr=0x1553F0
 * Purpose: cid 0x3B (STEPGUARD subclass) LoadFromBrx: allocates 0x480 bytes for route records, STEPGUARD
 *   load, sets +0x1018 = INT_MAX
 * Info: proposed name LoadDimitriFromBrx (T1); class DIMITRI slot14; TU tu_1553F0
 * Notes: Cid3B->Cid59 (CID 0x3B=59, hex-literal) | level_parser CONFIRMED-parse: payload layout followed by
 *   the parser, 100%% of bytes in 4 levels | class identity from whole_game WE names/object strings:
 *   LoadCid59FromBrx -> LoadDimitriFromBrx
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources:
 *   r3b_rest+verdicts_tu_r3r5+parse-verified+whole_game(class identity)
 * See: docs/engine/game-progression.md#7-source-files, docs/engine/guards-ai.md#8-source-files,
 *   docs/engine/guards-ai.md#9-key-functions
 * @end-sly2-annot */
void func_001553F0(struct S *p, void *pv)
{
    MSG msg;
    p->pv = PvAllocMsg(&msg, 0x480, 0x10);
    func_002039C0(p, pv);
    p->n = 0x7FFFFFFF;
}

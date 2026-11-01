struct MSG { int cb; int nKind; };
class FRAME { public: int a[4]; FRAME(); ~FRAME(); };
struct S { char pad[0x198]; int c; void *pv; };
extern "C" void *func_001AE378(MSG *pmsg);
extern "C" void *func_001AE238(MSG *pmsg, void *pv);
extern "C" void func_001F31E0(S *p, void *q);
static inline void *PvAllocMsg(MSG *pmsg, int cb, int nKind) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE378(pmsg); }
static inline void *PvReallocMsg(MSG *pmsg, int cb, int nKind, void *pv) { pmsg->cb = cb; pmsg->nKind = nKind; return func_001AE238(pmsg, pv); }
/** @sly2-annot addr=0x1DE478
 * Purpose: LASEN::LoadFromBrx (vt14): temp-allocate 0x5400 (64 beams x 0x150), LoadSoFromBrx (Splice adds
 *   beams), shrink to count
 * Info: proposed name LoadLasenFromBrx (T1); class LASEN slot14; TU sensor
 * Notes: superseded r6_1D5150 proposal: LoadLasenFromBrx? (0.3) | names_v2 C proposed
 *   SetLightDirection__FP5LIGHTP6VECTOR (0.638) | level_parser CONFIRMED-parse: payload layout followed by
 *   the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static naming pass; sources: r6b_rest+parse-verified
 * See: docs/engine/gameplay-objects.md#key-functions, docs/engine/guards-ai.md#5-perception,
 *   docs/formats/level-objects.md#5-per-class-payloads-vtable-slot-14
 * @end-sly2-annot */
extern "C" void func_001DE478(S *p, void *q)
{
    FRAME frame;
    MSG msg;
    p->pv = PvAllocMsg(&msg, 0x5400, 0x10);
    func_001F31E0(p, q);
    p->pv = PvReallocMsg(&msg, p->c * 0x150, 0x10, p->pv);
}

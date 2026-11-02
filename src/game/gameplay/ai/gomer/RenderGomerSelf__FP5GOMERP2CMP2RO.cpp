struct GOMER { char pad[0xE78]; int fE78; };
struct CM;
struct RO;
extern "C" void InitStepguard(GOMER *pgomer, CM *pcm, RO *pro);
/** @sly2-annot addr=0x16F148
 * Purpose: GOMER::Init: calls STEPGUARD::Init (0x205EB8) then sets a GOMER flag at +0xE78
 * Info: proposed name InitGomer__FP5GOMER (EXISTING); class GOMER slot7; TU gomer
 * Notes: agent/verifier proposes InitGomer for this existing name (see existing_name_changes.csv) | RENAME
 *   proposed -> InitGomer__FP5GOMER (existing_name_changes.csv)
 * Confidence: high [EXISTING 0.85] - evidence: arbitration; pre-existing symbol in symbol_addrs.txt; sources:
 *   r3_1539A8+arbitration
 * See: docs/engine/guards-ai.md#8-source-files, docs/engine/guards-ai.md#9-key-functions,
 *   docs/engine/rendering.md#12-source-files
 * @end-sly2-annot */
void RenderGomerSelf(GOMER *pgomer, CM *pcm, RO *pro)
{
    InitStepguard(pgomer, pcm, pro);
    pgomer->fE78 = 1;
}

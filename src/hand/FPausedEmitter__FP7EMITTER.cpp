struct CLOCK { float t; float dt; };
extern CLOCK D_002DDED0;
struct EMITTER { char pad[0xF0]; int k; int c; char pad2[0x148 - 0xF8]; float tUnpause; };
/** @sly2-annot addr=0x15A170
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
int FPausedEmitter(EMITTER *pemitter)
{
    if (D_002DDED0.t < pemitter->tUnpause)
        return 1;
    switch (pemitter->k) {
    case 0:
    case 1:
        if (pemitter->c == 0)
            return 1;
    }
    return 0;
}

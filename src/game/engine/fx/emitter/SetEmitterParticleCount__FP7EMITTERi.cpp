struct EMITTER { char pad[0xE0]; int fDirty; char pad2[0xC]; int k; int c; };
void PauseEmitterIndefinite(EMITTER *pemitter);
void UnpauseEmitter(EMITTER *pemitter);
/** @sly2-annot addr=0x15A240
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/particles-effects.md#emitter-0x159118-0x15cd68
 * @end-sly2-annot */
void SetEmitterParticleCount(EMITTER *pemitter, int c)
{
    switch (pemitter->k) {
    case 0:
        if (c == 0)
            PauseEmitterIndefinite(pemitter);
        else if (c == -1)
            UnpauseEmitter(pemitter);
        else
            pemitter->c = c;
        break;
    case -1:
        pemitter->c = c;
        break;
    }
    pemitter->fDirty = 1;
}

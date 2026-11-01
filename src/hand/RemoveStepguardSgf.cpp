class OBase { public: int a, b, c; virtual void V1(); virtual void V2(); virtual void Msg(int msg, int a, int c, void **apv, int x); };
class O : public OBase { public: char pad[0x2C0 - 0x10]; unsigned long fl; };
extern "C" unsigned long GrfFromSgf(int n);
extern "C" void SetStepguardGrfsgf(O *p, unsigned long fl);
/** @sly2-annot addr=0x209970
 * Purpose: Clear guard flag n, message 7 (-1,n)
 * Info: proposed name RemoveStepguardSgf (T1); TU stepguard
 * Notes: runtime: guard flag word +0x2C0 == union of BHV-raised flags in every state
 * Confidence: high [T1 0.85] - evidence: runtime (save-state RAM); sources: r7b_guard+runtime
 * See: docs/engine/guards-ai.md#2-the-guard-flag-word-0x2c0, docs/engine/guards-ai.md#9-key-functions,
 *   docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
extern "C" void RemoveStepguardSgf(O *p, int n)
{
    int m;
    void *apv[2];
    SetStepguardGrfsgf(p, p->fl & ~GrfFromSgf(n));
    m = -1;
    apv[0] = &m;
    apv[1] = &n;
    p->Msg(7, 0, 2, apv, 0);
}

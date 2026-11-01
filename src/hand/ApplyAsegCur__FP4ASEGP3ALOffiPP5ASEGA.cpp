struct ASEG; struct ALO;
struct VTE { short d; short pad; void (*pfn)(); };
struct ASEGA { int pad[3]; VTE *vt; };
void func_00130658(ASEGA **);
void ApplyAseg(ASEG *paseg, ALO *paloRoot, float t, float svt, int grfapl, ASEGA **ppasega);
/** @sly2-annot addr=0x1305B8
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/engine/animation.md#1-aseg-and-asega-asegcpp-0x12f248-asegacpp-0x1308a8,
 *   docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#aseg-0x12f248-0x1308a8
 * @end-sly2-annot */
void ApplyAsegCur(ASEG *paseg, ALO *paloRoot, float t, float svt, int grfapl, ASEGA **ppasega)
{
    func_00130658(ppasega);
    if (paseg)
    {
        ApplyAseg(paseg, paloRoot, t, svt, grfapl, ppasega);
        ASEGA *pasega = *ppasega;
        ((void (*)(void *))pasega->vt[1].pfn)((char *)pasega + pasega->vt[1].d);
    }
}

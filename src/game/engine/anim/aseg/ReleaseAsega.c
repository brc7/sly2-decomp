struct VTE { short d; short pad; void (*pfn)(); };
struct O { int pad[3]; struct VTE *vt; };
void func_00130B18(struct O *);
/** @sly2-annot addr=0x130658
 * Purpose: If *ppasega: RetractAsega(*ppasega), drop the reference (vt slot2 Decref), *ppasega=NULL
 * Info: proposed name ReleaseAsega (T1); TU aseg
 * Notes: v3: superseded RetractAsegaRef | top50_round (0.60): Body. Same owning-pointer release pattern as
 *   ReleaseAct; vt slot 2 is Decref (DecrefAsega 0x1308B8 family). Callers own an ASEGA handle for a state
 *   (SetBentleyState, PlayStepplayerScriptAnim, SetJtHurtState, SetMurrayCar
 * Confidence: mid (draft) [T1 0.60] - evidence: semantic review; sources: r1_116918+top50_round
 * See: docs/reference/functions/animation.md#aseg-0x12f248-0x1308a8,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void ReleaseAsega(struct O **pp) {
    if (*pp) {
        struct O *p; struct VTE *vt;
        func_00130B18(*pp);
        p = *pp; vt = p->vt; ((void (*)(void *))vt[2].pfn)((char *)p + vt[2].d); *pp = 0;
    }
}

struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; };
void RunBtnAsegs(void *p, int n);
/** @sly2-annot addr=0x148860
 * Purpose: Fire Splice event 6, run the 'off' asegs (RunBtnAsegs(1))
 * Info: proposed name UntriggerBtn (T1); TU button
 * Notes: names_v2 C proposed FFoundAshAseg__FP3ASHP4ASEG (0.638)
 * Confidence: mid (draft) [T1 0.70] - evidence: static naming pass; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void UntriggerBtn(struct O **p)
{
    struct O *o = *p;
    struct VTE *e = &o->vt[3];
    ((void (*)(void *, int, int, int, int, int))e->pfn)((char *)o + e->d, 6, 0, 0, 0, 0);
    RunBtnAsegs(p, 1);
    {
        struct O *o2 = *p;
        struct VTE *e2 = &o2->vt[4];
        ((void (*)(void *, int, void *))e2->pfn)((char *)o2 + e2->d, 7, p);
    }
}

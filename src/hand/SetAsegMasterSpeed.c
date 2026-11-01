struct ASEGA { char pad[0x2c]; struct ASEGA *pnext; };
struct S { char pad[0x50]; struct ASEGA *pfirst; char pad2[0x24]; float f78; };
void SetAsegaMasterSpeed(struct ASEGA *, float);
/** @sly2-annot addr=0x1306F0
 * Purpose: SetAsegaMasterSpeed on every application of the ASEG
 * Info: proposed name SetAsegMasterSpeed (T1); TU aseg
 * Notes: names_v2 v1 proposed PasegaNew__FP2SW (0.004)
 * Confidence: mid (draft) [T1 0.55] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/animation.md#aseg-0x12f248-0x1308a8
 * @end-sly2-annot */
void SetAsegMasterSpeed(struct S *p, float g) {
    struct { void *a, *b; int c; void *d; } dli;
    struct ASEGA *pa;
    for (pa = p->pfirst; pa; pa = pa->pnext) SetAsegaMasterSpeed(pa, g);
    p->f78 = g;
}

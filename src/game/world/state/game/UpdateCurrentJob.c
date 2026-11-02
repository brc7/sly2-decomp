struct X { char pad[0x64]; void *f64; void *f68; };
struct G { char pad[0x3B0C]; struct X *f3b0c; };
extern struct G D_003DD490;
int CploFindSwObjectsByClass(struct G *pg, int kind, int n, int m, int c, void **apv);
int FTaskComplete(void *p);
void EndCurrentJob(struct X *p);
void SetCurrentJob(struct X *p);
/** @sly2-annot addr=0x1649D0
 * Purpose: Picks the job whose start task is complete and end task is not, and switches the current job to it
 * Info: proposed name UpdateCurrentJob (T1); TU game
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
void UpdateCurrentJob(void)
{
    struct X *apx[256];
    struct X *pxFound = 0;
    int c = CploFindSwObjectsByClass(&D_003DD490, 5, 0x84, 0, 0x100, (void **)apx);
    int i;
    for (i = 0; i < c; i++) {
        struct X *px = apx[i];
        if (FTaskComplete(px->f64) && !FTaskComplete(px->f68))
            pxFound = px;
    }
    if (pxFound != D_003DD490.f3b0c) {
        if (D_003DD490.f3b0c)
            EndCurrentJob(D_003DD490.f3b0c);
        if (pxFound)
            SetCurrentJob(pxFound);
    }
}

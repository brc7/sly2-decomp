struct O {
    char pad[0x10];
    struct O *pParent;
    void *pv14;
    char pad2[0x10];
    unsigned long b0 : 52;
    long n52 : 4;
    unsigned int b56 : 4;
    unsigned int f60 : 1;
};
extern char D_003DD490[];
void func_00147E88(void *, void *);
/** @sly2-annot addr=0x1A6CA8
 * Purpose: Set the LO busy kind (LO+0x28 bits 52-55: 0 busy within sBusy of the camera, 1 always busy, 2
 *   never busy, 3 busy within sBusy of the hero) and re-sort the root in the SW busy list if it is in the
 *   world
 * Info: proposed name SetLoBusyk (T1); TU lo
 * Notes: top50_round (0.80): UpdateLoBusy 0x148300 switch on the same nibble: case1 fIn=1, case2 fIn=0, case0
 *   dist(PosLoWorld,camera pos D_002DDF40)<sBusy(+0x38), case3 dist to PpoHero. InitLo sets 2, InitSo 0,
 *   InitTarget/InitGrabspot 3, relays 1 wh
 * Confidence: high [T1 0.80] - evidence: semantic review; sources: r4_17F090+top50_round
 * See: docs/engine/cutscenes.md#5-key-functions, docs/engine/guards-ai.md#9-key-functions,
 *   docs/engine/object-model.md#9-key-functions
 * @end-sly2-annot */
void func_001A6CA8(struct O *p, int n)
{
    if (n != p->n52) {
        p->n52 = n;
        if (!p->pParent && p->f60 != 0)
            func_00147E88(D_003DD490, p->pv14);
    }
}

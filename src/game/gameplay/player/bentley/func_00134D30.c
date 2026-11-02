struct S { char pad[0x150]; void *f150; char pad2[0xf00 - 0x154]; int ff00; int ff04; char pad3[0x18]; int ff20; };
int FStepplayerInJtmode(struct S *);
int GrfConsumePressed__5POCTLUif(void *, int, float);
void EndJtMode(void *);
void UpdateJtMode(void *);
/** @sly2-annot addr=0x134D30
 * Purpose: While in jtmode: on button mask 0x4F0 in mode 1/kind 0 call 0x19CCE8, then update jtmode 0x19D330
 * Info: proposed name UpdateBentleyJtmode (T2); TU bentley
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#bentley-0x134648-0x1378d8
 * @end-sly2-annot */
void func_00134D30(struct S *p) {
    if (FStepplayerInJtmode(p)) {
        void *pv = &p->ff00;
        if (p->ff04 == 1 && p->ff20 == 0 && GrfConsumePressed__5POCTLUif(p->f150, 0x4f0, 0.25f)) EndJtMode(pv);
        UpdateJtMode(pv);
    }
}

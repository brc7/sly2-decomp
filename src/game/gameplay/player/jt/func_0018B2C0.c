struct S { char pad[0x150]; void *p150; char p1[0xDAC]; int xf00; int xf04; char p2[0x18]; int xf20; char p3[0xE1C]; int x1d40; };
int FStepplayerInJtmode(struct S *p);
int GrfConsumePressed__5POCTLUif(void *p, int n, float g);
void EndJtMode(void *p);
void UpdateJtMode(void *p);
/** @sly2-annot addr=0x18B2C0
 * Purpose: If player can act (0x1443D8) and POCTL button 0x410/0x4F0 pressed toggle JTMODE at x0F00
 *   (0x19CCE8), then update it (0x19D330)
 * Info: proposed name UpdateJtModeInput (T2); TU jt
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void func_0018B2C0(struct S *p)
{
    if (FStepplayerInJtmode(p)) {
        if (p->xf04 == 1 && !p->xf20 &&
            GrfConsumePressed__5POCTLUif(p->p150, p->x1d40 == 7 ? 0x410 : 0x4F0, 0.25f))
            EndJtMode(&p->xf00);
        UpdateJtMode(&p->xf00);
    }
}

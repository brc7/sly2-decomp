/* only diff: hazard nop placement (target: no nop after mfc1 before beqz, nop after mtc1); assembler-level */
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
int MpStepplayerCur(void *p);
void SetStepplayerMp(void *p, int n);
struct P { char pad[0x1308]; float g; };
/** @sly2-annot addr=0x1441E8
 * Purpose: Accumulate rate*dt in gMpFrac (+0x1308) and subtract whole units from MP (floored at 0)
 * Info: proposed name DrainStepplayerMp (T1); TU stepplayer
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/player.md#5-sly-jt-jtcpp-0x188488--component-tus,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void DrainStepplayerMp(struct P *p, float f)
{
    int n;
    p->g += f * D_002DDED0.dt;
    n = (int)p->g;
    if (n) {
        int x;
        p->g -= (float)n;
        x = MpStepplayerCur(p) - n;
        SetStepplayerMp(p, x > -1 ? x : 0);
    }
}

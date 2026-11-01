/* only diff: hazard nop placement (target: no nop after mfc1 before beqz, nop after mtc1); assembler-level */
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
int MpStepplayerCur(void *p);
void SetStepplayerMp(void *p, int n);
struct P { char pad[0x1308]; float g; };
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

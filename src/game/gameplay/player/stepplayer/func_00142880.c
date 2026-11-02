struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
float GRandInRange__Fff(float a, float b);
struct X { char pad[0x1138]; float a0, b0, a1, b1; int f; float tNext; };
/** @sly2-annot addr=0x142880
 * Purpose: tNext = now + GRandInRange(min,max) choosing range set by flag
 * Info: proposed name ScheduleIdleTimer (T2); TU stepplayer
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00142880(struct X *p)
{
    float t;
    if (p->f) {
        t = D_002DDED0.t;
        t += GRandInRange__Fff(p->a0, p->b0);
        p->tNext = t;
    } else {
        t = D_002DDED0.t;
        t += GRandInRange__Fff(p->a1, p->b1);
        p->tNext = t;
    }
}

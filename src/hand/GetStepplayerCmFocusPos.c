typedef int qword __attribute__((mode(TI)));
struct G { char pad[0x260]; void *p260; char pad2[0x300 - 0x264]; int n300; };
extern struct G D_002DDF40;
struct V { float x, y, z; } __attribute__((aligned(16)));
void func_0019E9D8(void *p, struct V *v);
qword func_001A62E8(void *p);
/** @sly2-annot addr=0x141588
 * Purpose: vt97: camera focus point = pipe hand pos if on pipe, else world pos with z from jump start/ground
 *   height + 0x1048 offset (or smoothed +0x104C when the camera follows this player)
 * Info: proposed name GetStepplayerCmFocusPos (T1); class STEPPLAYER slot97; TU stepplayer
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638,
 *   docs/reference/functions/player.md#jt-0x188488-0x18e540
 * @end-sly2-annot */
void GetStepplayerCmFocusPos(char *p, struct V *v)
{
    float z;
    if (*(int *)(p + 0xF54) != -1) {
        func_0019E9D8(p + 0xF50, v);
        return;
    }
    *(qword *)v = func_001A62E8(p);
    if (*(int *)(p + 0x208)) {
        z = *(float *)(p + 0x238) + *(float *)(p + 0x1048);
    } else if (*(int *)(p + 0x2DC) == 0x14 || D_002DDF40.n300 != 0 || D_002DDF40.p260 != p) {
        v->z += *(float *)(p + 0x248) + *(float *)(p + 0x1048);
        return;
    } else {
        z = *(float *)(p + 0x104C);
    }
    v->z = z;
}

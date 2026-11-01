struct CLOCK { float t; };
extern struct CLOCK D_002DDED0;
struct O { char pad[0xFBC]; int xfbc; };
struct S { char pad[0x4748]; int x4748; float x474c; };
int func_0015E9C8(struct S *, int, int, int, int, struct O **);
void func_00207030(struct O *, int);
/** @sly2-annot addr=0x19F170
 * Purpose: Set/clear the override flag with timestamp and put every STEPGUARD (CID 0x38, max 32) into guard
 *   state 0xE, or back to -1 if it was in 0xE
 * Info: proposed name SetSwGuardOverride (T2); TU tu_19F0A8
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#tu_19f0a8-0x19f0a8-0x19f3e8
 * @end-sly2-annot */
void func_0019F170(struct S *p, int f) {
    struct O *ap[32];
    int c, i;
    p->x4748 = f;
    p->x474c = D_002DDED0.t;
    c = func_0015E9C8(p, 0x105, 0x38, 0, 32, ap);
    for (i = 0; i < c; i++) {
        if (f) func_00207030(ap[i], 0xE);
        else if (ap[i]->xfbc == 0xE) func_00207030(ap[i], -1);
    }
}

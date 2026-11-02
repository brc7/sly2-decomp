struct O { char pad[0x12F0]; int x12f0; };
struct S { struct O *p; };
extern int D_002DFBC0;
extern char D_002DE2E0[];
int FActiveCplcy__FP5CPLCY(void *);
/** @sly2-annot addr=0x19EEF0
 * Purpose: Gadget check while on a pipe: only k==3, allowed if owner flag x12F0 and global flag and no active
 *   camera policy
 * Info: proposed name FJtPipeAllowsGadget (T2); TU pipe_jt
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r4b_rest
 * See: docs/reference/functions/player.md#pipe_jt-0x19de10-0x19f0a8
 * @end-sly2-annot */
int func_0019EEF0(struct S *p, int k) {
    if (k == 3) {
        int r = 0;
        if (p->p->x12f0 && (D_002DFBC0 & 2)) r = !FActiveCplcy__FP5CPLCY(D_002DE2E0);
        return r;
    }
    return 0;
}

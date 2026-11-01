extern char D_003DD040[];
extern char D_003DD490[];
struct S { char pad[0x2A0]; int a[6]; };
int func_001FD0C0(void *p, int i);
void *PloFindSwObject__FP2SWi3OIDP2LO(void *p, int a, int b, struct S *ps);
void *func_001F03C8(void *x, struct S *ps);
void PsmaApplySmOut(void *x, struct S *ps, int a, int b, void **ppv);
/** @sly2-annot addr=0x205478
 * Purpose: STEP vt83: for each set bit i<6 of mask, ++refcount +0x2A0[i]; on first ref ensure an SMA of the
 *   dialog-role SM (OidDialogSmRole 0x1FD0C0) on this step
 * Info: proposed name AddStepDialogSmRefs (T2); class STEP slot83; TU step
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#step-0x203948-0x205e70,
 *   docs/reference/functions/player.md#stepplayer-0x13ff70-0x144638
 * @end-sly2-annot */
void func_00205478(struct S *p, int mask)
{
    int i;
    for (i = 0; i < 6; i++) {
        int f = (unsigned int)i < 6;
        int bit = 1 << i;
        if (!f) bit = 0;
        if (mask & bit) {
            if (++p->a[i] < 2) {
                int id = func_001FD0C0(D_003DD040, i);
                void *x = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, id, p);
                if (x) {
                    void *y = func_001F03C8(x, p);
                    if (!y)
                        PsmaApplySmOut(x, p, -1, 9, &y);
                }
            }
        }
    }
}

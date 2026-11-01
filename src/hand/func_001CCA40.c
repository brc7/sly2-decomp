struct O { char pad[0xE0]; int grf; };
struct S { char pad[0x1000]; int grf; };
extern char D_003DD490[];
int func_0015E9C8(void *p, int a, int b, int c, int d, struct O **apo);
/** @sly2-annot addr=0x1CCA40
 * Purpose: First nearby object of class 0x14 whose flags match the hands' grf
 * Info: proposed name PloFindJtHandsTarget (T2); TU jthands
 * Notes: names_v2 C proposed AdjustCmJoy__FP2CMP3JOY5JOYIDPf (0.438)
 * Confidence: low (draft) [T2 0.40] - evidence: static naming pass; sources: r5b_rest
 * See: docs/reference/functions/player.md#jthands-0x1cc158-0x1cd5f0
 * @end-sly2-annot */
struct O *func_001CCA40(struct S *p, int n)
{
    struct O *apo[4];
    int c = func_0015E9C8(D_003DD490, 0x401, 0x14, n, 4, apo);
    int i;
    for (i = 0; i < c; i++) {
        if (apo[i]->grf & p->grf) return apo[i];
    }
    return 0;
}

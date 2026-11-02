struct E { int k; int a[6]; };
struct L { unsigned int c; struct E ae[16]; };
extern struct E D_002E1E68;
void func_002333B4(void *, int, int);
/** @sly2-annot addr=0x1A4FB0
 * Purpose: Append a zeroed step of kind k to the 16-entry queue (overflow returns dummy D_002E1E68)
 * Info: proposed name PbootstepAdd (T2); TU tu_1A4E08
 * Notes: names_v2 C proposed ProcessSwCallbacks__FP2SW (0.200)
 * Confidence: low (draft) [T2 0.35] - evidence: static naming pass; sources: r4b_rest
 * See: docs/reference/functions/world-sw.md#tu_1a4e08-0x1a4e08-0x1a5270
 * @end-sly2-annot */
struct E *func_001A4FB0(struct L *pl, int k)
{
    struct E *pe;
    if (pl->c >= 16)
        return &D_002E1E68;
    pe = &pl->ae[pl->c++];
    func_002333B4(pe, 0, sizeof(struct E));
    pe->k = k;
    return pe;
}

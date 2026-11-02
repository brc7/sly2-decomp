struct M { float a[16]; };
extern struct M D_002C44F0;
void func_002333B4(void *p, int c, int n);
/** @sly2-annot addr=0x1A6B30
 * Purpose: LO vt32: optionally write identity into M4 arg, return static D_002C44F0
 * Info: proposed name LO_PxfLocal (T2); class LO slot32; TU lo
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4_17F090
 * See: docs/reference/functions/object-model.md#lo-0x1a5270-0x1a7518
 * @end-sly2-annot */
struct M *func_001A6B30(void *unused, struct M *pm)
{
    if (pm) {
        func_002333B4(pm, 0, 0x40);
        pm->a[0] = pm->a[5] = pm->a[10] = pm->a[15] = 1.0f;
    }
    return &D_002C44F0;
}

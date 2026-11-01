int func_00178D00(void *p);
/** @sly2-annot addr=0x144EE8
 * Purpose: true if the child behaviour finished (0x178D00) or its state == 2
 * Info: proposed name FBhv193Done (T2); TU tu_144B28
 * Notes: names_v2 C proposed EnsureAloFader__FP3ALO (0.438)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r2b_rest
 * See: docs/reference/functions/player.md#tu_144b28-0x144b28-0x1471c0
 * @end-sly2-annot */
int func_00144EE8(char *p)
{
    char *q = *(char **)(p + 0x18);
    int r = 0;
    if (func_00178D00(q) || *(int *)(q + 0x4C) == 2) r = 1;
    return r;
}

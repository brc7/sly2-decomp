struct S { char pad[0x18]; int c; char pad2[0xE0]; int *a; int *ai; };
/** @sly2-annot addr=0x1FD028
 * Purpose: sfxid -> SNDMGR sound entry via aisnd index table (NULL if unmapped)
 * Info: proposed name PsndFromSfxid (T1); TU sound
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
int *func_001FD028(struct S *p, int n)
{
    int i;
    if (n < 0 || n >= p->c) return 0;
    i = p->ai[n];
    if (i < 0) return 0;
    return &p->a[i];
}

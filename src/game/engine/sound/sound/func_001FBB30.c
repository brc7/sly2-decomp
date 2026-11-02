struct VTE { short d; short pad; void (*pfn)(void *); };
struct A { char pad[0xC]; struct VTE *vt; };
void StopSnd(struct A *p, float g);
void SetSndFlags(struct A *p, int a, int b);
/** @sly2-annot addr=0x1FBB30
 * Purpose: IsStillPlaying callback: not playing -> StopSnd; else re-arm status query (0x400); Decref
 * Info: proposed name OnBanksndStatus (T2); TU sound
 * Confidence: low (draft) [T2 0.45] - evidence: static analysis; sources: r7_1F8DD8
 * See: docs/reference/functions/sound.md#sound-0x1faf70-0x1ff3b8
 * @end-sly2-annot */
void func_001FBB30(int f, long l)
{
    struct A *p = (struct A *)l;
    if (!f)
        StopSnd(p, 0.0f);
    else
        SetSndFlags(p, 0x400, 0x400);
    p->vt[2].pfn((char *)p + p->vt[2].d);
}

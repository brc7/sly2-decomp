struct K { int a; int b; };
extern struct K D_002A4790;
extern char D_003DD490[];
struct S { void *pframe; };
void func_001F4FB0(struct S *);
void *PrefFindBinding__6CFrameUii(void *, int, int);
void EnsureSwCallback(void *, void *, struct K, int);
/** @sly2-annot addr=0x132238
 * Purpose: BARRIER vt55 (XP created): SO base 0x1F4FB0, then if the object has a Splice binding for event 5,
 *   EnsureSwCallback(g_sw, this, &0x132298)
 * Info: proposed name NotifyBarrierImpact (T1); class BARRIER slot55; TU barrier
 * Confidence: high [T1 0.85] - evidence: static analysis; sources: r2b_rest+verify_side
 * See: docs/engine/gameplay-objects.md#gameplay-object-classes,
 *   docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#barrier-0x1320c8-0x132570
 * @end-sly2-annot */
void NotifyBarrierImpact(struct S *p) {
    func_001F4FB0(p);
    if (p->pframe && PrefFindBinding__6CFrameUii(p->pframe, 5, 1)) EnsureSwCallback(D_003DD490, p, D_002A4790, 0);
}

struct CMD { int kind; int b; char pad[0x48]; };
struct Q { char pad[0x50]; int c; struct CMD *a; int cMax; };
extern struct CMD D_003DA740;
void func_002333B4(void *p, int b, int cb);
/** @sly2-annot addr=0x1E01F8
 * Purpose: Append a zeroed 0x50-byte RELAYCMD {relayck, fAsync} to the relay (limit crelaycmdMax or 0x200);
 *   on overflow return a scratch command D_003DA740
 * Info: proposed name PrelaycmdAddRelay (T1); TU relay
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r6b_rest
 * See: docs/engine/cutscenes.md#5-key-functions,
 *   docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
struct CMD *func_001E01F8(struct Q *pq, int kind, int b)
{
    struct CMD *p;
    int cMax = pq->cMax;
    if (cMax <= -1) cMax = 0x200;
    if (pq->c >= cMax) return &D_003DA740;
    p = &pq->a[pq->c++];
    func_002333B4(p, 0, sizeof(struct CMD));
    p->kind = kind;
    p->b = b;
    return p;
}

typedef struct S { char pad[0x18]; int x18; } S;
extern struct G D_003DD490;
void *func_0015E938(struct G *g, int k, int id, int x);
int func_00132B10(void *p, int k);
void *func_001CB2E0(void *p);
/** @sly2-annot addr=0x1E1DB8
 * Purpose: Resolve an oid relative to the script context (ploContext +0x18); proxies (CID 6) resolve to their
 *   current object
 * Info: proposed name PloFindJsgObject (T1); TU relay
 * Confidence: mid (draft) [T1 0.55] - evidence: static analysis; sources: r6b_rest
 * See: docs/reference/functions/cutscenes.md#relay-0x1dfad8-0x1e5d80
 * @end-sly2-annot */
void *func_001E1DB8(S *p, int id)
{
    void *q;
    if (id == -1) {
        q = 0;
    } else {
        q = func_0015E938(&D_003DD490, 4, id, p->x18);
        if (q && func_00132B10(q, 6)) q = func_001CB2E0(q);
    }
    return q;
}

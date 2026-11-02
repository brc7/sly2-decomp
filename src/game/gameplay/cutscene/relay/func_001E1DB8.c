typedef struct S { char pad[0x18]; int x18; } S;
extern struct G D_003DD490;
void *PloFindSwObject__FP2SWi3OIDP2LO(struct G *g, int k, int id, int x);
int FIsBasicDerivedFrom__FP5BASIC3CID(void *p, int k);
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
        q = PloFindSwObject__FP2SWi3OIDP2LO(&D_003DD490, 4, id, p->x18);
        if (q && FIsBasicDerivedFrom__FP5BASIC3CID(q, 6)) q = func_001CB2E0(q);
    }
    return q;
}

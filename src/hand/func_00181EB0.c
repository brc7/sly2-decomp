struct R { char pad[0xC]; int c; char dl[0xC]; };
struct Q { char pad[0x20]; struct R *pr; int n; float t; };
struct S { char pad[0x28C]; char dl[0xC]; };
struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
extern char D_002DEB30[];
void func_0011A2E8(void *pdl, void *pv);
void func_0011A220(void *pdl, void *pv);
void func_001619A0(void *g, int n, struct S *p);
void func_00181B60(struct S *p, struct R *r);
/** @sly2-annot addr=0x181EB0
 * Purpose: Segment state switch: move between DLs, game event, relayout
 * Info: proposed name SetHackwallSegmentState (T2); class HACKWALL; TU tu_17F090
 * Notes: Cid4C->Cid76 (CID 0x4C=76, hex-literal) | class identity from whole_game WE names/object strings:
 *   SetCid76SegmentState -> SetHackwallSegmentState
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00181EB0(struct S *p, struct Q *q, int n)
{
    struct R *r;
    if (n == q->n) return;
    q->n = n;
    q->t = D_002DDED0.t;
    if (n != 2) return;
    r = q->pr;
    r->c--;
    func_0011A2E8(r->dl, q);
    func_0011A220(p->dl, q);
    if (!r->c) func_001619A0(D_002DEB30, 0x3A, p);
    func_00181B60(p, r);
}

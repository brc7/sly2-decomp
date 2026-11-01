struct DLE { void *next; void *prev; };
struct R { char pad[0x10]; char dl[0xC]; };
struct Q { char pad[0x20]; struct R *pr; char p1[0x3C]; struct DLE dle; };
struct E { int a; int b; int c; int d; char p[0xC]; int x1c; float g; int x24; int x28; float h; };
struct S { char pad[0x1C8]; int c; struct E ae[4]; struct Q *pqFirst; };
void func_0017F6B0(struct S *p);
void ClearDle__FP3DLE(struct DLE *pdle);
void AppendDlEntry__FP2DLPv(void *pdl, void *pv);
void ClearDl__FP2DL(void *pdl);
void func_00181A90(struct S *p, struct E *pe);
/** @sly2-annot addr=0x1819E0
 * Purpose: Reset all segments
 * Info: proposed name ResetHackwall (T2); class HACKWALL slot83; TU tu_17F090
 * Notes: names_v2 v1 proposed FDetectSgg__FP3SGG (0.638) | Cid4C->Cid76 (CID 0x4C=76, hex-literal) | class
 *   identity from whole_game WE names/object strings: ResetCid76 -> ResetHackwall
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r4b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_001819E0(struct S *p)
{
    struct Q *q;
    int i;
    func_0017F6B0(p);
    q = p->pqFirst;
    while (q) {
        struct Q *qNext = (struct Q *)q->dle.next;
        ClearDle__FP3DLE(&q->dle);
        AppendDlEntry__FP2DLPv(q->pr->dl, q);
        q = qNext;
    }
    ClearDl__FP2DL(&p->pqFirst);
    for (i = 0; i < p->c; i++) {
        struct E *pe = &p->ae[i];
        pe->d = pe->b;
        func_00181A90(p, pe);
    }
}

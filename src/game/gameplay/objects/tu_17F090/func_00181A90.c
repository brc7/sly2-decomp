struct Q { char pad[0x24]; int n; char p1[0x14]; float g3c; int x40; char p2[8]; int x4c; char p3[0x10]; struct Q *next; };
struct E { int a; int b; int c; int d; struct Q *pqFirst; void *pqLast; int ib; int x1c; float g; int x24; int x28; float h; };
struct S;
void func_00181B60(struct S *p, struct E *pe);
float SMaxShape(int n);
void func_00181EB0(struct S *p, struct Q *q, int n);
/** @sly2-annot addr=0x181A90
 * Purpose: Reset one segment's pieces along its shape
 * Info: proposed name ResetHackwallSegment (T2); class HACKWALL; TU tu_17F090
 * Notes: names_v2 v1 proposed FAbandonSggSearch__FP3SGG (0.200) | Cid4C->Cid76 (CID 0x4C=76, hex-literal) |
 *   class identity from whole_game WE names/object strings: ResetCid76Segment -> ResetHackwallSegment
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r4b_rest+whole_game(class
 *   identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00181A90(struct S *p, struct E *pe)
{
    int i = 0;
    float g;
    struct Q *q;
    func_00181B60(p, pe);
    g = SMaxShape(pe->x24);
    for (q = pe->pqFirst; q; q = q->next) {
        q->n = -1;
        func_00181EB0(p, q, 0);
        q->g3c = g * ((float)i + 0.5f) / (float)pe->d;
        q->x40 = 0;
        q->x4c = pe->x1c;
        i++;
    }
}

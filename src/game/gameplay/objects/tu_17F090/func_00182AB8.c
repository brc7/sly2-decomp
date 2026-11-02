struct E { int a; int b; int c; char p[0x10]; int x1c; float g; int p2[2]; float h; };
struct S { char pad[0x1C8]; unsigned int c; struct E ae[4]; };
extern int D_002BE8AC;
/** @sly2-annot addr=0x182AB8
 * Purpose: Append a segment record (max 4) {a,b,c,g}
 * Info: proposed name AddHackwallSegment (T2); class HACKWALL; TU tu_17F090
 * Notes: Cid4C->Cid76 (CID 0x4C=76, hex-literal) | class identity from whole_game WE names/object strings:
 *   AddCid76Segment -> AddHackwallSegment
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00182AB8(struct S *p, int a, int b, int c, float g)
{
    if (p->c < 4) {
        struct E *pe = &p->ae[p->c++];
        pe->a = a;
        pe->b = b;
        pe->c = c;
        pe->g = g;
        pe->h = -1.0f;
        pe->x1c = D_002BE8AC;
    }
}

struct E { int a; int pad4; int b; int padc; int c; int pad14; int d; int pad1c; int pad20; };
struct L { char pad[0x1010]; int c; struct E *ae; };
/** @sly2-annot addr=0x155530
 * Purpose: Appends a route record of 4 oids
 * Info: proposed name AddDimitriRoute (T2); class DIMITRI; TU tu_1553F0
 * Notes: Cid3B->Cid59 (CID 0x3B=59, hex-literal) | class identity from whole_game WE names/object strings:
 *   AddCid59Route -> AddDimitriRoute
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest+whole_game(class identity)
 * See: docs/reference/functions/guards-ai.md#tu_1553f0-0x1553f0-0x1564e8
 * @end-sly2-annot */
void func_00155530(struct L *p, int a, int b, int c, int d)
{
    struct E *pe = &p->ae[p->c++];
    pe->a = a;
    pe->b = b;
    pe->c = c;
    pe->d = d;
}

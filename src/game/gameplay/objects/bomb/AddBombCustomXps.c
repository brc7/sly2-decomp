typedef int qword __attribute__((mode(TI)));
struct E { qword a; qword b; float g; int id; int pad[2]; };
struct P { char pad[0xE4]; int xE4; char pad2[0x1F4 - 0xE8]; int c; int n; struct E *ae; };
struct Q { char pad[0x60]; int id; };
struct O { char pad[0xEC]; struct Q *pq; };
struct S { int x0; struct O *po; };
struct G { int x; };
extern struct G D_003DD490;
void func_001F5990(struct P *p, struct S *ps, int a2, int a3, int a4, int a5);
void func_001A7C18(struct G *pg, int n, struct S *ps, qword a, qword b, int k, int x, int y, float g, int z);
/** @sly2-annot addr=0x13ADE8
 * Purpose: BOMB vt60: base SO hook, then for a custom-xp entry matching the other object add an XP (PxpAdd
 *   type 9)
 * Info: proposed name AddBombCustomXps (T1); class BOMB slot60; TU bomb
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r2b_rest
 * See: docs/engine/gameplay-objects.md#key-functions,
 *   docs/reference/functions/gameplay-objects.md#bomb-0x13a9e8-0x13bdf0
 * @end-sly2-annot */
void AddBombCustomXps(struct P *p, struct S *ps, int a2, int a3, int a4, int a5)
{
    func_001F5990(p, ps, a2, a3, a4, a5);
    if (p->c) {
        struct E *pe;
        for (pe = p->ae; pe < p->ae + p->n; pe++) {
            if (pe->id == ps->po->pq->id) {
                func_001A7C18(&D_003DD490, p->xE4, ps, pe->a, pe->b, 9, 0, 0, pe->g, a5);
                return;
            }
        }
    }
}

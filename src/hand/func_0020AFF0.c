struct X { int a; };
struct E { int n; struct X *px; int b; int c; };
struct S { char pad[0xAC]; int nAC; struct E e; };
void func_002090D0(struct E *pe, int n);
struct X *func_0020C010(int cb);
void func_0020BD78(struct X *px);
/** @sly2-annot addr=0x20AFF0
 * Purpose: binding kind 2 (0x40 payload)
 * Info: proposed name SetCid120AttackBindingK2 (T2); class CID120; TU stepguard
 * Notes: Cid78->Cid120 (CID 0x78=120, agent-default(hex))
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020AFF0(struct S *p, int a)
{
    struct E *pe = &p->e;
    func_002090D0(pe, p->nAC);
    p->e.n = 2;
    pe->px = func_0020C010(0x40);
    func_0020BD78(pe->px);
    pe->px->a = a;
    pe->c = 0;
}

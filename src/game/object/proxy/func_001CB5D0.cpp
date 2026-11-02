#define V(n) virtual void v##n();
struct A { char pad[0xDC]; int f; };
class BASIC {
public:
    int a, b, c;
    V(1) V(2) V(3) V(4) V(5) V(6) V(7) V(8) V(9) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19) V(20)
    V(21) V(22) V(23) V(24) V(25) V(26)
    virtual void Fn(int n);
};
class LO : public BASIC {
public:
    char pad[0x24 - 0x10];
    LO *pNext;
    char pad2[0x64 - 0x28];
    LO *pFirst;
    char pad3[0xE8 - 0x68];
    A *pa;
};
extern "C" {
void func_001CB5C8(LO *p, int n);
void func_001F37C8(LO *p, int n);
void func_001CB5D0(LO *p, int n);
}
/** @sly2-annot addr=0x1CB5D0
 * Purpose: SO vt27
 * Info: proposed name SO_VUnk27Proxy (T2); class SO slot27; TU proxy
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r5_1AE130
 * See: docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
void func_001CB5D0(LO *p, int n)
{
    LO *pc;
    int f;
    func_001CB5C8(p, n);
    f = p->pa ? p->pa->f : 0;
    if (f) func_001F37C8(p, 0);
    for (pc = p->pFirst; pc; pc = pc->pNext) pc->Fn(n);
}

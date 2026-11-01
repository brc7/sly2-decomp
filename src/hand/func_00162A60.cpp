class OBase {
public:
    int a, b, c;
    virtual void V1();
    virtual void V2();
    virtual void Msg(int msgid, int a, int c, void **apv, int d);
};
class O : public OBase {
public:
    char pad[0x7C - 0x10];
    O *f7c;
    char pad2[0x8C - 0x80];
    int f8c, f90, f94, f98;
    char pad3[0xF0 - 0x9C];
    int ff0;
};
struct W { int a, b, f8; char pad[0x38 - 0xC]; int f38; };
struct G { char pad[0x3B14]; O *f3b14; char pad2[0x3C88 - 0x3B18]; int f3c88; int f3c8c; };
extern G D_003DD490;
extern char D_002DEB30[];
extern "C" void func_00165DC0(G *pg, O *p);
extern "C" void func_00165E10(G *pg, O *p);
extern "C" W *func_00210B70();
extern "C" int func_00210A88();
extern "C" W *func_00210990(int n);
extern "C" int func_00162670(O *p);
extern "C" void func_001619A0(void *pv, int n, O *p);
extern "C" void func_001620B0(O *p, int n, int c, int a);
extern "C" void func_001623D0(O *p, int n, int c, int a, int b);
static inline int FSame(W *pw1, W *pw2)
{
    if (pw1 == pw2) return 1;
    if (!pw1 || !pw2) return 0;
    return (pw1->f8 ^ pw1->f38) == (pw2->f8 ^ pw2->f38);
}
/** @sly2-annot addr=0x162A60
 * Purpose: Clears current job (and pending task if it belonged to p); resets the warp target when in the
 *   job's world entry; if live: event 0x22, state-5 visibility/RSMGs, SendMessage 0x14
 * Info: proposed name EndJob (T1); TU game
 * Notes: names_v2 v1 proposed OnTurretActive__FP6TURRETiP2PO (0.005)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
extern "C" void func_00162A60(O *p)
{
    func_00165DC0(&D_003DD490, 0);
    if (D_003DD490.f3b14 && D_003DD490.f3b14->f7c && D_003DD490.f3b14->f7c == p)
        func_00165E10(&D_003DD490, 0);
    W *pw1 = func_00210B70();
    W *pw2 = func_00210990(func_00210A88());
    if (FSame(pw1, pw2)) {
        D_003DD490.f3c88 = 0x189;
        D_003DD490.f3c8c = -1;
    }
    if (func_00162670(p)) {
        func_001619A0(D_002DEB30, 0x22, p);
        func_001620B0(p, 5, p->f8c, p->f90);
        func_001623D0(p, 5, p->f94, p->f98, p->ff0);
        p->Msg(0x14, 0, 0, 0, 0);
    }
}

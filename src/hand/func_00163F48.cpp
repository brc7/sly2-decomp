class OBase {
public:
    int a, b, c;
    virtual void V1();
    virtual void V2();
    virtual void Msg(int msgid, int a, int c, void **apv, int d);
};
class O : public OBase {
public:
    char pad[0x5C - 0x10];
    int f5c;
    int f60;
    char pad2[0xA8 - 0x64];
    int fa8, fac, fb0, fb4;
    char pad3[0xF4 - 0xB8];
    int ff4;
};
struct G;
extern G D_003DD490;
extern char D_002DEB30[];
extern char D_003DA0C0[];
extern "C" void func_00165DE8(G *pg, O *p);
extern "C" int func_001637E8(O *p);
extern "C" void func_001619A0(void *pv, int n, O *p);
extern "C" void func_001620B0(O *p, int n, int c, int a);
extern "C" void func_001623D0(O *p, int n, int c, int a, int b);
extern "C" void func_001DA578(void *pv, int n);
/** @sly2-annot addr=0x163F48
 * Purpose: Clears current task; if live: event 0x24, state-5 visuals/RSMGs, stops its music, SendMessage 0x14
 * Info: proposed name EndTask (T1); TU game
 * Confidence: mid (draft) [T1 0.60] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
extern "C" void func_00163F48(O *p)
{
    func_00165DE8(&D_003DD490, 0);
    if (func_001637E8(p)) {
        func_001619A0(D_002DEB30, 0x24, p);
        func_001620B0(p, 5, p->fa8, p->fac);
        func_001623D0(p, 5, p->fb0, p->fb4, p->ff4);
        if (p->f5c != -1 && p->f60)
            func_001DA578(D_003DA0C0, p->f5c);
        p->Msg(0x14, 0, 0, 0, 0);
    }
}

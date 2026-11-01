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
    int f7c;
    int pad80, pad84, pad88;
    int f8c, f90, f94, f98;
    char pad2[0xF0 - 0x9C];
    int ff0;
};
struct G;
extern G D_003DD490;
extern char D_002DEB30[];
extern "C" void func_00165DC0(G *pg, O *p);
extern "C" int func_00162670(O *p);
extern "C" void func_00163460(O *p);
extern "C" void func_001619A0(void *pv, int n, O *p);
extern "C" void func_001620B0(O *p, int n, int c, int a);
extern "C" void func_001623D0(O *p, int n, int c, int a, int b);
/** @sly2-annot addr=0x1629A0
 * Purpose: Makes p the current job; if live: ++count, save vars, event 0x21, state-4 visibility/RSMGs,
 *   SendMessage 0x13
 * Info: proposed name BeginJob (T1); TU game
 * Notes: names_v2 v1 proposed UpdateTurretActive__FP6TURRETP3JOYf (0.000)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r3b_rest
 * See: docs/reference/functions/game-progression.md#game-0x1613a8-0x166270
 * @end-sly2-annot */
extern "C" void func_001629A0(O *p)
{
    func_00165DC0(&D_003DD490, p);
    if (func_00162670(p)) {
        p->f7c++;
        func_00163460(p);
        func_001619A0(D_002DEB30, 0x21, p);
        func_001620B0(p, 4, p->f8c, p->f90);
        func_001623D0(p, 4, p->f94, p->f98, p->ff0);
        p->Msg(0x13, 0, 0, 0, 0);
    }
}

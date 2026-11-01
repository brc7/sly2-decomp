struct S { int a; int b; int c; void *vt; char pad[0x6C]; void *p7c; int n80; };
extern char D_002CA240[];
struct SL;
extern struct SL D_003E133C;
void func_001C42E8(struct S *p);
void func_0011A070(struct SL *psl, void *pv);
/** @sly2-annot addr=0x1C4280
 * Purpose: POCTL ctor: vptr D_002CA240, Reset, ppo(+0x7C)=ppo, nState(+0x80)=-1, append to g_sw POCTL list
 *   D_003E133C
 * Info: proposed name POCTL_ctor (T1); class POCTL; TU po
 * Notes: names_v2 C proposed CloneRoh__FP3ROHT0 (0.438) | C++ signature hint (PO*) | mangled dropped (base
 *   does not match final name): __5POCTLP2PO
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
struct S *func_001C4280(struct S *p, void *q)
{
    p->a = 0;
    p->b = 0;
    p->vt = D_002CA240;
    func_001C42E8(p);
    p->p7c = q;
    p->n80 = -1;
    func_0011A070(&D_003E133C, p);
    return p;
}

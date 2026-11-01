struct MSG { int a; int b; int k; };
struct P { char pad[0x160]; int id; int f164; int f168; int f16C; int x170; };
void func_00133590(struct P *p, int kind, struct MSG *pmsg);
void func_00147410(struct P *p, int n);
/** @sly2-annot addr=0x147338
 * Purpose: vt5: msg 9 kinds 0x5FB..0x601 toggle three flags or start (state 3)
 * Info: proposed name HandleCid104Message (T2); class CID104 slot5; TU tu_1471C0
 * Notes: Cid68->Cid104 (CID 0x68=104, vtable)
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/cutscenes.md#tu_1471c0-0x1471c0-0x147e20
 * @end-sly2-annot */
void func_00147338(struct P *p, int kind, struct MSG *pmsg)
{
    func_00133590(p, kind, pmsg);
    if (kind != 9) return;
    if (pmsg->a != p->id) return;
    switch (pmsg->k) {
    case 0x5FB: p->f164 = 1; break;
    case 0x5FC: p->f164 = 0; break;
    case 0x5FD: p->f168 = 1; break;
    case 0x5FE: p->f168 = 0; break;
    case 0x5FF: p->f16C = 0; break;
    case 0x600: p->f16C = 1; break;
    case 0x601:
        if (p->x170 != -1) func_00147410(p, 3);
        break;
    }
}

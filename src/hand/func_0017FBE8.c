struct G { int a; };
extern struct G D_003DD490;
extern int D_002E1E00;
extern char D_002DEB30[];
struct S { char pad[0x184]; int n184; char pad2[0x38]; int n1C0; };
int func_0020F0B0(struct G *g);
int func_0017F1D0(struct S *p, int *pn);
void func_00180100(struct S *p);
void func_001619A0(void *g, int n, int m);
void func_0017FD08(struct S *p, int n);
/** @sly2-annot addr=0x17FBE8
 * Purpose: vt75: ignored in states 2-4 / hands-off / global flag; CID47::TakeDamage, update health bar, game
 *   event 0xF and state 2 (hit) or 4 (defeated)
 * Info: proposed name HACKSHIP_TakeDamage (T1); class HACKSHIP slot75; TU tu_17F090
 * Notes: CID48->CID72 (CID 0x48=72, vtable) | class identity from whole_game WE names/object strings:
 *   CID72_TakeDamage -> HACKSHIP_TakeDamage
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
int func_0017FBE8(struct S *p, int *pn) {
    if (p->n1C0 < 5) { if (p->n1C0 >= 2) return 0; }
    if (func_0020F0B0(&D_003DD490)) return 0;
    if (D_002E1E00 & 1) return 0;
    if (!func_0017F1D0(p, pn)) return 0;
    func_00180100(p);
    if (p->n184 > 0) { func_001619A0(D_002DEB30, 0xF, *pn); func_0017FD08(p, 2); }
    else func_0017FD08(p, 4);
    return 1;
}

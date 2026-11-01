struct E { int a; int b; int x8; int xc; char pad[0x30]; };
struct S { char pad[0x188]; int n; };
void func_0017F1A0(struct S *p, struct E *ae, int i);
int FIsBasicDerivedFrom__FP5BASIC3CID(int oid, int n);
void func_0017F5B0(struct S *p, int n);
/** @sly2-annot addr=0x182700
 * Purpose: vt55: base then start dying
 * Info: proposed name AdjustHackwallXp (T2); class HACKWALL slot55; TU tu_17F090
 * Notes: CID4C->CID76 (CID 0x4C=76, hex-literal) | class identity from whole_game WE names/object strings:
 *   AdjustCid76Xp -> AdjustHackwallXp
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_00182700(struct S *p, struct E *ae, int i)
{
    func_0017F1A0(p, ae, i);
    if (p->n) {
        struct E *pe = &ae[1 - i];
        int oid = pe->xc;
        if (FIsBasicDerivedFrom__FP5BASIC3CID(oid, 0x47))
            func_0017F5B0(p, oid);
    }
}

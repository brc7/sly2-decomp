struct E { int a; int b; int x8; int xc; char p0[8]; float g18; char pad[0x24]; };
struct S { char pad[0x1CC]; int x1cc; };
void func_0017F1A0(struct S *p, struct E *ae, int i);
int FIsBasicDerivedFrom__FP5BASIC3CID(int oid, int n);
void func_0017F5B0(struct S *p, int n);
/** @sly2-annot addr=0x1803E0
 * Purpose: vt55: CID47::AdjustXp then start dying on contact
 * Info: proposed name AdjustHackbulletXp (T2); class HACKBULLET slot55; TU tu_17F090
 * Notes: CID4A->CID74 (CID 0x4A=74, hex-literal) | class identity from whole_game WE names/object strings:
 *   AdjustCid74Xp -> AdjustHackbulletXp
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_001803E0(struct S *p, struct E *ae, int i)
{
    struct E *pe;
    int oid;
    func_0017F1A0(p, ae, i);
    pe = &ae[1 - i];
    oid = pe->xc;
    if (FIsBasicDerivedFrom__FP5BASIC3CID(oid, 0x47) || !p->x1cc)
        func_0017F5B0(p, oid);
    if (p->x1cc) ae[2].g18 = 1.0f;
}

struct S { char pad[0x40]; int fl; char p2[0x248]; char dl[0xC]; };
void func_0017F090(struct S *p);
void func_0011A1F8(void *pdl, int ib);
/** @sly2-annot addr=0x1817E8
 * Purpose: CID 0x4C vt7: InitCid47, grf
 * Info: proposed name InitHackwall (T1); class HACKWALL slot7; TU tu_17F090
 * Notes: Cid4C->Cid76 (CID 0x4C=76, hex-literal) | class identity from whole_game WE names/object strings:
 *   InitCid76 -> InitHackwall
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r4b_rest+whole_game(class identity)
 * See: docs/reference/functions/gameplay-objects.md#tu_17f090-0x17f090-0x182c70
 * @end-sly2-annot */
void func_001817E8(struct S *p)
{
    int v;
    func_0017F090(p);
    v = p->fl;
    if ((v ^ 1) & 1) *(int *)v |= 8;
    else p->fl = v | 8;
    func_0011A1F8(p->dl, 0x60);
}

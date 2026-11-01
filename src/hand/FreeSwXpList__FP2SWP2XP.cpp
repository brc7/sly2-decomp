struct SLOTHEAP { int a[4]; };
struct XP { char pad[0xAC]; XP *pxpNextSw; };
struct SW { char pad[0x3D40]; SLOTHEAP slotheapXp; };
void FreeSlotheapPv(SLOTHEAP *psh, void *pv) __asm__("func_0011CD98");
/** @sly2-annot addr=0x1A7E10
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/physics-collision.md#mark-0x1a7c18-0x1aa978
 * @end-sly2-annot */
void FreeSwXpList(SW *psw, XP *pxp)
{
    while (pxp) {
        XP *pxpNext = pxp->pxpNextSw;
        FreeSlotheapPv(&psw->slotheapXp, pxp);
        pxp = pxpNext;
    }
}

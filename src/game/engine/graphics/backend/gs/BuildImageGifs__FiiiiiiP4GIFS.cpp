#include "gifs.h"
/** @sly2-annot addr=0x170650
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name BuildImageGifs__FiiiiiiP4GIFS (EXISTING); TU gs
 * Confidence: name high [EXISTING ]; purpose not reviewed - evidence: pre-existing symbol in symbol_addrs.txt
 * See: docs/reference/functions/rendering.md#gs-0x16f2d0-0x1709a8
 * @end-sly2-annot */
void BuildImageGifs(int dbp, int dbw, int dpsm, int rrw, int rrh, int cqw, GIFS *pgifs)
{
    pgifs->AddPrimPack(0, 1, 0xE);
    pgifs->PackAD(0x50, ((long)dbp << 32) | ((long)dbw << 48) | ((long)dpsm << 56));
    pgifs->PackAD(0x51, 0);
    pgifs->PackAD(0x52, rrw | ((long)rrh << 32));
    pgifs->PackAD(0x53, 0);
    pgifs->AddImage(cqw);
}

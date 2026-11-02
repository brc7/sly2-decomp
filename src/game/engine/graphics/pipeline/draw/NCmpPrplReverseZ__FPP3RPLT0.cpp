struct RPL { int a; float z; int pad[2]; };
/** @sly2-annot addr=0x157A50
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Info: proposed name NCmpPrplReverseZ__FPP3RPLT0 (EXISTING); TU draw
 * Confidence: name high [EXISTING ]; purpose not reviewed - evidence: pre-existing symbol in symbol_addrs.txt
 * See: docs/engine/rendering.md#12-source-files,
 *   docs/reference/functions/rendering.md#draw-0x157a50-0x1585d8,
 *   docs/reference/tu-map.md#tu-map-address-range---source-file
 * @end-sly2-annot */
int NCmpPrplReverseZ(RPL **pprpl1, RPL **pprpl2)
{
    RPL *plhs = *pprpl1;
    RPL *prhs = *pprpl2;
    if (prhs->z < plhs->z)
        return -1;
    if (plhs->z < prhs->z)
        return 1;
    return plhs - prhs;
}

typedef struct RPL { int a; float z; int pad[2]; } RPL;
/** @sly2-annot addr=0x157A98
 * Purpose: qsort comparator: RPLs by ascending z, ties by address (front-to-back order)
 * Info: proposed name NCmpPrplZ (T1); TU draw
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/rendering.md#draw-0x157a50-0x1585d8
 * @end-sly2-annot */
int NCmpPrplZ__FPP3RPLT0(RPL **pprpl1, RPL **pprpl2)
{
    RPL *plhs = *pprpl1;
    RPL *prhs = *pprpl2;
    if (prhs->z < plhs->z)
        return 1;
    if (plhs->z < prhs->z)
        return -1;
    return prhs - plhs;
}

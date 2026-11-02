typedef struct DYNA { char *ab; int c; int cMax; int cb; } DYNA;

void func_00116D70(DYNA *pdyna, int c);
void *func_0023320C(void *pvDst, const void *pvSrc, unsigned int cb);

/** @sly2-annot addr=0x116E30
 * Purpose: Reserve(c+n) then copy n elements of cb bytes at the end; c += n
 * Info: proposed name CDynArray_Append (T1); class CDynArray; TU dynarray
 * Notes: names_v2 v1 proposed FOpenSector__18CBinaryInputStreamUiUi (0.002)
 * Confidence: mid (draft) [T1 0.60] - evidence: static naming pass; sources: r1_116918
 * See: docs/reference/functions/core-library.md#dynarray-0x116ca0-0x117030
 * @end-sly2-annot */
void CDynArray_Append(DYNA *pdyna, void *pv, int c)
{
    func_00116D70(pdyna, pdyna->c + c);
    func_0023320C(pdyna->ab + pdyna->cb * pdyna->c, pv, pdyna->cb * c);
    pdyna->c += c;
}

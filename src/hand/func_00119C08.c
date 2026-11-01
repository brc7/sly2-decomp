typedef struct ALLOC ALLOC;
typedef struct CBinaryInputStream CBinaryInputStream;
typedef struct HEAPHOLDER { ALLOC *palloc; } HEAPHOLDER;
typedef struct X { HEAPHOLDER *pheap; } X;

void CBinaryInputStream_ReadStringSw(CBinaryInputStream *pbis, char **ppchz, ALLOC *palloc, int nKind);

/** @sly2-annot addr=0x119C08
 * Purpose: vtable slot 4: ReadStringSw(pbis, ppchz, this->pheap->palloc, kind 7)
 * Info: proposed name CUnk2CA5E0_ReadString (T2); class CUnk2CA5E0; TU tu_119BC8
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r1_116918
 * See: docs/reference/functions/core-library.md#tu_119bc8-0x119bc8-0x119da0
 * @end-sly2-annot */
void func_00119C08(X *px, char **ppchz, CBinaryInputStream *pbis)
{
    CBinaryInputStream_ReadStringSw(pbis, ppchz, px->pheap->palloc, 7);
}

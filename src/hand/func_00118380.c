typedef struct VTE { short d; short pad; void *(*pfn)(void *, int, int); } VTE;
typedef struct ALLOC { VTE *pvt; } ALLOC;
typedef struct CBinaryInputStream CBinaryInputStream;

int func_00117C20(CBinaryInputStream *pbis);
void func_00117968(CBinaryInputStream *pbis, int cb, void *pv);

/** @sly2-annot addr=0x118380
 * Purpose: Reads a U16-length-prefixed string into a buffer allocated from a heap/alloc request (Sly2 adds
 *   allocator + kind args)
 * Info: proposed name CBinaryInputStream_ReadStringSw (T1); class CBinaryInputStream; TU bis
 * Notes: level_parser CONFIRMED-parse: payload layout followed by the parser, 100%% of bytes in 4 levels
 * Confidence: mid (draft) [T1 0.75] - evidence: static analysis; sources: r1_116918+parse-verified
 * See: docs/engine/core-library.md#7-key-functions,
 *   docs/reference/functions/core-library.md#bis-0x1177a8-0x118668
 * @end-sly2-annot */
void func_00118380(CBinaryInputStream *pbis, char **ppchz, ALLOC *palloc, int nKind)
{
    int cch = func_00117C20(pbis);
    char *pchz;
    if (cch == 0) {
        *ppchz = 0;
        return;
    }
    pchz = (char *)palloc->pvt[1].pfn((char *)palloc + palloc->pvt[1].d, cch + 1, nKind);
    func_00117968(pbis, cch, pchz);
    pchz[cch] = 0;
    *ppchz = pchz;
}

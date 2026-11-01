typedef struct VTE { short d; short pad; void *(*pfn)(void *, int, int); } VTE;
typedef struct ALLOC { VTE *pvt; } ALLOC;
typedef struct CBinaryInputStream CBinaryInputStream;

int func_00117C20(CBinaryInputStream *pbis);
void func_00117968(CBinaryInputStream *pbis, int cb, void *pv);

void CBinaryInputStream_ReadStringSw(CBinaryInputStream *pbis, char **ppchz, ALLOC *palloc, int nKind)
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

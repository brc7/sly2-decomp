typedef struct SL { void *pvFirst; void *pvLast; int ibNext; } SL;

void PrependSlEntry(SL *psl, void *pv);
void func_0011A070(SL *psl, void *pv);

void InsertSlEntryAfter(SL *psl, void *pvPrev, void *pv)
{
    if (pvPrev == 0) {
        PrependSlEntry(psl, pv);
    } else if (pvPrev == psl->pvLast) {
        func_0011A070(psl, pv);
    } else {
        void **ppvPrev = (void **)((char *)pvPrev + psl->ibNext);
        *(void **)((char *)pv + psl->ibNext) = *ppvPrev;
        *ppvPrev = pv;
    }
}

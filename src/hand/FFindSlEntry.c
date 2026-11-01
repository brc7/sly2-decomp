typedef struct SL { void *pvFirst; void *pvLast; int ibNext; } SL;

int FFindSlEntry(SL *psl, void *pv)
{
    int f = 0;
    if (*(void **)((char *)pv + psl->ibNext) != 0 || psl->pvLast == pv) {
        f = 1;
    }
    return f;
}

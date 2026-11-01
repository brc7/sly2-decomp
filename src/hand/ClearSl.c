typedef struct SL { void *pvFirst; void *pvLast; int ibNext; } SL;
void ClearSl(SL *psl) { psl->pvLast = 0; psl->pvFirst = 0; }

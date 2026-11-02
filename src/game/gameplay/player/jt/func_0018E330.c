extern char D_003DD490[];
int CploFindSwObjectsByClass(void *g, int a, int b, int c, int cMax, void **ap);
void func_001F1900(void *p);
/** @sly2-annot addr=0x18E330
 * Purpose: Find up to 32 objects (0x15E9C8 g_sw,0x505,0x38) and call 0x1F1900 on each
 * Info: TU jt
 * Notes: descriptive only: (JT callback: notify objects) | no usable identifier; purpose only
 * Confidence: low (draft) [T2 0.20] - evidence: static analysis; sources: r4_17F090
 * @end-sly2-annot */
void func_0018E330(void)
{
    void *ap[32];
    int c, i;
    c = CploFindSwObjectsByClass(D_003DD490, 0x505, 0x38, 0, 0x20, ap);
    for (i = 0; i < c; i++)
        func_001F1900(ap[i]);
}

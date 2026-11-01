struct D { int a; int ib; };
extern char D_003DD490[];
int PloFindSwObject__FP2SWi3OIDP2LO(void *, int, int, void *);
/** @sly2-annot addr=0x13E590
 * Purpose: LORUN callback: *(OID-resolved LO*)(plo + ib) = PloFindSwObject(g_sw, 4, oid, plo)
 * Info: proposed name PostLoadOidField (T1); TU brx
 * Confidence: high [T1 0.85] - evidence: semantic review; sources: r2_131F70
 * See: docs/engine/world-sw.md#10-key-functions, docs/reference/functions/world-sw.md#brx-0x13e368-0x13ff70
 * @end-sly2-annot */
void PostLoadOidField__FP2LOPv(char *p, struct D *pd) {
    *(int *)(p + pd->ib) = PloFindSwObject__FP2SWi3OIDP2LO(D_003DD490, 4, pd->a, p);
}

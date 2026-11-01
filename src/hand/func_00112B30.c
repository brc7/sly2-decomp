typedef struct BASIC { int m0; int m4; int m8; void *pvtbl; } BASIC;
extern char D_002D5120[];
void *func_001AE4F8(int cb);
void func_002562A8(void *pv);

static inline BASIC *CtorBasic(BASIC *p)
{
    p->pvtbl = D_002D5120;
    p->m0 = 0;
    p->m4 = 0;
    return p;
}

/** @sly2-annot addr=0x112B30
 * Purpose: pfnNew factory for CID124 (cid 124): allocates 0x160 bytes (operator new 0x1AE4F8), calls ctor
 *   func_002562A8
 * Info: proposed name NewPathlink (T1); TU crt0_splicewrap
 * Notes: v3 class identity: NewCid124 -> NewPathlink
 * Confidence: high [T1 0.95] - evidence: static naming pass; sources: r0_thunks+table-verified(D_002BAAE8
 *   pfnNew/size 185/185; vt slot6 199/199)+class_review(master_names_v2_1)
 * See: docs/reference/functions/crt0-thunks.md#crt0_splicewrap-0x100000-0x1147d8
 * @end-sly2-annot */
void *func_00112B30(void)
{
    BASIC *p = CtorBasic((BASIC *)func_001AE4F8(0x160));
    char *pb = (char *)p + 0x60;
    int i = 5;
    do {
        func_002562A8(pb);
        i--;
        pb += 0x20;
    } while (i != -1);
    return p;
}

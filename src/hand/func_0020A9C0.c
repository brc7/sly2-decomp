struct S { char pad[0xEB8]; int aEB8[8]; int cED8; int aEDC[8]; int cEFC; };
int func_0013E3C8(int a, struct S *p, int b, int c);
static __inline__ void Push(struct S *p, int id, int v)
{
    p->aEDC[p->cEFC++] = v;
    p->aEB8[p->cED8++] = id;
}
/** @sly2-annot addr=0x20A9C0
 * Purpose: Return first attack object; if none create default cid 0x78 (oid 0x2CC)
 * Info: proposed name PattackDefaultStepguard (T2); TU stepguard
 * Confidence: low (draft) [T2 0.35] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
int func_0020A9C0(struct S *p)
{
    int v;
    if (p->cED8 == 0) {
        v = func_0013E3C8(0x78, p, 0x2CC, -1);
        Push(p, 0x2CC, v);
    } else {
        v = p->aEDC[0];
    }
    return v;
}

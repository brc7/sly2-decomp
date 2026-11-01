struct K { short x; short y; int i; };
struct S { char pad[0x104]; struct K k; };
void func_002311B0(struct S *p, int n);
/** @sly2-annot addr=0x156D98
 * Purpose: XFM clone (0x2311B0) preserving a 3-word key field
 * Info: proposed name CloneCid47 (T2); class CID47 slot41; TU noise
 * Notes: Cid2F->Cid47 (CID 0x2F=47, hex-literal)
 * Confidence: low (draft) [T2 0.60] - evidence: static analysis; sources: r3_1539A8
 * See: docs/reference/functions/guards-ai.md#noise-0x1564e8-0x157200
 * @end-sly2-annot */
void func_00156D98(struct S *p, int n)
{
    struct K k = p->k;
    func_002311B0(p, n);
    p->k = k;
}

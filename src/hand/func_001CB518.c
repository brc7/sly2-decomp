typedef int qword __attribute__((mode(TI)));
struct VTE { short d; short pad; void (*pf)(); };
struct LO { char pad[0xC]; struct VTE *vt; char p1[0x24 - 0x10]; struct LO *x24; };
struct Q { char pad[0xDC]; int xDC; };
struct S { char pad[0x64]; struct LO *x64; char p1[0xCC - 0x68]; int xCC; char p2[0xE8 - 0xD0]; struct Q *xE8; };
void func_001CB4E0(struct S *p, int n);
void func_001F7A68(struct S *p, int n);
qword func_001A62E8(void *p);
void *func_001C0D70(qword q, int n);
void func_001F37C8(struct S *p, void *pv);
/** @sly2-annot addr=0x1CB518
 * Purpose: SO slot 26 (called by PspareAcquire with the PROXY when a spare copy is placed): ResetLoForProxy
 *   base, SetSoZapMode(0), clear +0xCC, re-pick the physics region from PpathzoneFromPos when needed, then
 *   slot 26 on children
 * Info: proposed name ResetSoForProxy (T2); class SO slot26; TU proxy
 * Notes: v3: superseded SO_VUnk26Proxy | top50_round (0.50): PspareAcquire 0x1CAEC8 calls vt37(matrix) then
 *   vt26(proxy) on each copy object; vt37 is the existing ApplySoProxy 0x1F4010 (Sly 1 ApplyAloProxy =
 *   transform by the proxy), so slot 26 is the separate reset step. LO base 0x
 * Confidence: low (draft) [T2 0.50] - evidence: Sly 1 / Lilly reference; semantic review; sources:
 *   r5_1AE130+top50_round
 * See: docs/reference/functions/gameplay-objects.md#tu_1d1370-0x1d1370-0x1d5150,
 *   docs/reference/functions/object-model.md#proxy-0x1c9940-0x1cb908
 * @end-sly2-annot */
void func_001CB518(struct S *p, int n)
{
    struct LO *plo;
    int f;
    func_001CB4E0(p, n);
    func_001F7A68(p, 0);
    p->xCC = -1;
    f = 0;
    if (p->xE8) f = p->xE8->xDC;
    if (f) {
        func_001F37C8(p, func_001C0D70(func_001A62E8(p), 1));
    }
    for (plo = p->x64; plo; plo = plo->x24)
        ((void (*)(void *, int))plo->vt[26].pf)((char *)plo + plo->vt[26].d, n);
}

struct S { char pad[0xE8]; int f; };
void func_001CB518(struct S *p);
void func_001BAF18(struct S *p, int n);
void SetSoCnstrForce(struct S *p, int n);
void SetSoCnstrTorque(struct S *p, int n);
/** @sly2-annot addr=0x1BBEA8
 * Purpose: CID 98 vt26: proxy base, reset to first stage (0x609), restore constraints
 * Info: proposed name Cid98_VUnk26Proxy (T2); class CID98 slot26; TU tu_1BAB50
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1bab50-0x1bab50-0x1bc050
 * @end-sly2-annot */
void func_001BBEA8(struct S *p)
{
    func_001CB518(p);
    func_001BAF18(p, 0x609);
    if (p->f) {
        SetSoCnstrForce(p, 1);
        SetSoCnstrTorque(p, 1);
    }
}

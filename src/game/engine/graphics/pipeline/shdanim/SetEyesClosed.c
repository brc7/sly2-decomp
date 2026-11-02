struct EYES { char pad[0x64]; int n64; char pad2[0x74-0x68]; float g74; float g78; };
void func_001EE100(struct EYES *p, int eyess);
void SetSaiIframe(struct EYES *p, int iframe);
/** @sly2-annot addr=0x1EE3B0
 * Purpose: Set EYES closed amount (called per EYES SAA by SetSoEyesClosed 0x1F80D0)
 * Info: proposed name SetEyesClosed (T1); TU shdanim
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/rendering.md#shdanim-0x1ed7a8-0x1ee988,
 *   docs/reference/functions/world-sw.md#sw-0x20d9d0-0x210eb0
 * @end-sly2-annot */
void SetEyesClosed(struct EYES *p, float g)
{
    p->g78 = g;
    if (g >= 1.0f) {
        func_001EE100(p, 2);
    } else {
        p->n64 = -1;
        func_001EE100(p, 0);
    }
    SetSaiIframe(p, (int)p->g74);
}

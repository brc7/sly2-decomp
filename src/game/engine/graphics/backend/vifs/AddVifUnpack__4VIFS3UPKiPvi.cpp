enum UPK { UPK_0 };
struct VIFS { void AddVifUnpack(UPK upk, int c, void *pv, int iqw); };
void UnpackHelper__4VIFS3UPKiiPiPPUi(VIFS *pvifs, UPK upk, int c, int iqw, int *pcb, unsigned int **ppw);
/** @sly2-annot addr=0x226F28
 * Purpose: not written up yet; only the symbol name (pre-existing / Sly 1-derived) describes it.
 * Confidence: mid (draft) - evidence: pre-existing symbol name only; behaviour not reviewed
 * See: docs/reference/functions/rendering.md#shd-0x1ea568-0x1ed7a8,
 *   docs/reference/functions/rendering.md#vifs-0x226c10-0x227350
 * @end-sly2-annot */
void VIFS::AddVifUnpack(UPK upk, int c, void *pv, int iqw)
{
    int cb;
    unsigned int *pw;
    unsigned int *pwSrc = (unsigned int *)pv;
    UnpackHelper__4VIFS3UPKiiPiPPUi(this, upk, c, iqw, &cb, &pw);
    unsigned int *pwDst = pw;
    int cw = (cb + 3) >> 2;
    int cRem = cw & 3;
    int cBlk = cw - cRem;
    int i;
    for (i = 0; i < cRem; i++) *pwDst++ = *pwSrc++;
    for (i = 0; i < cBlk; i += 4) {
        unsigned int a = pwSrc[0], b = pwSrc[1], c2 = pwSrc[2], d = pwSrc[3];
        pwDst[0] = a; pwDst[1] = b; pwDst[2] = c2; pwDst[3] = d;
        pwSrc += 4; pwDst += 4;
    }
}

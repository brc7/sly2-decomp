struct S { int f0; void *f4; char pad[0x24 - 8]; void *f24; };
extern void *D_002DE594;
extern void *D_002DE598;
void PropagateSais(struct S *p, int f);
void RebuildFrameGifs__Fv(void);
void SendDmaSyncGsFinish__FP10sceDmaChanP2QW(void *pch, void *pqw);
/** @sly2-annot addr=0x160C80
 * Purpose: Synchronously renders one frame: PrepareGsForFrameRender, rebuild frame packets, send VIF chain,
 *   and GIF chain if fRenderGifs
 * Info: proposed name RenderFrame (T1); TU frm
 * Confidence: high [T1 0.90] - evidence: static analysis; sources: r3_1539A8+verdicts_tu_r3r5+verdicts_A1
 * See: docs/engine/rendering.md#13-key-functions, docs/guides/port-plan.md#61-interception-point-k7-k8-k16,
 *   docs/reference/functions/rendering.md#frm-0x1607a0-0x1613a8
 * @end-sly2-annot */
void RenderFrame__FP3FRMi(struct S *p, int f)
{
    PropagateSais(p, f);
    RebuildFrameGifs__Fv();
    SendDmaSyncGsFinish__FP10sceDmaChanP2QW(D_002DE594, p->f24);
    if (f)
        SendDmaSyncGsFinish__FP10sceDmaChanP2QW(D_002DE598, p->f4);
}

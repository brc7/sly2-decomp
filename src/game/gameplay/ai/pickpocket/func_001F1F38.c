struct VTE { short d; short p; void (*pfn)(void *, int); };
struct VT { struct VTE a[22]; };
struct Q { char pad[0xC]; struct VT *pvt; };
struct P { char pad[0x494]; struct Q *pq; };
#ifdef __cplusplus
extern "C" {
#endif
void func_00205D10(struct P *, int);
#define VF(q, n) (q)->pvt->a[21].pfn((char *)(q) + (q)->pvt->a[21].d, n)
/** @sly2-annot addr=0x1F1F38
 * Purpose: STEPGUARD vt87(f): STEP base (empty 0x205D10), then pouch(+0x494)->vt21(!f)
 * Info: proposed name SetStepguardPlayable (T2); class STEPGUARD slot87; TU pickpocket
 * Notes: superseded r6_1D5150 proposal: STEPGUARD::vt87 (0.3) | agent qualifier: (vt87) | runtime: pickpocket
 *   pouch is CID99 (SO child), not a PO
 * Confidence: low (draft) [T2 0.35] - evidence: runtime (save-state RAM); sources: r6b_rest+runtime-corrected
 * See: docs/reference/functions/guards-ai.md#pickpocket-0x1f1768-0x1f2108
 * @end-sly2-annot */
void func_001F1F38(struct P *p, int f)
{
    func_00205D10(p, f);
    if (f) { if (p->pq) VF(p->pq, 0); }
    else if (p->pq) VF(p->pq, 1);
}
#ifdef __cplusplus
}
#endif

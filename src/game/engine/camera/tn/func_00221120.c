struct S { char pad[0x10C]; int x; };
/** @sly2-annot addr=0x221120
 * Purpose: returns 0 if field null else stores &field
 * Info: proposed name FGetTnPshape (T2); class TN slot28; TU tn
 * Notes: agent qualifier: splice getter
 * Confidence: low (draft) [T2 0.30] - evidence: static analysis; sources: r8_21BB88
 * See: docs/reference/functions/camera.md#tn-0x220d40-0x223f70
 * @end-sly2-annot */
int func_00221120(struct S *p, int **pp)
{
    if (!p->x) return 0;
    *pp = &p->x; return 1;
}

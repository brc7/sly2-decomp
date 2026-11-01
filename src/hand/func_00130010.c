struct VTE { short d; short pad; int (*pfn)(); };
struct O { int pad[3]; struct VTE *vt; };
struct A { int f0; int f4; };
extern char D_003DD490[];
struct O *func_0015E938(void *, int, int, void *);
/** @sly2-annot addr=0x130010
 * Purpose: Event target resolution via PloFindSwObject (root-relative or world)
 * Info: proposed name ResolveAsegEventObject (T2); TU aseg
 * Notes: verifier: purpose partly wrong: Finds object by oid (grffso 1 under q, else 4 from p) and also
 *   stores po->vt43(pa->f0) into *pn (0 if not found); purpose omits that second output. | names_v2 C
 *   proposed PostAsegLoadCallback__FP4ASEG5MSGIDPv (0.638)
 * Confidence: low (draft) [T2 0.30] - evidence: static naming pass; sources: r1_116918+verdicts_tu_r1r2
 * See: docs/engine/animation.md#7-key-functions, docs/reference/functions/animation.md#aseg-0x12f248-0x1308a8
 * @end-sly2-annot */
void func_00130010(void *p, void *q, struct A *pa, struct O **ppo, int *pn) {
    if (q) *ppo = func_0015E938(D_003DD490, 1, pa->f4, q);
    else *ppo = func_0015E938(D_003DD490, 4, pa->f4, p);
    if (!*ppo) *pn = 0;
    else { struct O *po = *ppo; *pn = ((int (*)(void *, int))po->vt[43].pfn)((char *)po + po->vt[43].d, pa->f0); }
}

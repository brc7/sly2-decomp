struct VTE { short d; short pad; int (*pfn)(void *, int, int, int, void **, int); };
struct O { int a[3]; struct VTE *vt; };
/** @sly2-annot addr=0x1C4228
 * Purpose: Virtual slot 81 (Lilly AddHpMp(dhp,dmp)): forwards (dhp,dmp) as message 0xC through vt slot 3
 * Info: proposed name AddPoHpMp (T1); class PO slot81; TU po
 * Notes: names_v2 B proposed LoadRohFromBrx__FP3ROHP18CBinaryInputStream (0.923)
 * Confidence: high [T1 0.85] - evidence: static naming pass; sources: r5_1AE130+verdicts_tu_r3r5
 * See: docs/engine/player.md#11-key-functions, docs/reference/functions/player.md#po-0x1c3d70-0x1c4e50
 * @end-sly2-annot */
void func_001C4228(struct O *p, int a, int b)
{
    void *apv[2];
    int n0, n1;
    /*B*/n0 = a; n1 = b; apv[0] = &n0; apv[1] = &n1;/*E*/
    { struct VTE *e = &p->vt[3]; e->pfn((char *)p + e->d, 0xC, 0, 2, apv, 0); }
}

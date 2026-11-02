typedef int qword __attribute__((mode(TI)));
struct M { qword a[4]; };
struct O { char pad[0x40]; struct M m; char pad2[0xA4 - 0x80]; int nA4; };
struct E { struct O *po; int pad[7]; struct M m; char pad3[0xA8 - 0x60]; int nA8; int nAC; };
struct VTE { short d; short p; int (*pfn)(void *); };
struct VT { struct VTE a[63]; };
struct Q { char pad[0x34]; char dl[1]; };
struct P { char pad[0xC]; struct VT *pvt; char pad2[0xEC - 0x10]; struct Q *pq; };
void func_001F27F0(struct P *);
int FFindDlEntry__FP2DLPv(void *, void *);
void AppendDlEntry__FP2DLPv(void *, void *);
void func_001F4BF8(struct P *);
void UpdateSoPartGeom(struct E *);
void func_001F5078(struct P *);
/** @sly2-annot addr=0x1F2870
 * Purpose: Add one SOPART to the tree record's part DL (+0x34): copy XF matrix/stamp, mark tree dirty
 *   (0x1F4BF8), UpdateSoPartGeom, invalidate if not fixed
 * Info: proposed name AddSoPartToTree (T1); TU so
 * Confidence: mid (draft) [T1 0.50] - evidence: static analysis; sources: r6_1D5150
 * See: docs/reference/functions/physics-collision.md#so-0x1f2108-0x1f8dd8
 * @end-sly2-annot */
void AddSoPartToTree(struct P *p, struct E *pe)
{
    struct O *po;
    func_001F27F0(p);
    if (FFindDlEntry__FP2DLPv(p->pq->dl, pe)) return;
    AppendDlEntry__FP2DLPv(p->pq->dl, pe);
    po = pe->po;
    pe->nA8 = -1;
    pe->m = po->m;
    pe->nAC = po->nA4;
    func_001F4BF8(p);
    UpdateSoPartGeom(pe);
    if (!p->pvt->a[62].pfn((char *)p + p->pvt->a[62].d)) func_001F5078(p);
}

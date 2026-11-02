struct VTE { short d; short pad; void (*pfn)(); };
struct O { char pad[0xC]; struct VTE *vt; };
struct P { struct O *po; char pad[0x9C - 4]; int n9C; int pad2; int nA4; };
void *func_001FBB90(int n);
void func_001FB2F8(void *p, void *q);
void RunBtnAsegs(struct P *p, int n);
void SetBtnButtons(struct P *p, int n);
/** @sly2-annot addr=0x1487A0
 * Purpose: Fire Splice event 5 on the owner, run the 'on' asegs and RSMG triggers
 * Info: proposed name TriggerBtn (T1); TU button
 * Confidence: mid (draft) [T1 0.70] - evidence: static analysis; sources: r2b_rest
 * See: docs/reference/functions/gameplay-objects.md#button-0x148620-0x149a50
 * @end-sly2-annot */
void TriggerBtn(struct P *p)
{
    struct O *o = p->po;
    struct VTE *e = &o->vt[3];
    ((void (*)(void *, int, int, int, int, int))e->pfn)((char *)o + e->d, 5, 0, 0, 0, 0);
    if (!p->n9C) {
        struct O *o2 = p->po;
        func_001FB2F8(func_001FBB90(2), o2);
    }
    RunBtnAsegs(p, 0);
    if (!p->nA4) SetBtnButtons(p, 2);
    {
        struct O *o3 = p->po;
        struct VTE *e3 = &o3->vt[4];
        ((void (*)(void *, int, void *))e3->pfn)((char *)o3 + e3->d, 6, p);
    }
}

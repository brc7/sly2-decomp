struct CLOCK { float t; float dt; };
extern struct CLOCK D_002DDED0;
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x2C]; int n3C; };
struct T { char pad[0x32C]; int n32C; };
struct S { char pad[0x10]; struct T *p10; int pad2; char a18[0x18]; int n30; float g34; };
void PopBhv(void *p);
void ClearBhvBusy(struct S *p);
struct O *func_00205438(void *p, int n);
/** @sly2-annot addr=0x17A8C8
 * Purpose: BHV176 (thrown/stunned guard) state switch: push BHVANIM (cid 0x9D) with anim flag 18/19/20/21/19
 *   for states 0-4, airborne flag step+0x32C for states 0-1; -1 clears busy
 * Info: proposed name SetBhv176State (T2); TU guardbhv
 * Notes: BHV class word normalised: SetBhvthrownState -> SetBhv176State | top50_round (0.45): Body; BHV slot
 *   15 canon. Class word BHVTHROWN not accepted by canon (placeholder kept) -> T2 | T2: placeholder token 176
 * Confidence: low (draft) [T2 0.45] - evidence: semantic review; sources: r9_bhv_screen+top50_round
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_0017A8C8(struct S *p, int n) {
    if (p->n30 == n) return;
    PopBhv(p->a18);
    p->n30 = n;
    p->g34 = D_002DDED0.t;
    switch (n) {
    case 0: case 1: case 2: case 3: case 4:
        {
            struct O *o = func_00205438(p->p10, 0x9D);
            int a[5] = { 18, 19, 20, 21, 19 };
            struct VTE *e;
            o->n3C = a[p->n30];
            e = &o->vt[8]; e->pfn((char *)o + e->d);
        }
        p->p10->n32C = (unsigned)p->n30 < 2;
        break;
    case -1:
        ClearBhvBusy(p);
        p->p10->n32C = 0;
        break;
    }
}

typedef int qword __attribute__((mode(TI)));
extern float D_002BE694;
struct VTE { short d; short pad; void (*pfn)(void *); };
struct O { char pad[0xC]; struct VTE *vt; char pad2[0x28]; int n38; int n3C; };
struct S { char pad[0x10]; void *p10; int pad2; char a18[0x34]; int n4C; char pad3[0xC0 - 0x50]; int nC0; int pad4; int nC8; int pad5; qword qD0; float gE0; };
void func_00178CD8(struct S *p, int n);
void func_001EF0D8(void *p);
struct O *func_00205438(void *p, int n);
qword func_001A62E8(void *p);
/** @sly2-annot addr=0x1791C8
 * Purpose: BHVWALKPATH SetState: moving states push BHVANIM with walk flag (+0xC0, 8 = with object), stopped
 *   states push BHVIDLE
 * Info: proposed name SetBhvwalkpathState (T1); class BHVWALKPATH slot15; TU guardbhv
 * Notes: runtime2/4 confirmed the BHVWALKPATH role on live guards (S02-S41)
 * Confidence: mid (draft) [T1 0.65] - evidence: runtime (save-state RAM); sources:
 *   r9_bhv_screen+runtime(class role)
 * See: docs/reference/functions/guards-ai.md#guardbhv-0x174898-0x17f090
 * @end-sly2-annot */
void func_001791C8(struct S *p, int n) {
    int old = p->n4C;
    if (n == old) return;
    func_00178CD8(p, n);
    switch (p->n4C) {
    case 0: case 1:
        if ((unsigned)old < 2) return;
        func_001EF0D8(p->a18);
        {
            struct O *o = func_00205438(p->p10, 0x9D);
            struct VTE *e;
            o->n3C = p->nC0;
            if (p->nC0 == 8) o->n38 = p->nC8;
            e = &o->vt[8]; e->pfn((char *)o + e->d);
        }
        p->qD0 = func_001A62E8(p->p10);
        p->gE0 = D_002BE694;
        break;
    case 2: case 3: case 4:
        if (old == 2 || old == 4 || old == 3) return;
        func_001EF0D8(p->a18);
        {
            struct O *o = func_00205438(p->p10, 0x9E);
            struct VTE *e;
            o->n38 = p->nC8;
            e = &o->vt[8]; e->pfn((char *)o + e->d);
        }
        break;
    }
}

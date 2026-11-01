typedef int qword __attribute__((mode(TI)));
struct A { char pad[0x124]; int k124; };
struct S { char pad[0xF80]; int kF80; struct A *pF84; int xF88; int nF8C; void *pF90; };
int func_00132B10(struct A *pa, int n);
qword func_001A62E8(void *p);
void func_001E6F60(struct A *pa, qword q, int a, int b, int c, int d, int *pn);
void func_00204F78(struct S *p, struct A *pa, int n);

/** @sly2-annot addr=0x20B2A8
 * Purpose: Attach to object by class: CID11 pipe?, CID13 rail?, CID19 (k124==3), CID39 -> +0xF80 kind 0..3
 * Info: proposed name AttachStepguard (T2); TU stepguard
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020B2A8(struct S *p, struct A *pa, int n)
{
    p->pF84 = pa;
    p->nF8C = n;
    if (func_00132B10(pa, 0xB)) {
        p->kF80 = 1;
        func_001E6F60(p->pF84, func_001A62E8(p->pF90), 0, 0, 0, 0, &p->xF88);
        func_00204F78(p, p->pF84, 1);
    } else if (func_00132B10(p->pF84, 0xD)) {
        p->kF80 = 0;
        func_001E6F60(p->pF84, func_001A62E8(p), 0, 0, 0, 0, &p->xF88);
        func_00204F78(p, p->pF84, 1);
    } else if (func_00132B10(p->pF84, 0x13) && p->pF84->k124 == 3) {
        p->kF80 = 2;
        func_00204F78(p, p->pF84, 1);
    } else if (func_00132B10(p->pF84, 0x27)) {
        p->kF80 = 3;
        func_00204F78(p, p->pF84, 1);
    } else {
        p->pF84 = 0;
    }
}

struct O { char pad[0x30]; int n30; };
struct L { int k; int rest[27]; };
struct S { char pad[0x26C]; char x26C[0x2DC - 0x26C]; int a2DC[3]; int n2E8; };
void *func_002333B4(void *p, int c, int n);
struct O *PbhvFindBhvstk(void *p, int n);
void SetBhvguardMode(struct O *po, struct L *pl);
/** @sly2-annot addr=0x20A540
 * Purpose: Set +0x2E8; restart BHV156 when it is in sub-state 1
 * Info: proposed name SetStepguardMode2E8 (T2); TU stepguard
 * Confidence: low (draft) [T2 0.25] - evidence: static analysis; sources: r7b_guard
 * See: docs/reference/functions/guards-ai.md#stepguard-0x205e70-0x20b840
 * @end-sly2-annot */
void func_0020A540(struct S *p, int n)
{
    if (p->n2E8 != n) {
        struct O *po;
        p->n2E8 = n;
        func_002333B4(p->a2DC, 0, 0xC);
        po = PbhvFindBhvstk(p->x26C, 0x9C);
        if (po) {
            int k = po->n30;
            if (k == 1) {
                struct L l;
                func_002333B4(&l, 0, sizeof(l));
                l.k = -1;
                SetBhvguardMode(po, &l);
                func_002333B4(p->a2DC, 0, 0xC);
                l.k = k;
                SetBhvguardMode(po, &l);
            }
        }
    }
}

struct K { int k; };
struct R { struct K *pk; int n; float g; };
struct U { char pad[0x38]; float g38; };
struct T { char pad[0xF80]; int nF80; };
struct S { char pad[0x10]; struct T *pt; int pad1; struct U *p18; char pad2[0x18]; int n34; struct K *p38; };
void *func_002333B4(void *p, int c, int n);
/** @sly2-annot addr=0x171C18
 * Purpose: Saves current command/context (and wait time) for later resumption
 * Info: proposed name GetBhv169ResumeState (T2); TU tu_1709A8
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r3b_rest
 * See: docs/reference/functions/guards-ai.md#tu_1709a8-0x1709a8-0x171f58
 * @end-sly2-annot */
void func_00171C18(struct S *p, struct R *r) {
    switch (p->pt->nF80) {
    case -1:
        r->pk = p->p38;
        r->n = p->n34;
        if (r->pk && r->pk->k == 6) r->g = p->p18->g38;
        break;
    case 0: case 1:
        func_002333B4(r, 0, sizeof(*r));
        break;
    }
}

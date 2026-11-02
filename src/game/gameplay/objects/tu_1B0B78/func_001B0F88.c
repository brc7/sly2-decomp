typedef int qword __attribute__((mode(TI)));
struct M { int k; int pad[3]; qword a; qword b; qword c; };
struct VTE { short d; short pad; void (*pfn)(void *, struct M *); };
struct O { char pad[0xC]; struct VTE *vt; };
struct A { char pad[0x20]; qword q20; qword q30; int pad1; int n44; };
struct S { char pad[0x1C8]; struct O *p1C8; char pad1[0x220 - 0x1CC]; int n220; };
int func_001F7588(struct S *p, struct A *pa);
void func_001B1328(struct S *p, int n);
/** @sly2-annot addr=0x1B0F88
 * Purpose: vt75-like zap handler: on accepted hit with flag 8 go to state 5 and send vt51 (explode) to linked
 *   object x1C8
 * Info: proposed name HandleCid96Zap (T2); class CID96 slot75; TU tu_1B0B78
 * Confidence: low (draft) [T2 0.40] - evidence: static analysis; sources: r5b_rest
 * See: docs/reference/functions/gameplay-objects.md#tu_1b0b78-0x1b0b78-0x1b1db8
 * @end-sly2-annot */
int func_001B0F88(struct S *p, struct A *pa)
{
    int r = func_001F7588(p, pa);
    if (r && (pa->n44 & 8) && p->n220 != 5) {
        func_001B1328(p, 5);
        if (p->p1C8) {
            struct M m;
            m.k = 6;
            m.b = pa->q20;
            m.a = pa->q30;
            p->p1C8->vt[51].pfn((char *)p->p1C8 + p->p1C8->vt[51].d, &m);
        }
    }
    return r;
}
